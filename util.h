#pragma once
#include <iostream>
#include "matmul.h"

const double EPS = 1e-4;

struct vec3 {
    double x, y, z;
    vec3 operator+(const vec3& rhs) const;
    vec3 operator-(const vec3& rhs) const;
    vec3 operator*(double val) const;
    vec3 operator/(double val) const;
    vec3 operator-() const;
    vec3& operator+=(const vec3& rhs);
    vec3& operator-=(const vec3& rhs);
    vec3& operator*=(double val);
    vec3& operator/=(double val);
    operator bool() const;
};

struct mat4 {
    alignas(32) double data[16];
    mat4();
    mat4& operator=(const mat4& rhs);
    mat4& operator*=(const mat4& rhs);
    vec3 operator()(const vec3& vec) const;
    static mat4 Perspective(double fov, double ratio, double near, double far);
    static mat4 RotZ(double angle);
    static mat4 RotX(double angle);
    static mat4 Translate(const vec3& vec);
};

std::ostream& operator<<(std::ostream& os, const vec3& val);
std::ostream& operator<<(std::ostream& os, const mat4& val);

double NormU(double x);

double NormS(double x);

bool NormSegmentXY(vec3& a, vec3& b);

bool NormSegmentZ(vec3& a, vec3& b, double near, double far);
