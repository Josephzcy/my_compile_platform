// Copyright [2019] - MiniEye INC.
#include <cmath>
#include "glog/logging.h"
#include "vehicle_model.h"

namespace apa {

constexpr float kmph2mps = 1/3.6;
constexpr float mps2kmph = 3.6;
constexpr float step_dt = 0.05;  // s
constexpr float kepsilon = 1e-8;
constexpr uint16_t max_counter = 1000;

VehicleModel::VehicleModel(const VehicleParameters& veh_para) {
    m_veh_para = veh_para;
    m_is_init = true;
    m_WheelCnt_first_hit = false;
}

void VehicleModel::RearCentereKinematicBicycleModel(
        const TrackReqAuxInfo& aux_info,
        const float predicted_time_horizion,
        const Position& cur_vehicle_pose,
        Position* predicted_vehicle_pose) {
    CHECK(m_is_init);
    CHECK_GT(predicted_time_horizion, 0);
    float dt = step_dt;
    float cur_x = cur_vehicle_pose.x();
    float cur_y = cur_vehicle_pose.y();
    float cur_theta = cur_vehicle_pose.theta();
    float cur_v = aux_info.speed_kmph * kmph2mps;
    float cur_a = aux_info.longitudinal_acceleration;
    float next_x = cur_x;
    float next_y = cur_y;
    float next_theta = cur_theta;
    float next_v = cur_v;
    float cur_kappa = 0;
    if (std::fabs(cur_v) > kepsilon) {
        cur_kappa = aux_info.yaw_rate / cur_v;
    }
    if (dt >= predicted_time_horizion) {
        dt = predicted_time_horizion;
    }
    float countdown_time = predicted_time_horizion;
    bool finish_flag = false;
    while (countdown_time > kepsilon && !finish_flag && std::fabs(cur_v) > kepsilon) {
        countdown_time -= dt;
        if (countdown_time < kepsilon) {
            dt = countdown_time + dt;
            finish_flag = true;
        }
        float ds = dt * (cur_v + 0.5 * dt * cur_a);
        float cur_beta = 0.5 * cur_v * dt * cur_kappa;
        next_theta = cur_theta + ds * cur_kappa;
        next_x = cur_x + ds * std::cos(cur_beta + cur_theta);
        next_y = cur_y + ds * std::sin(cur_beta + cur_theta);
        next_v = cur_v + cur_a * dt;
        //  itertation
        cur_x = next_x;
        cur_y = next_y;
        cur_theta = next_theta;
        cur_v = next_v;
        VLOG(2) << "dt(" << dt << "), cur_v(" << cur_v << ", cur_a(" << cur_a << "), cur_kappa(" << cur_kappa
                   << "), cur_beta(" << cur_beta << "), cur_theta(" << cur_theta << "), ds(" << ds << "), next_x("
                   << next_x << "), next_y(" << next_y << "), next_theta(" << next_theta << "), next_v(" << next_v << ")";
    }
    predicted_vehicle_pose->set_x(cur_x);
    predicted_vehicle_pose->set_y(cur_y);
    predicted_vehicle_pose->set_theta(cur_theta);
}

void VehicleModel::BicycleModel(
        const TrackReqAuxInfo& aux_info,
        const float predicted_time_horizion,
        const Position& cur_vehicle_pose,
        Position* predicted_vehicle_pose) {
    CHECK(m_is_init);
    CHECK_GT(predicted_time_horizion, 0);
    float dt = step_dt;
    float cur_x = cur_vehicle_pose.x();
    float cur_y = cur_vehicle_pose.y();
    float cur_theta = cur_vehicle_pose.theta();
    float cur_v = aux_info.speed_kmph * kmph2mps;
    float front_wheel_angle = -1 * (aux_info.steer_deg / m_veh_para.steer_ratio) * D2R;
    float cur_kappa = std::tan(front_wheel_angle) / m_veh_para.wheel_base;
    float cur_yaw_rate = cur_v * cur_kappa;
    float next_x = cur_x;
    float next_y = cur_y;
    float next_theta = cur_theta;
    float next_v = cur_v;
    if (dt >= predicted_time_horizion) {
        dt = predicted_time_horizion;
    }
    float countdown_time = predicted_time_horizion;
    bool finish_flag = false;
    while (countdown_time > kepsilon && !finish_flag && std::fabs(cur_v) > kepsilon) {
        countdown_time -= dt;
        if (countdown_time < kepsilon) {
            dt = countdown_time + dt;
            finish_flag = true;
        }
        float ds = dt * cur_v;
        next_theta = cur_theta + cur_yaw_rate * dt;
        float cur_beta = 0.5 * cur_v * dt * cur_kappa;
        next_x = cur_x + ds * std::cos(cur_beta + cur_theta);
        next_y = cur_y + ds * std::sin(cur_beta + cur_theta);
        //  itertation
        cur_x = next_x;
        cur_y = next_y;
        cur_theta = next_theta;
        cur_v = next_v;
        VLOG(2) << "dt(" << dt << "), cur_v(" << cur_v << ", cur_theta(" << cur_theta << "), cur_kappa(" << cur_kappa
                   << "), cur_beta(" << cur_beta << "), cur_yaw_rate(" << cur_yaw_rate << "), ds(" << ds << "), next_x("
                   << next_x << "), next_y(" << next_y << "), next_theta(" << next_theta << "), next_v(" << next_v << ")";
    }
    predicted_vehicle_pose->set_x(cur_x);
    predicted_vehicle_pose->set_y(cur_y);
    predicted_vehicle_pose->set_theta(cur_theta);
}

void VehicleModel::Geometry(
        const TrackReqAuxInfo& aux_info,
        const float predicted_time_horizion,
        const Position& cur_vehicle_pose,
        Position* predicted_vehicle_pose) {
    CHECK(m_is_init);
    CHECK_GT(predicted_time_horizion, 0);
    float dt = step_dt;
    float cur_x = cur_vehicle_pose.x();
    float cur_y = cur_vehicle_pose.y();
    float cur_theta = cur_vehicle_pose.theta();
    float cur_v = aux_info.speed_kmph * kmph2mps;
    float front_wheel_angle = -1 * (aux_info.steer_deg / m_veh_para.steer_ratio) * D2R;
    float cur_kappa = std::tan(front_wheel_angle) / m_veh_para.wheel_base;
    float cur_yaw_rate = cur_v * cur_kappa;
    float next_x = cur_x;
    float next_y = cur_y;
    float next_theta = cur_theta;
    float next_v = cur_v;
    if (dt >= predicted_time_horizion) {
        dt = predicted_time_horizion;
    }
    float countdown_time = predicted_time_horizion;
    bool finish_flag = false;
    while (countdown_time > kepsilon && !finish_flag && std::fabs(cur_v) > kepsilon
            && std::fabs(cur_kappa) > kepsilon) {
        countdown_time -= dt;
        if (countdown_time < kepsilon) {
            dt = countdown_time + dt;
            finish_flag = true;
        }
        float ds = 2 * std::sin(0.5 * cur_yaw_rate * dt) / cur_kappa;  //  ds
        next_theta = cur_theta + cur_yaw_rate * dt;
        float cur_beta = 0.5 * cur_v * dt * cur_kappa;
        next_x = cur_x + ds * std::cos(cur_beta + cur_theta);
        next_y = cur_y + ds * std::sin(cur_beta + cur_theta);
        //  itertation
        cur_x = next_x;
        cur_y = next_y;
        cur_theta = next_theta;
        cur_v = next_v;
        VLOG(2) << "dt(" << dt << "), cur_v(" << cur_v << ", cur_theta(" << cur_theta << "), cur_kappa(" << cur_kappa
                   << "), cur_beta(" << cur_beta << "), cur_yaw_rate(" << cur_yaw_rate << "), ds(" << ds << "), next_x("
                   << next_x << "), next_y(" << next_y << "), next_theta(" << next_theta << "), next_v(" << next_v << ")";
    }
    predicted_vehicle_pose->set_x(cur_x);
    predicted_vehicle_pose->set_y(cur_y);
    predicted_vehicle_pose->set_theta(cur_theta);
}

void VehicleModel::Robot(
        const TrackReqAuxInfo& aux_info,
        const float predicted_time_horizion,
        const Position& cur_vehicle_pose,
        Position* predicted_vehicle_pose) {
    CHECK(m_is_init);
    CHECK_GT(predicted_time_horizion, 0);
    float dt = step_dt;
    float cur_x = cur_vehicle_pose.x();
    float cur_y = cur_vehicle_pose.y();
    float cur_theta = cur_vehicle_pose.theta();
    float cur_v = 0.5 * (aux_info.wheel_spd_rl + aux_info.wheel_spd_rr)* kmph2mps;
    float cur_yaw_rate = -0.5 * (aux_info.wheel_spd_rr - aux_info.wheel_spd_rl)* kmph2mps
            / m_veh_para.rear_wheel_distance;
    float cur_kappa = 0;
    if (std::fabs(cur_v) > kepsilon) {
        cur_kappa = cur_yaw_rate / cur_v;
    }
    float next_x = cur_x;
    float next_y = cur_y;
    float next_theta = cur_theta;
    float next_v = cur_v;
    if (dt >= predicted_time_horizion) {
        dt = predicted_time_horizion;
    }
    float countdown_time = predicted_time_horizion;
    bool finish_flag = false;
    while (countdown_time > kepsilon && !finish_flag && std::fabs(cur_v) > kepsilon) {
        countdown_time -= dt;
        if (countdown_time < kepsilon) {
            dt = countdown_time + dt;
            finish_flag = true;
        }
        float ds = dt * cur_v;  //  ds
        next_theta = cur_theta + cur_yaw_rate * dt;
        float cur_beta = 0.5 * cur_v * dt * cur_kappa;
        next_x = cur_x + ds * std::cos(cur_beta + cur_theta);
        next_y = cur_y + ds * std::sin(cur_beta + cur_theta);
        //  itertation
        cur_x = next_x;
        cur_y = next_y;
        cur_theta = next_theta;
        cur_v = next_v;
        VLOG(2) << "dt(" << dt << "), cur_v(" << cur_v << ", cur_theta(" << cur_theta << "), cur_kappa(" << cur_kappa
                   << "), cur_beta(" << cur_beta << "), cur_yaw_rate(" << cur_yaw_rate << "), ds(" << ds << "), next_x("
                   << next_x << "), next_y(" << next_y << "), next_theta(" << next_theta << "), next_v(" << next_v << ")";
    }
    predicted_vehicle_pose->set_x(cur_x);
    predicted_vehicle_pose->set_y(cur_y);
    predicted_vehicle_pose->set_theta(cur_theta);
}

void VehicleModel::ImproveModel(const TrackReqAuxInfo& aux_info, const float predicted_time_horizion,
                      const Position& cur_vehicle_pose, Position* predicted_vehicle_pose) {
    CHECK(m_is_init);
    CHECK_GT(predicted_time_horizion, 0);
    float dt = step_dt;
    float cur_x = cur_vehicle_pose.x();
    float cur_y = cur_vehicle_pose.y();
    float cur_theta = cur_vehicle_pose.theta();
    float cur_v = aux_info.speed_kmph * kmph2mps;
    float cur_a = aux_info.longitudinal_acceleration;
    float front_wheel_angle = -1 * (aux_info.steer_deg / m_veh_para.steer_ratio) * D2R;
    float cur_radius = std::fabs(m_veh_para.wheel_base / std::tan(front_wheel_angle))
                        - 0.5 * m_veh_para.rear_wheel_distance;
    float cur_kappa = 1.f / cur_radius;
    float next_x = cur_x;
    float next_y = cur_y;
    float next_theta = cur_theta;
    float next_v = cur_v;
    if (dt >= predicted_time_horizion) {
        dt = predicted_time_horizion;
    }
    float countdown_time = predicted_time_horizion;
    bool finish_flag = false;
    while (countdown_time > kepsilon && !finish_flag) {
        countdown_time -= dt;
        if (countdown_time < kepsilon) {
            dt = countdown_time + dt;
            finish_flag = true;
        }
        int drict_sign = 0;
        if (cur_v >= 0) {
            drict_sign = 1;
        } else {
            drict_sign = -1;
        }
        float d_theta = 0.f;
        float dpsi = 0.f;
        float ds = drict_sign * WheelPulseAddUp(aux_info);
        if (std::fabs(aux_info.steer_deg) >= 10) {
            if (aux_info.steer_deg <= 0.f) {
                dpsi = 1 * ds / cur_radius;
            } else {
                dpsi = -1 * ds / cur_radius;
            }
            d_theta = drict_sign * std::fabs(std::sin(0.5 * dpsi) * 2.f * cur_radius);
        } else {
            d_theta = ds;
            dpsi = 0.f;
        }
        next_theta = cur_theta + dpsi;
        next_x = cur_x + d_theta * std::cos(cur_theta + 0.5 * dpsi);
        next_y = cur_y + d_theta * std::sin(cur_theta + 0.5 * dpsi);
        //  itertation
        cur_x = next_x;
        cur_y = next_y;
        cur_theta = next_theta;
        cur_v = next_v;
        VLOG(2) << "dt(" << dt << "), cur_v(" << cur_v << ", cur_a(" << cur_a << "), cur_kappa(" << cur_kappa
                   << "), cur_theta(" << cur_theta << "), ds(" << ds << "), next_x("
                   << next_x << "), next_y(" << next_y << "), next_theta(" << next_theta << "), next_v(" << next_v << ")";
    }
    predicted_vehicle_pose->set_x(cur_x);
    predicted_vehicle_pose->set_y(cur_y);
    predicted_vehicle_pose->set_theta(cur_theta);
}

void VehicleModel::TestModel(const TrackReqAuxInfo& aux_info, const float predicted_time_horizion,
                      const Position& cur_vehicle_pose, Position* predicted_vehicle_pose) {
    CHECK(m_is_init);
    CHECK_GT(predicted_time_horizion, 0);
    float dt = step_dt;
    float cur_x = cur_vehicle_pose.x();
    float cur_y = cur_vehicle_pose.y();
    float cur_theta = cur_vehicle_pose.theta();
    float cur_v = aux_info.speed_kmph * kmph2mps;
    float cur_a = aux_info.longitudinal_acceleration;
    float next_x = cur_x;
    float next_y = cur_y;
    float next_theta = cur_theta;
    float next_v = cur_v;
    float cur_kappa = 0;
    if (std::fabs(cur_v) > kepsilon) {
        cur_kappa = aux_info.yaw_rate / cur_v;
    }
    if (dt >= predicted_time_horizion) {
        dt = predicted_time_horizion;
    }
    float countdown_time = predicted_time_horizion;
    bool finish_flag = false;
    while (countdown_time > kepsilon && !finish_flag && std::fabs(cur_v) > kepsilon) {
        countdown_time -= dt;
        if (countdown_time < kepsilon) {
            dt = countdown_time + dt;
            finish_flag = true;
        }
        float ds = dt * (cur_v + 0.5 * dt * cur_a);
        float cur_beta = 0.5 * cur_v * dt * cur_kappa;
        next_theta = cur_theta + ds * cur_kappa;
        int drict_sign = 0;
        if (cur_v >= 0) {
            drict_sign = 1;
        } else {
            drict_sign = -1;
        }
        float dist = drict_sign * WheelPulseAddUp(aux_info);
        // next_x = cur_x + ds * std::cos(cur_beta + cur_theta);
        // next_y = cur_y + ds * std::sin(cur_beta + cur_theta);
        next_x = cur_x + dist * std::cos(cur_beta + cur_theta);
        next_y = cur_y + dist * std::sin(cur_beta + cur_theta);
        next_v = cur_v + cur_a * dt;
        //  itertation
        cur_x = next_x;
        cur_y = next_y;
        cur_theta = next_theta;
        cur_v = next_v;
        VLOG(2) << "dt(" << dt << "), cur_v(" << cur_v << ", cur_a(" << cur_a << "), cur_kappa(" << cur_kappa
                   << "), cur_beta(" << cur_beta << "), cur_theta(" << cur_theta << "), ds(" << ds << "), next_x("
                   << next_x << "), next_y(" << next_y << "), next_theta(" << next_theta << "), next_v(" << next_v << ")";
    }
    predicted_vehicle_pose->set_x(cur_x);
    predicted_vehicle_pose->set_y(cur_y);
    predicted_vehicle_pose->set_theta(cur_theta);
}

Position VehicleModel::Predict(uint8_t mode, const TrackReqAuxInfo& aux_info,
                const float predicted_time_horizion, const Position& cur_vehicle_pose) {
    CHECK(m_is_init);
    Position predicted_vehicle_position;
    if (mode == 1) {
        RearCentereKinematicBicycleModel(aux_info, predicted_time_horizion,
                                        cur_vehicle_pose, &predicted_vehicle_position);
    } else if (mode == 2) {
        BicycleModel(aux_info, predicted_time_horizion,
                                        cur_vehicle_pose, &predicted_vehicle_position);
    } else if (mode == 3) {
        Geometry(aux_info, predicted_time_horizion,
                                        cur_vehicle_pose, &predicted_vehicle_position);
    } else if (mode == 4) {
        Robot(aux_info, predicted_time_horizion,
                                        cur_vehicle_pose, &predicted_vehicle_position);
    } else if (mode == 5) {
        ImproveModel(aux_info, predicted_time_horizion,
                                        cur_vehicle_pose, &predicted_vehicle_position);
    } else if (mode == 6) {
        TestModel(aux_info, predicted_time_horizion,
                                        cur_vehicle_pose, &predicted_vehicle_position);
    } else {
        LOG(ERROR) << "pass mode is invalid!";
    }
    return predicted_vehicle_position;
}

float VehicleModel::WheelPulseAddUp(const TrackReqAuxInfo& aux_info) {
    CHECK(m_is_init);
    float res = 0;
    if (!m_WheelCnt_first_hit) {
        m_WheelCnt_first_hit = true;
        m_last_auxinfo = aux_info;
        return res;
    }
    const uint16_t cur_rl_cnt = aux_info.wheel_pulse_counter_rl;
    const uint16_t cur_rr_cnt = aux_info.wheel_pulse_counter_rr;
    const uint16_t last_rl_cnt = m_last_auxinfo.wheel_pulse_counter_rl;
    const uint16_t last_rr_cnt = m_last_auxinfo.wheel_pulse_counter_rr;
    const float ratio = m_veh_para.wheel_rolling_ratio;
    uint16_t rl_cnt_err = 0;
    if (cur_rl_cnt < last_rl_cnt) {
        rl_cnt_err = max_counter + cur_rl_cnt - last_rl_cnt;
    } else {
        rl_cnt_err = cur_rl_cnt - last_rl_cnt;
    }
    if (rl_cnt_err > 100) {
        m_last_auxinfo = aux_info;
        return res;
    }
    float rl_ds = rl_cnt_err * ratio;
    uint16_t rr_cnt_err = 0;
    if (cur_rr_cnt < last_rr_cnt) {
        rr_cnt_err = max_counter + cur_rr_cnt - last_rr_cnt;
    } else {
        rr_cnt_err = cur_rr_cnt - last_rr_cnt;
    }
    if (rr_cnt_err > 100) {
        m_last_auxinfo = aux_info;
        return res;
    }
    float rr_ds = rr_cnt_err * ratio;
    res = 0.5 * (rl_ds + rr_ds);
    m_last_auxinfo = aux_info;
    return res;
}

}  // namespace apa
