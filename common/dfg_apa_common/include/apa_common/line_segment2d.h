// Copyright [2019] - MiniEye INC.
#pragma once
#include <string>
#include "vec2d.h"
namespace apa {

/**
 * @class LineSegment2d
 * @brief Line segment in 2-D
 */

class LineSegment2d {
 public:
    /**
     * @brief Empty constructor.
     */    
    LineSegment2d();

    /**
     * @brief Constructor with start point and end point.
     * @param start the start point of the line segment
     * @param end the end point of the line segment
     */
    LineSegment2d(const Vec2d &start, const Vec2d &end);

    /**
     * @brief Get the start point.
     * @return The start point of the line segment.
     */
    const Vec2d &start() const { return m_start; }

    /**
     * @brief Get the end point.
     * @return The end point of the line segment.
     */
    const Vec2d &end() const { return m_end; }

    /**
     * @brief Get the uint direction from start point to end point.
     * @return The uint direction of the line segment.
     */
    const Vec2d &unit_direction() const { return m_unit_direction; }

    /**
     * @brief Get the uint normal vector clockwise from start point to end point.
     * @return The uint normal vector clockwise of the line segment.
     */
    const Vec2d &unit_normal_clockwise() const { return m_unit_normal_clockwise; }

    /**
     * @brief Get the uint normal vector anticlockwise from start point to end point.
     * @return The uint normal vector anticlockwise of the line segment.
     */
    const Vec2d &unit_normal_anticlockwise() const { return m_unit_normal_anticlockwise; }

    /**
     * @brief Get the center of the line segment.
     * @return The center of the line segment.
     */
    Vec2d center() const { return (m_start + m_end) / 2.0f; }

    /**
     * @brief Get a new line-segment with the same strat point, but rotated counter-clock-wise by the given angle.
     * @return The ratated line-segment's end point.
     */
    Vec2d rotate(const float angle);

    /**
     * @brief Get the heading of the line segment.
     * @return The heading, witch is the angle between unit direction and x-axis.
     */
    float heading() const { return m_heading; }

    /**
     * @brief Get the cosine of the heading.
     * @return The cosine of the heading.
     */
    float cos_heading() const { return m_unit_direction.x(); }

    /**
     * @brief Get the sine of the heading.
     * @return The sine of the heading.
     */
    float sin_heading() const { return m_unit_direction.y(); }

    /**
     * @brief Get the length of the line segment.
     * @return The length of the line segment.
     */
    float length() const { return m_length; }

    /**
     * @brief Get the length_sqr of the line segment.
     * @return The length_sqr of the line segment.
     */
    float length_sqr() const { return m_length * m_length; }

    /**
     * @brief Compute the shortest distance from a point on the line segment
     *        to a point in 2-D.
     * @param point The point to compute the distance to.
     * @return The shortest distance from points on the line segment to point.
     */
    float DistanceTo(const Vec2d &point) const;

    /**
     * @brief Compute the shortest distance from a point on the line segment
     *        to a point in 2-D, and get the nearest point on the line segment.
     * @param point The point to compute the distance to.
     * @param nearest_pt The nearest point on the line segment
     *        to the input point.
     * @return The shortest distance from points on the line segment
     *         to the input point.
     */
    float DistanceTo(const Vec2d &point, Vec2d *const nearest_pt) const;

    /**
     * @brief Compute the square of the shortest distance from a point
     *        on the line segment to a point in 2-D.
     * @param point The point to compute the squared of the distance to.
     * @return The square of the shortest distance from points
     *         on the line segment to the input point.
     */
    float DistanceSquareTo(const Vec2d &point) const;

    /**
     * @brief Compute the square of the shortest distance from a point
     *        on the line segment to a point in 2-D,
     *        and get the nearest point on the line segment.
     * @param point The point to compute the squared of the distance to.
     * @param nearest_pt The nearest point on the line segment
     *        to the input point.
     * @return The shortest distance from points on the line segment
     *         to the input point.
     */
    float DistanceSquareTo(const Vec2d &point, Vec2d *const nearest_pt) const;

    /**
     * @brief Check if a point is within the line segment.
     * @param point The point to check if it is within the line segment.
     * @return Whether the input point is within the line segment or not.
     */
    bool IsPointIn(const Vec2d &point) const;

    /**
     * @brief Check if the line segment has an intersect
     *        with another line segment in 2-D.
     * @param other_segment The line segment to check if it has an intersect.
     * @return Whether the line segment has an intersect
     *         with the input other_segment.
     */
    bool HasIntersect(const LineSegment2d &other_segment) const;

    /**
     * @brief Compute the intersect with another line segment in 2-D if any.
     * @param other_segment The line segment to compute the intersect.
     * @param point the computed intersect between the line segment and
     *        the input other_segment.
     * @return Whether the line segment has an intersect
     *         with the input other_segment.
     */
    bool GetIntersect(const LineSegment2d &other_segment, Vec2d *const point) const;

    /**
     * @brief Compute the projection of a vector onto the line segment.
     * @param point The end of the vector (starting from the start point of the
     *        line segment) to compute the projection onto the line segment.
     * @return The projection of the vector, which is from the start point of
     *         the line segment to the input point, onto the unit direction.
     */

    float ProjectOntoUnit(const Vec2d &point) const;

    /**
     * @brief Compute the cross product of a vector onto the line segment.
     * @param point The end of the vector (starting from the start point of the
     *        line segment) to compute the cross product onto the line segment.
     * @return The cross product of the unit direction and
     *         the vector, which is from the start point of
     *         the line segment to the input point.
     */
    float ProductOntoUnit(const Vec2d &point) const;

    /**
     * @brief Compute perpendicular foot of a point in 2-D on the straight line
     *        expanded from the line segment.
     * @param point The point to compute the perpendicular foot from.
     * @param foot_point The computed perpendicular foot from the input point to
     *        the straight line expanded from the line segment.
     * @return The distance from the input point to the perpendicular foot.
     */
    float GetPerpendicularFoot(const Vec2d &point, Vec2d *const foot_point) const;

    /**
     * @brief Get the debug string including the essential information.
     * @return Information of the line segment for debugging.
     */
    std::string DebugString() const;

    /**
     * @brief Compute the normal-line the intersect with another line segment's normal-line in 2-D if any.
     * @param other_segment The line segment to compute the intersect.
     * @param clockwise whether clockwise or anti_clockwise, true is clockwise.
     * @param point the computed intersect between the line segment and
     *        the input other_segment.
     * @return Whether the line segment has an intersect
     *         with the input other_segment.
     */
    bool GetNormalIntersect(const LineSegment2d &other_segment, const bool clockwise, Vec2d *const point) const;

 private:
    Vec2d m_start;
    Vec2d m_end;
    Vec2d m_unit_direction;
    Vec2d m_unit_normal_clockwise;
    Vec2d m_unit_normal_anticlockwise;
    float m_heading;
    float m_length;
};

}  // namespace apa
