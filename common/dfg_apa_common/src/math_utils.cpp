// Copyright [2019] - MiniEye INC.

#include "math_utils.h"
#include <cmath>
#include <utility>
#include "glog/logging.h"

namespace apa {

float Sqr(const float x) { return x * x; }

float CrossProd(const Vec2d &start_point, const Vec2d &end_point1, const Vec2d &end_point2) {
    return (end_point1 - start_point).CrossProd(end_point2 - start_point);
}

float InnerProd(const Vec2d &start_point, const Vec2d &end_point1, const Vec2d &end_point2) {
    return (end_point1 - start_point).InnerProd(end_point2 - start_point);
}

float CrossProd(const float x0, const float y0, const float x1, const float y1) {
    return x0 * y1 - x1 * y0;
}

float InnerProd(const float x0, const float y0, const float x1, const float y1) {
    return x0 * x1 + y0 * y1;
}

float WrapAngle(const float angle) {
    const float new_angle = std::fmod(angle, M_PI * 2.0);
    return new_angle < 0 ? new_angle + M_PI * 2.0 : new_angle;
}

float NormalizeAngle(const float angle) {
    float a = std::fmod(angle + M_PI, M_PI * 2.0);
    if (a < 0.0) {
        a += (M_PI * 2.0);
    }
    return a - M_PI;
}

float AngleDiff(const float from, const float to) {
    return NormalizeAngle(to - from);
}

int RandomInteger(const int s, const int t, unsigned int random_seed) {
    if (s >= t) {
        return s;
    }
    return s + rand_r(&random_seed) % (t - s + 1);
}

double RandomDouble(const double s, const double t, unsigned int random_seed) {
    return s + (t - s) / 16383.0 * (rand_r(&random_seed) & 16383);
}

double Gaussian(const double u, const double std, const double x) {
    return (1.0 / std::sqrt(2 * M_PI * std * std)) * std::exp(-(x - u) * (x - u) / (2 * std * std));
}

double Sigmoid(const double x) {
    return 1.0 / (1.0 + std::exp(-x));
}

Eigen::Vector2d RotateVector2d(const Eigen::Vector2d &v_in, const float theta) {
    const float cos_theta = std::cos(theta);
    const float sin_theta = std::sin(theta);

    auto x = cos_theta * v_in.x() - sin_theta * v_in.y();
    auto y = sin_theta * v_in.x() + cos_theta * v_in.y();

    return {x, y};
}

std::pair<float, float> Cartesian2Polar(float x, float y) {
    float r = std::sqrt(x * x + y * y);
    float theta = std::atan2(y, x);
    return std::make_pair(r, theta);
}
}

