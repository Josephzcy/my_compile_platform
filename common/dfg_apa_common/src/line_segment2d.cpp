// Copyright [2019] - MiniEye INC.
#include <cmath>
#include <algorithm>
#include <utility>
#include "glog/logging.h"
#include "line_segment2d.h"
#include "math_utils.h"

namespace apa {
namespace {

const float scale_ratio = 5000.f;

bool IsWithin(double val, double bound1, double bound2) {
  if (bound1 > bound2) {
    std::swap(bound1, bound2);
  }
  return val >= bound1 - kMathEpsilon && val <= bound2 + kMathEpsilon;
}

}  // namespace

LineSegment2d::LineSegment2d() {
    m_unit_direction = Vec2d(1, 0);
    m_unit_normal_clockwise = Vec2d(0, 1);
    m_unit_normal_anticlockwise = Vec2d(0, 1);
    m_heading = 0.f;
    m_length = 0.f;
}

LineSegment2d::LineSegment2d(const Vec2d &start, const Vec2d &end) : m_start(start), m_end(end) {
    const float dx = m_end.x() - m_start.x();
    const float dy = m_end.y() - m_start.y();
    m_length = hypot(dx, dy);
    m_unit_direction = (m_length <= kMathEpsilon ? Vec2d(0.f, 0.f) : Vec2d(dx / m_length, dy / m_length));
    m_unit_normal_clockwise = m_unit_direction.rotate(M_PI_2);
    m_unit_normal_anticlockwise = m_unit_direction.rotate(-M_PI_2);
    m_heading = m_unit_direction.Angle();
}

Vec2d LineSegment2d::rotate(const float angle) {
    Vec2d diff_vec = m_end - m_start;
    diff_vec.SelfRotate(angle);
    return m_start + diff_vec;
}

float LineSegment2d::DistanceTo(const Vec2d &point) const {
    if (m_length <= kMathEpsilon) {
        return point.DistanceTo(m_start);
    }
    const float x0 = point.x() - m_start.x();
    const float y0 = point.y() - m_start.y();
    // proj为point在本线段上面的投影
    const float proj = x0 * m_unit_direction.x() + y0 * m_unit_direction.y();
    if (proj <= 0.0) {
    // 说明point在线段起点外侧，那么距离直接就是point和m_start距离
        return hypot(x0, y0);
    }
    if (proj >= m_length) {
    // 说明point在线段终点外侧，那么距离直接就是point和m_end距离
        return point.DistanceTo(m_end);
    }
    // 说明point在线段区间内，距离是投影
    return std::abs(x0 * m_unit_direction.y() - y0 * m_unit_direction.x());
}

float LineSegment2d::DistanceTo(const Vec2d &point, Vec2d *const nearest_pt) const {
    CHECK_NOTNULL(nearest_pt);
    if (m_length <= kMathEpsilon) {
        *nearest_pt = m_start;
        return point.DistanceTo(m_start);
    }
    const float x0 = point.x() - m_start.x();
    const float y0 = point.y() - m_start.y();
    const float proj = x0 * m_unit_direction.x() + y0 * m_unit_direction.y();
    if (proj < 0.0) {
        *nearest_pt = m_start;
        return hypot(x0, y0);
    }
    if (proj > m_length) {
        *nearest_pt = m_end;
        return point.DistanceTo(m_end);
    }
    *nearest_pt = m_start + m_unit_direction * proj;
    return std::abs(x0 * m_unit_direction.y() - y0 * m_unit_direction.x());
}


float LineSegment2d::DistanceSquareTo(const Vec2d &point) const {
    if (m_length <= kMathEpsilon) {
        return point.DistanceSquareTo(m_start);
    }
    const float x0 = point.x() - m_start.x();
    const float y0 = point.y() - m_start.y();
    const float proj = x0 * m_unit_direction.x() + y0 * m_unit_direction.y();
    if (proj <= 0.0) {
        return Square(x0) + Square(y0);
    }
    if (proj >= m_length) {
        return point.DistanceSquareTo(m_end);
    }
    return Square(x0 * m_unit_direction.y() - y0 * m_unit_direction.x());
}

float LineSegment2d::DistanceSquareTo(const Vec2d &point, Vec2d *const nearest_pt) const {
    CHECK_NOTNULL(nearest_pt);
    if (m_length <= kMathEpsilon) {
        *nearest_pt = m_start;
        return point.DistanceSquareTo(m_start);
    }
    const float x0 = point.x() - m_start.x();
    const float y0 = point.y() - m_start.y();
    const float proj = x0 * m_unit_direction.x() + y0 * m_unit_direction.y();
    if (proj <= 0.0) {
        *nearest_pt = m_start;
        return Square(x0) + Square(y0);
    }
    if (proj >= m_length) {
        *nearest_pt = m_end;
        return point.DistanceSquareTo(m_end);
    }
    *nearest_pt = m_start + m_unit_direction * proj;
    return Square(x0 * m_unit_direction.y() - y0 * m_unit_direction.x());
}

bool LineSegment2d::IsPointIn(const Vec2d &point) const {
    if (m_length <= kMathEpsilon) {
        return std::abs(point.x() - m_start.x()) <= kMathEpsilon &&
            std::abs(point.y() - m_start.y()) <= kMathEpsilon;
    }
    const float prod = CrossProd(point, m_start, m_end);
    if (std::abs(prod) > kMathEpsilon) {
        return false;
    }
    return IsWithin(point.x(), m_start.x(), m_end.x()) && IsWithin(point.y(), m_start.y(), m_end.y());
}

float LineSegment2d::ProjectOntoUnit(const Vec2d &point) const {
    return m_unit_direction.InnerProd(point - m_start);
}

float LineSegment2d::ProductOntoUnit(const Vec2d &point) const {
    return m_unit_direction.CrossProd(point - m_start);
}

bool LineSegment2d::HasIntersect(const LineSegment2d &other_segment) const {
    Vec2d point;
    return GetIntersect(other_segment, &point);
}

bool LineSegment2d::GetIntersect(const LineSegment2d &other_segment, Vec2d *const point) const {
    CHECK_NOTNULL(point);
    if (IsPointIn(other_segment.start())) {
        *point = other_segment.start();
        return true;
    }
    if (IsPointIn(other_segment.end())) {
        *point = other_segment.end();
        return true;
    }
    if (other_segment.IsPointIn(m_start)) {
        *point = m_start;
        return true;
    }
    if (other_segment.IsPointIn(m_end)) {
        *point = m_end;
        return true;
    }
    if (m_length <= kMathEpsilon || other_segment.length() <= kMathEpsilon) {
        return false;
    }
    const float cc1 = CrossProd(m_start, m_end, other_segment.start());
    const float cc2 = CrossProd(m_start, m_end, other_segment.end());
    if (cc1 * cc2 >= -kMathEpsilon) {
        return false;
    }
    const float cc3 =
        CrossProd(other_segment.start(), other_segment.end(), m_start);
    const float cc4 =
        CrossProd(other_segment.start(), other_segment.end(), m_end);
    if (cc3 * cc4 >= -kMathEpsilon) {
        return false;
    }
    const float ratio = cc4 / (cc4 - cc3);
    *point = Vec2d(m_start.x() * ratio + m_end.x() * (1.0 - ratio),
                    m_start.y() * ratio + m_end.y() * (1.0 - ratio));
    return true;
}

// return distance with perpendicular foot point.
float LineSegment2d::GetPerpendicularFoot(const Vec2d &point, Vec2d *const foot_point) const {
    CHECK_NOTNULL(foot_point);
    if (m_length <= kMathEpsilon) {
        *foot_point = m_start;
        return point.DistanceTo(m_start);
    }
    const float x0 = point.x() - m_start.x();
    const float y0 = point.y() - m_start.y();
    const float proj = x0 * m_unit_direction.x() + y0 * m_unit_direction.y();
    *foot_point = m_start + m_unit_direction * proj;
    return std::abs(x0 * m_unit_direction.y() - y0 * m_unit_direction.x());
    }

std::string LineSegment2d::DebugString() const {
    std::string str = "";
    str = "segment2d ( start = " + m_start.DebugString() + "  end = " + m_end.DebugString() + " )";
    return str;
}

bool LineSegment2d::GetNormalIntersect(const LineSegment2d &other_segment,
                                       const bool clockwise, Vec2d *const point) const {
    LineSegment2d self;
    LineSegment2d other;
    if (clockwise) {
        self = LineSegment2d(self.m_start, self.m_unit_normal_clockwise * scale_ratio);
        other = LineSegment2d(other_segment.m_start, other_segment.m_unit_normal_clockwise * scale_ratio);
    } else {
        self = LineSegment2d(self.m_start, self.m_unit_normal_anticlockwise * scale_ratio);
        other = LineSegment2d(other_segment.m_start, other_segment.m_unit_normal_anticlockwise * scale_ratio);
    }
    return self.GetIntersect(other, point);
}

}