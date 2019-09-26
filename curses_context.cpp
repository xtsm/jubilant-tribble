#include "curses_context.h"
#include "util.h"
#include "global.h"
#include <curses.h>
#include <cmath>
#include <chrono>
#include <thread>

CursesContext::CursesContext() :
        ColorBuffer(),
        ZBuffer(),
        CurrentColor(),
        MSAARate(1),
        BufferWidth(0),
        BufferHeight(0),
        ScreenWidth(0),
        ScreenHeight(0),
        FPSLimit(60),
        FOV(0),
        ContextCamera(),
        Projection(),
        Model(),
        LastDisplayTime() {
    initscr();

    start_color();
    DEBUG << "COLORS: " << COLORS << std::endl;
    DEBUG << "COLOR_PAIRS: " << COLOR_PAIRS << std::endl;
    for (int r = 0; r < 6; r++) {
        for (int g = 0; g < 6; g++) {
            for (int b = 0; b < 6; b++) {
                int id = r + g * 6 + b * 36;
                init_color(id, r * 200, g * 200, b * 200);
                init_pair(id + 1, id, id);
            }
        }
    }
    
    keypad(stdscr, true);
    curs_set(0);
    timeout(0);
    ResizeFit();
    SetFOV(2);
}

CursesContext::~CursesContext() {
    endwin();
}

void CursesContext::SetFPSLimit(int limit) {
    FPSLimit = limit;
}

Camera& CursesContext::GetCamera() {
    return ContextCamera;
}

void CursesContext::SetFOV(double fov) {
    FOV = fov;
    RecalcPerspective();
}

void CursesContext::RecalcPerspective() {
    Projection = mat4::Perspective(FOV, static_cast<double>(ScreenWidth / 2) / ScreenHeight, NEAR, FAR);
}

void CursesContext::SetColor(const vec3& clr) {
    CurrentColor = clr;
}

void CursesContext::ResizeFit() {
    getmaxyx(stdscr, ScreenHeight, ScreenWidth);
    BufferWidth = ScreenWidth / 2 * MSAARate;
    BufferHeight = ScreenHeight * MSAARate;
    RecalcPerspective();
    ColorBuffer.resize(BufferHeight);
    ZBuffer.resize(BufferHeight);
    for (std::vector<vec3>& row : ColorBuffer) {
        row.resize(BufferWidth);
    }
    for (std::vector<double>& row : ZBuffer) {
        row.resize(BufferWidth);
    }
}

void CursesContext::Clear() {
    for (std::vector<vec3>& row : ColorBuffer) {
        std::fill(row.begin(), row.end(), CurrentColor);
    }
    for (std::vector<double>& row : ZBuffer) {
        std::fill(row.begin(), row.end(), -1.1);
    }
}

void CursesContext::Display() {
    for (int i = 0; i < ScreenHeight; i++) {
        move(i, 0);
        for (int j = 0; j < ScreenWidth / 2; j++) {
            vec3 clr = ColorBuffer[i][j];
            size_t color_id = roundl(NormU(clr.x) * 5) + roundl(NormU(clr.y) * 5) * 6 + roundl(NormU(clr.z) * 5) * 36;
            attron(COLOR_PAIR(color_id + 1));
            printw("##");
        }
    }
    if (FPSLimit > 0) {
        auto frameTime = std::chrono::microseconds(1000000) / FPSLimit;
        auto currentTime = std::chrono::steady_clock::now() - LastDisplayTime;
        if (frameTime > currentTime) {
            std::this_thread::sleep_for(frameTime - currentTime);
        }
    }
    refresh();
    LastDisplayTime = std::chrono::steady_clock::now();
}

void CursesContext::SetMSAARate(int rate) {
    MSAARate = rate;
}

void CursesContext::DrawLine(vec3 p1, vec3 p2) {
    p1 = ContextCamera.GetView()(Model(p1));
    p2 = ContextCamera.GetView()(Model(p2));
    if (!NormSegmentZ(p1, p2, NEAR, FAR)) {
        return;
    }
    p1 = Projection(p1);
    p2 = Projection(p2);
    if (!NormSegmentXY(p1, p2)) {
        return;
    }
    p1.x = roundl((BufferWidth - 1) * (p1.x + 1) / 2);
    p1.y = roundl((BufferHeight - 1) * (-p1.y + 1) / 2);
    std::swap(p1.x, p1.y);
    p2.x = roundl((BufferWidth - 1) * (p2.x + 1) / 2);
    p2.y = roundl((BufferHeight - 1) * (-p2.y + 1) / 2);
    std::swap(p2.x, p2.y);

    vec3 d = p2 - p1;
    double m = std::max(std::abs(d.x), std::abs(d.y));
    if (m > 0.9) {
        d /= m;
    }
    if (ZBuffer[roundl(p1.x)][roundl(p1.y)] < p1.z) {
        ZBuffer[roundl(p1.x)][roundl(p1.y)] = p1.z;
        ColorBuffer[roundl(p1.x)][roundl(p1.y)] = CurrentColor;
    }
    while (p1 - p2) {
        p1 += d;
        if (ZBuffer[roundl(p1.x)][roundl(p1.y)] < p1.z) {
            ZBuffer[roundl(p1.x)][roundl(p1.y)] = p1.z;
            ColorBuffer[roundl(p1.x)][roundl(p1.y)] = CurrentColor;
        }
    }
}
