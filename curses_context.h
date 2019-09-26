#pragma once
#include "util.h"
#include "camera.h"
#include <cstdint>
#include <vector>
#include <fstream>
#include <chrono>

class CursesContext {
public:
    CursesContext();
    ~CursesContext();
    void SetColor(const vec3& clr);
    void SetMSAARate(int rate);
    void Clear();
    void ResizeFit();
    void Display();
    void SetFOV(double fov);
    void SetFPSLimit(int limit);
    void DrawLine(vec3 p1, vec3 p2);
    void RecalcPerspective();
    Camera& GetCamera();
private:
    std::vector<std::vector<vec3>> ColorBuffer;
    std::vector<std::vector<double>> ZBuffer;
    vec3 CurrentColor;
    int MSAARate;
    int BufferWidth, BufferHeight;
    int ScreenWidth, ScreenHeight;
    size_t FPSLimit;
    double FOV;
    Camera ContextCamera;
    mat4 Projection, Model;
    std::chrono::time_point<std::chrono::steady_clock> LastDisplayTime;
};
