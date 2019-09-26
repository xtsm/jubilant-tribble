#include "camera.h"
#include "global.h"
#include <cmath>

Camera::Camera() :
        View(),
        Recalc(true),
        Position{0, 0, 0},
        RotAzi(0),
        RotZen(0) {}
vec3 Camera::GetDirection() {
    return {sin(RotAzi) * cos(RotZen), cos(RotAzi) * cos(RotZen), sin(RotZen)};
}
Camera& Camera::Rotate(double azi, double zen) {
    RotAzi += azi;
    RotZen += zen;
    Recalc = true;
    return *this;
}
Camera& Camera::SetRotation(double azi, double zen) {
    RotAzi = azi;
    RotZen = zen;
    Recalc = true;
    return *this;
}
Camera& Camera::Move(const vec3& vec) {
    Position += vec;
    Recalc = true;
    return *this;
}
Camera& Camera::SetPosition(const vec3& vec) {
    Position = vec;
    Recalc = true;
    return *this;
}
const mat4& Camera::GetView() {
    if (Recalc) {
        View = mat4();
        View.data[5] = -1;
        View *= mat4::RotX(M_PI / 2 - RotZen);
        View *= mat4::RotZ(-RotAzi);
        View *= mat4::Translate(-Position);
        DEBUG << View << GetDirection() << std::endl;
        Recalc = false;
    }
    return View;
}
