// Copyright [2019] - MiniEye INC.

#include "glog/logging.h"
#include "position.h"

namespace apa {

    std::vector<Vec2d> Position::GetVehicleBox(const float &rear_suspension_length,
                                           const float &vehicle_length, const float &vehicle_width) {
        std::vector<Vec2d> res;
        if (rear_suspension_length == 0 || vehicle_length == 0|| vehicle_width == 0) {
            LOG(ERROR) << "The pass parameter is invalid!";
            return res;
        }
        Vec2d c(m_x - rear_suspension_length*cos(m_theta) + vehicle_width*sin(m_theta)/2,
                m_y - rear_suspension_length*sin(m_theta) - vehicle_width*cos(m_theta)/2);

        Vec2d d(m_x - rear_suspension_length*cos(m_theta) - vehicle_width*sin(m_theta)/2,
                m_y - rear_suspension_length*sin(m_theta) + vehicle_width*cos(m_theta)/2);

        Vec2d a(d.x() + vehicle_length*cos(m_theta), d.y() + vehicle_length*sin(m_theta));

        Vec2d b(c.x() + vehicle_length*cos(m_theta), c.y() + vehicle_length*sin(m_theta));
        res.push_back(a);
        res.push_back(b);
        res.push_back(c);
        res.push_back(d);
        VLOG(2) << "a: " << a.DebugString() << "b: " << b.DebugString()
                << "c: " << c.DebugString() << "d: " << d.DebugString();
        return res;
    }

    std::string Position::DebugString() const {
        std::string str;
        str = "Pose ( x = " + std::to_string(m_x) + ", y = " + std::to_string(m_y)
                + ", theta = " + std::to_string(m_theta * 180.0 / M_PI) + ")";
        return str;
    }

    void Position::Reset() {
        m_x = 0;
        m_y = 0;
        m_theta = 0;
    }

}  // namespace apa
