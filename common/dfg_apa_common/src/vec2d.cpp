// Copyright [2019] - MiniEye INC.

#include "vec2d.h"
#include <cmath>
#include "glog/logging.h"

namespace apa {

    Vec2d Vec2d::CreateUnitVec2d(const float angle) {
        return Vec2d(cos(angle), sin(angle));
    }

    float Vec2d::Length() const { return std::hypot(m_x, m_y); }

    float Vec2d::LengthSquare() const { return m_x * m_x + m_y * m_y; }

    float Vec2d::Angle() const {return std::atan2(m_y, m_x); }

    void Vec2d::Normalize() {
        const float l = Length();
        if (l > kMathEpsilon) {
            m_x /= l;
            m_y /= l;
        } else {
            LOG(ERROR) << "Length of vector is near zero, please check the vector!";
        }
    }

    float Vec2d::DistanceTo(const Vec2d &other) const {
        return std::hypot(m_x - other.m_x, m_y - other.m_y);
    }

    float Vec2d::DistanceToBC(const Vec2d &b, const Vec2d &c) const {
        const float k1 = 2;
        const float k2 = 0.0625;
        float res = 0;
        Vec2d a(m_x, m_y);
        const float ab = a.DistanceTo(b);
        const float ac = a.DistanceTo(c);
        const float bc = b.DistanceTo(c);
        if (ab <= 0 || ac <= 0 || bc <= 0) {
            LOG(ERROR) << "CalDistFromAToBC: input is invalid!";
            return res;
        }
        res = k1 * sqrt(k2 *(ac + ab + bc) * (ac + ab - bc) * (ac + bc - ab) * (ab + bc - ac)) / bc;
        return res;
    }

    float Vec2d::DistanceSquareTo(const Vec2d &other) const {
        const float dx = m_x - other.m_x;
        const float dy = m_y - other.m_y;
        return dx * dx + dy * dy;
    }

    float Vec2d::CrossProd(const Vec2d &other) const {
        return m_x * other.y() - m_y * other.x();
    }

    float Vec2d::InnerProd(const Vec2d &other) const {
        return m_x * other.x() + m_y * other.y();
    }

    Vec2d Vec2d::rotate(const float angle) const {
        return Vec2d(m_x * cos(angle) - m_y * sin(angle), m_y * cos(angle) + m_x * sin(angle));
    }

    void Vec2d::SelfRotate(const float angle) {
        float temp_x = m_x;
        m_x = m_x * cos(angle) - m_y * sin(angle);
        m_y = m_y * cos(angle) + temp_x * sin(angle);
    }

    Vec2d Vec2d::operator+(const Vec2d &other) const {
        return Vec2d(m_x + other.x(), m_y + other.y());
    }

    Vec2d Vec2d::operator-(const Vec2d &other) const {
        return Vec2d(m_x - other.x(), m_y - other.y());
    }

    Vec2d Vec2d::operator*(const float ratio) const {
        return Vec2d(m_x * ratio, m_y * ratio);
    }

    Vec2d Vec2d::operator/(const float ratio) const {
        if (std::abs(ratio) > kMathEpsilon) {
            return Vec2d(m_x / ratio, m_y / ratio);
        } else {
        LOG(ERROR) << "ratio is near zero, please check the ratio!";
        return Vec2d(m_x, m_y);
        }
    }

    Vec2d &Vec2d::operator+=(const Vec2d &other) {
        m_x += other.x();
        m_y += other.y();
        return *this;
    }

    Vec2d &Vec2d::operator-=(const Vec2d &other) {
        m_x -= other.x();
        m_y -= other.y();
        return *this;
    }

    Vec2d &Vec2d::operator*=(const float ratio) {
        m_x *= ratio;
        m_y *= ratio;
        return *this;
    }

    Vec2d &Vec2d::operator/=(const float ratio) {
        if (std::abs(ratio) > kMathEpsilon) {
            m_x /= ratio;
            m_y /= ratio;
        } else {
            LOG(ERROR) << "ratio is near zero, please check the ratio!";
        }
        return *this;
    }

    bool Vec2d::operator==(const Vec2d &other) const {
        return (std::abs(m_x - other.x() < kMathEpsilon) && std::abs(m_y - other.y() < kMathEpsilon));
    }

    Vec2d operator*(const float ratio, const Vec2d &vec) {
        return vec * ratio;
    }

    std::string Vec2d::DebugString() const {
        std::string str;
        str = "vec2d ( x = " + std::to_string(m_x) + ", y = " + std::to_string(m_y) + ")";
        return str;
    }

}

