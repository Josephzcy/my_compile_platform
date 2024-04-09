// Copyright [2019] - MiniEye INC.

/**
 * @file
 * @brief Defines the Vec2d class
 */

#pragma once

#include <cmath>
#include <string>

namespace apa {

constexpr float kMathEpsilon = 1e-10;
constexpr float D2R = M_PI / 180.0;
constexpr float R2D = 180.0 / M_PI;

/**
 * @class Vec2d
 * @brief Implements a class of 2-dimensional vectors
 */
class Vec2d{
 public:
    //! Constructor which takes x and y coordinates.
    Vec2d(const float x, const float y) : m_x(x), m_y(y) {}

    //! Constructor return the zero vector.
    Vec2d() : Vec2d(0.f, 0.f) {}

    const Vec2d& operator=(const Vec2d& lhs) {
       m_x = lhs.m_x;
       m_y = lhs.m_y;
       return *this;
    }

    //! Creates a unit-vector which a given angle(rad) to the positive x semi-axis.
    static Vec2d CreateUnitVec2d(const float angle);

    //! Getter for x component.
    float x() const { return m_x; }

    //! Getter for y component.
    float y() const { return m_y; }

    //! Setter for x component.
    void set_x(const float x) { m_x = x; }

    //! Setter for y component.
    void set_y(const float y) { m_y = y; }

    //! Gets the length of the vector.
    float Length() const;

    //! Gets the squared length of the vector.
    float LengthSquare() const;

    //! Gets the angle(rad) between the vector and the positive x semi-axis.
    float Angle() const;

    //! Returns the unit vector that co-linear with this vector.
    void Normalize();

    //! Returns the distance to the given vector.
    float DistanceTo(const Vec2d &other) const;

    //! Returns the distance to the given vector by vec2d b and vec2d c.
    float DistanceToBC(const Vec2d &b, const Vec2d &c) const;

    //! Returns the squared distance to the given vector.
    float DistanceSquareTo(const Vec2d &other) const;

    //! Returns the "cross" product between these two Vec2d.
    float CrossProd(const Vec2d &other) const;

    //! Returns the inner product between these two Vec2d.
    float InnerProd(const Vec2d &other) const;

    //! rotate clockwise the vector by angle(rad).
    Vec2d rotate(const float angle) const;

    //! rotate clockwise the vector itself by angle(rad).
    void SelfRotate(const float angle);

    //! Sums two vec2d.
    Vec2d operator+(const Vec2d &other) const;

    //! Subtracts two vec2d.
    Vec2d operator-(const Vec2d &other) const;

    //! Multiplies vec2d by a scalar.
    Vec2d operator*(const float ratio) const;

    //! Divides vec2d by a scalar.
    Vec2d operator/(const float ratio) const;

     //! Sums another vec2d to the current one.
    Vec2d &operator+=(const Vec2d &other);

    //! Subtracts another vec2d to the current one.
    Vec2d &operator-=(const Vec2d &other);

    //! Multiplies this vec2d by a scalar.
    Vec2d &operator*=(const float ratio);

    //! Divides this vec2d by a scalar.
    Vec2d &operator/=(const float ratio);

    //! Compare two vec2d.
    bool operator==(const Vec2d &other) const;

    //! Return a human-readable string representing this object.
    std::string DebugString() const;

 protected:
    float m_x;
    float m_y;
};

//! Multiplies the given vec2d by a given scalar.
Vec2d operator*(const float ratio, const Vec2d &vec);

}
