#include "global.h"
#include "curses_context.h"
#include "util.h"
#include <curses.h>
#include <random>
#include <cmath>
#include <iostream>
const size_t N = 20;
double f[N][N];

int main() {
    DEBUG.open("debug.log");
    CursesContext ctx;

    for (size_t frame = 0; frame < 10000; frame++) {
        int ch;
        while ((ch = getch()) != ERR) {
            switch (ch) {
                case KEY_LEFT:
                    ctx.GetCamera().Rotate(M_PI / 24, 0);
                    break;
                case KEY_RIGHT:
                    ctx.GetCamera().Rotate(-M_PI / 24, 0);
                    break;
                case KEY_UP:
                    ctx.GetCamera().Rotate(0, M_PI / 24);
                    break;
                case KEY_DOWN:
                    ctx.GetCamera().Rotate(0, -M_PI / 24);
                    break;
                case KEY_RESIZE:
                    ctx.ResizeFit();
                    break;
            }
        }

        ctx.SetColor({0.1, 0.1, 0.1});
        ctx.Clear();
        ctx.SetColor({0, 1, 0});
        for (size_t i = 0; i < N; i++) {
          for (size_t j = 0; j < N; j++) {
            if (i + 1 < N) {
              ctx.DrawLine({static_cast<double>(i), static_cast<double>(j), f[i][j] - 5}, {static_cast<double>(i + 1), static_cast<double>(j), f[i + 1][j] - 5});
            }
            if (j + 1 < N) {
              ctx.DrawLine({static_cast<double>(i), static_cast<double>(j), f[i][j] - 5}, {static_cast<double>(i), static_cast<double>(j + 1), f[i][j + 1] - 5});
            }
            f[i][j] = sin((frame + i + j) / 10.0);
          }
        }
        ctx.Display();
    }
    return 0;
}
