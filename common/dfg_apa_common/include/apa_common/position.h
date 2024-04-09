// Copyright [2019] - MiniEye INC.
#pragma once
#include <cmath>
#include <string>
#include <vector>
#include "gflags/gflags.h"
#include "vec2d.h"
namespace apa {

struct Position {
/**
 * @class Position
 * 
 * @brief difine the position point of spot coordinate.
 */

 public:
    /**
     * @brief default constructor which take x,y,theta
     * @param x is in the spot coordinate
     * @param y is in the spot coordinate
     * @param theta angle(rad) of vector xy in the spot coordinate 
     */

    Position(const float x, const float y, const float theta) : m_x(x), m_y(y), m_theta(theta) {}

    /**
     * @brief return the zero Position.
     */
    Position() : Position(0.f, 0.f, 0.f) {}

    //! Getter for x component.
    float x() const { return m_x; }

    //! Getter for y component.
    float y() const { return m_y; }

    //! Getter for theta component.
    float theta() const { return m_theta; }

    //! Setter for x component.
    void set_x(const float x) { m_x = x; }

    //! Setter for y component.
    void set_y(const float y) { m_y = y; }

    //! Setter for theta component.
    void set_theta(const float theta) { m_theta = theta; }

    void Reset();

    Position &operator=(const Position &other) {
        this->m_x = other.m_x;
        this->m_y = other.m_y;
        this->m_theta = other.m_theta;
        return *this;
    }

    /**
     * @brief calculate current four outline vector of vehicle box.
     * @param rear_suspension_length the rear suspension length of vehicle.
     * @param vehicle_length the length of vehicle.
     * @param vehicle_width the width of vehicle.
     * @return computed position vector, as fl(a), fr(b), rr(c), rl(d)
     *  <pre>
     * 
     *    front of car
     * A +-----------+ B
     *   |           |  
     *   /           /  turn with radius
     *   |           |
     *   |           | 
     *   |           | 
     *   |           | 
     *   |     X     |                             O
     *   |<--->.<----|----------------------------->* (trun center)   
     *   |           | 
     *   |           |
     * D +-----------+ C
     *    back of car
     * left <-      -> right
     * 
     *  <pre>
     */
    std::vector<Vec2d> GetVehicleBox(const float &rear_suspension_length,
                                     const float &vehicle_length, const float &vehicle_width);

    // /**
    //  * @brief transfer to the spot coordinate by given three vec2d.
    //  * @param o origin vector
    //  * @param x vector co-line to x-axle
    //  * @param y vector co-line to x-axle
    //  * @return computed x,y,theta in spot coordinate
    //  */
    // void Transfer(const Vec2d &o, const Vec2d &x, const Vec2d &y) {}

    //! Return a human-readable string representing this object.

    float DistanceTo(const Position &other) {
        Vec2d a(m_x, m_y);
        Vec2d b(other.x(), other.y());
        return a.DistanceTo(b);
    }
    std::string DebugString() const;

 protected:
    float m_x;
    float m_y;
    float m_theta;
};

}  // namespace apa
