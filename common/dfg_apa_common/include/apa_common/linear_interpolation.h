// Copyright [2019] - MiniEye INC.

/**
 * @file
 * @brief Linear interpolation functions.
 */

#pragma once

#include <cmath>
#include "glog/logging.h"
#include "modules/common/math/math_utils.h"

namespace apa {

/**
 * @brief Linear interpolation between two points of type T.
 * @param x0 The coordinate of the first point.
 * @param t0 The interpolation parameter of the first point.
 * @param x1 The coordinate of the second point.
 * @param t1 The interpolation parameter of the second point.
 * @param t The interpolation parameter for interpolation.
 * @param x The coordinate of the interpolated point.
 * @return Interpolated point.
 */
template <typename T>
T lerp(const T &x0, const double t0, const T &x1, const double t1, const double t) {
    if (std::abs(t1 - t0) <= 1.0e-6) {
        LOG(ERROR) << "input time difference is too small";
        return x0;
    }
    const double r = (t - t0) / (t1 - t0);
    const T x = x0 + r * (x1 - x0);
    return x;
}

/**
 * @brief Spherical linear interpolation between two angles.
 *        The two angles are within range [-M_PI, M_PI).
 * @param a0 The value of the first angle.
 * @param t0 The interpolation parameter of the first angle.
 * @param a1 The value of the second angle.
 * @param t1 The interpolation parameter of the second angle.
 * @param t The interpolation parameter for interpolation.
 * @param a The value of the spherically interpolated angle.
 * @return Interpolated angle.
 */
double slerp(const double a0, const double t0, const double a1, const double t1, const double t) {
    if (std::abs(t1 - t0) <= kMathEpsilon) {
        LOG(ERROR) << "input time difference is too small";
        return NormalizeAngle(a0);
    }
    const double a0_n = NormalizeAngle(a0);
    const double a1_n = NormalizeAngle(a1);
    double d = a1_n - a0_n;
    if (d > M_PI) {
        d = d - 2 * M_PI;
    } else if (d < -M_PI) {
        d = d + 2 * M_PI;
    }

    const double r = (t - t0) / (t1 - t0);
    const double a = a0_n + d * r;
    return NormalizeAngle(a);
}
}  // apa
