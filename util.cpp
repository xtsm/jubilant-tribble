#include "util.h"
#include "global.h"
#include <algorithm>
#include <cmath>

double NormU(double x) {
    return x < 0 ? 0 : x > 1 ? 1 : x;
}

double NormS(double x) {
    return x < -1 ? -1 : x > 1 ? 1 : x;
}

bool NormSegmentXY(vec3& a, vec3& b) {
    double l = 0, r = 1;
    if (a.x == b.x) {
        if (a.x < -1 || a.x > 1) {
            return false;
        }
    } else {
        l = std::max(l, ((a.x < b.x ? -1 : 1) - a.x) / (b.x - a.x));
        r = std::min(r, ((a.x < b.x ? 1 : -1) - a.x) / (b.x - a.x));
    }
    if (a.y == b.y) {
        if (a.y < -1 || a.y > 1) {
            return false;
        }
    } else {
        l = std::max(l, ((a.y < b.y ? -1 : 1) - a.y) / (b.y - a.y));
        r = std::min(r, ((a.y < b.y ? 1 : -1) - a.y) / (b.y - a.y));
    }
    if (l > r) return false;
    vec3 d = b - a;
    b = a + d * r;
    a = a + d * l;
    return true;
}

vec3 vec3::operator+(const vec3& rhs) const {
    vec3 ret = *this;
    return ret += rhs;
}
vec3 vec3::operator-(const vec3& rhs) const {
    vec3 ret = *this;
    return ret -= rhs;
}
vec3 vec3::operator*(double val) const {
    vec3 ret = *this;
    return ret *= val;
}
vec3 vec3::operator/(double val) const {
    vec3 ret = *this;
    return ret /= val;
}
vec3 vec3::operator-() const {
    return {-x, -y, -z};
}
vec3& vec3::operator+=(const vec3& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}
vec3& vec3::operator-=(const vec3& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}
vec3& vec3::operator*=(double val) {
    x *= val;
    y *= val;
    z *= val;
    return *this;
}
vec3& vec3::operator/=(double val) {
    x /= val;
    y /= val;
    z /= val;
    return *this;
}
vec3::operator bool() const {
    return std::abs(x) > EPS || std::abs(y) > EPS || std::abs(z) > EPS;
}

std::ostream& operator<<(std::ostream& os, const vec3& val) {
    return os << '(' << val.x << ',' << val.y << ',' << val.z << ')';
}

std::ostream& operator<<(std::ostream& os, const mat4& val) {
    for (int i = 0; i < 16; i++ ) {
        os << val.data[i] << " ";
        if (i % 4 == 3) os << std::endl;
    }
    return os;
}

mat4::mat4() {
    std::fill(data, data + 16, 0);
    data[0] = data[5] = data[10] = data[15] = 1;
}

mat4::mat4(const mat4& rhs) {
    std::copy(rhs.data, rhs.data + 16, data);
}

mat4& mat4::operator=(const mat4& rhs) {
    std::copy(rhs.data, rhs.data + 16, data);
    return *this;
}

mat4& mat4::operator*=(const mat4& rhs) {
    matmul(data, rhs.data);
    return *this;
}

vec3 mat4::operator()(const vec3& vec) const {
    double vecData[4] = {vec.x, vec.y, vec.z, 1};
    vecmul(data, vecData);
    return {vecData[0] / vecData[3], vecData[1] / vecData[3], vecData[2] / vecData[3]};
}

mat4 mat4::Perspective(double fov, double ratio, double near, double far) {
    mat4 ret;
    ret.data[0] = 1.0 / tan(fov / 2);
    ret.data[5] = ret.data[0] * ratio;
    ret.data[10] = (near + far) / (near - far);
    ret.data[11] = 2 * near * far / (far - near);
    ret.data[14] = 1;
    ret.data[15] = 0;
    return ret;
}
mat4 mat4::RotZ(double angle) {
    mat4 ret;
    ret.data[0] = cos(angle);
    ret.data[4] = sin(angle);
    ret.data[5] = ret.data[0];
    ret.data[1] = -ret.data[4];
    return ret;
}
mat4 mat4::RotX(double angle) {
    mat4 ret;
    ret.data[5] = cos(angle);
    ret.data[9] = sin(angle);
    ret.data[10] = ret.data[5];
    ret.data[6] = -ret.data[9];
    return ret;
}
mat4 mat4::Translate(const vec3& vec) {
    mat4 ret;
    ret.data[3] = vec.x;
    ret.data[7] = vec.y;
    ret.data[11] = vec.z;
    return ret;
}

bool NormSegmentZ(vec3& a, vec3& b, double near, double far) {
    double l = 0, r = 1; 
    if (a.z == b.z) {
        if (a.z < near || a.z > far) {
            return false;
        }
    } else {
        l = std::max(l, ((a.z < b.z ? near : far) - a.z) / (b.z - a.z));
        r = std::min(r, ((a.z < b.z ? far : near) - a.z) / (b.z - a.z));
    }
    if (l > r) return false;
    vec3 d = b - a;
    b = a + d * r;
    a = a + d * l;
    return true;

}
