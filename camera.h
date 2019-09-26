#pragma once
#include "util.h"

class Camera {
public:
    Camera();
    vec3 GetDirection();
    Camera& Rotate(double azi, double zen);
    Camera& SetRotation(double azi, double zen);
    Camera& Move(const vec3& vec);
    Camera& SetPosition(const vec3& vec);
    const mat4& GetView();
private:
    mat4 View;
    bool Recalc;
    vec3 Position;
    double RotAzi, RotZen;
};
