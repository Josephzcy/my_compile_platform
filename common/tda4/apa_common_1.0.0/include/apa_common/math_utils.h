// Copyright [2019] - MiniEye INC.
#pragma once

#include <limits>
#include <utility>

#include "Eigen/Dense"
#include "./vec2d.h"

namespace apa {

float Sqr(const float x);

/**
 * @brief Cross product between two 2-D vectors from the common start point and end at two other points.
 * @param start_point The common start point of two vectors in 2-D
 * @param end_point1 The end point of the first vector
 * @param end_point2 The end point of the second vector
 * 
 * @return the cross product result
 */
float CrossProd(const Vec2d &start_point, const Vec2d &end_point1, const Vec2d &end_point2);

/**
 * @brief Inner product between two 2-D vectors from the common start point and end at two other points.
 * @param start_point The common start point of two vectors in 2-D
 * @param end_point1 The end point of the first vector
 * @param end_point2 The end point of the second vector
 * 
 * @return the cross product result
 */
float InnerProd(const Vec2d &start_point, const Vec2d &end_point1, const Vec2d &end_point2);

/**
 * @brief Cross product between two vectors, one vector is formed by 1st and 2nd parameters of the function,
 *        the other vector is formed by 3rd and 4th parameters of the function.
 * @param x0 the x coordinate of the first vector
 * @param y0 the y coordinate of the first vector
 * @param x1 the x coordinate of the second vector
 * @param y1 the y coordinate of the second vector
 * 
 * @return the cross product result
 */
float CrossProd(const float x0, const float y0, const float x1, const float y1);

/**
 * @brief Inner product between two vectors, one vector is formed by 1st and 2nd parameters of the function,
 *        the other vector is formed by 3rd and 4th parameters of the function.
 * @param x0 the x coordinate of the first vector
 * @param y0 the y coordinate of the first vector
 * @param x1 the x coordinate of the second vector
 * @param y1 the y coordinate of the second vector
 * 
 * @return the Inner product result
 */
float InnerProd(const float x0, const float y0, const float x1, const float y1);

/**
 * @brief Wrap angle to [0, 2 * PI].
 * @param angle the original value of angle
 * 
 * @return the wrapped value of the angle
 */
float WrapAngle(const float angle);

/**
 * @brief Normalize angle to [-PI, PI].
 * @param angle the original value of angle
 * 
 * @return the normalized value of the angle
 */
float NormalizeAngle(const float angle);

/**
 * @brief Calculate the difference between angle from and to.
 * @param from the start angle
 * @param to the end angle
 * 
 * @return the difference between from and to. The range is between [-PI, PI].
 */
float AngleDiff(const float from, const float to);

/**
 * @brief Get a random integer between two integer values by a random seed.
 * @param s the lower bound of the random float.
 * @param t the upper bound of the random float.
 * @param random_seed the random seed.
 * 
 * @return A random float between s and t based on the input random_seed.
 */
int RandomInteger(const int s, const int t, unsigned int random_seed = 1);

double RandomDouble(const double s, const double t, unsigned int random_seed = 1);

/**
 * @brief Compute squared value.
 * @param value the target value to get its squared value
 * 
 * @return Squared value of the input value.
 */
template<typename T>
inline T Square(const T value) {
    return value * value;
}

/**
 * @brief Clamp a value between two bounds.
 *        If the value goes beyond the bounds, return one of the bounds, otherwise return the orginal value.
 * @param value the original value to be clamped.
 * @param bound1 One bound to clamp the value.
 * @param bound2 the other bound to clamp the value.
 * 
 * @return The clamped value.
 */
template<typename T>
T Clamp(const T value, T bound1, T bound2) {
    if (bound1 > bound2) {
        std::swap(bound1, bound2);
    }

    if (value < bound1) {
        return bound1;
    } else if (value > bound2) {
        return bound2;
    } else {
        return value;
    }
}

//  Gaussian
double Gaussian(const double u, const double std, const double x);

//  Sigmoid
double Sigmoid(const double x);

//  Rotate a 2d vector counter-clockwise by theta, 逆时针
Eigen::Vector2d RotateVector2d(const Eigen::Vector2d &v_in, const float theta);

inline std::pair<float, float> RFUToFLU(const float x, const float y) {
    return std::make_pair(y, -x);
}

inline std::pair<float, float> FLUToRFU(const float x, const float y) {
    return std::make_pair(-y, x);
}

//  Norm one dimensional data
inline void L2Norm(int feat_dim, float *feat_data) {
    if (feat_dim == 0) {
        return;
    }
    //  feature normalization
    float l2norm = 0.0;
    for (int i = 0; i < feat_dim; ++i) {
        l2norm += feat_data[i] * feat_data[i];
    }
    if (l2norm == 0) {
        float val = 1.f / std::sqrt(static_cast<float>(feat_dim));  // 1.f 表示float类型，1.0为double
        for (int i = 0; i< feat_dim; ++i) {
            feat_data[i] = val;
        }
    } else {
        l2norm = std::sqrt(l2norm);
        for (int i = 0; i< feat_dim; ++i) {
            feat_data[i] /= l2norm;
        }
    }
}

//  Cartesian coordinates to polar coordinates
std::pair<float, float> Cartesian2Polar(float x, float y);

}
