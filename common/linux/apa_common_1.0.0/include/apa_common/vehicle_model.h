// Copyright [2019] - MiniEye INC.
#pragma once
#include "apa_interface.h"
#include "position.h"

namespace apa {

class VehicleModel {
 public:

    VehicleModel() = default;

    explicit VehicleModel(const VehicleParameters& veh_para);

    Position Predict(uint8_t mode, const TrackReqAuxInfo& aux_info,
                     const float predicted_time_horizion, const Position& cur_vehicle_pose);
    float WheelPulseAddUp(const TrackReqAuxInfo& aux_info);

 private:
    void RearCentereKinematicBicycleModel(const TrackReqAuxInfo& aux_info,
                                        const float predicted_time_horizion,
                                        const Position& cur_vehicle_pose,
                                        Position* predicted_vehicle_pose);
    void BicycleModel(const TrackReqAuxInfo& aux_info, const float predicted_time_horizion,
                      const Position& cur_vehicle_pose, Position* predicted_vehicle_pose);
    void Geometry(const TrackReqAuxInfo& aux_info, const float predicted_time_horizion,
                  const Position& cur_vehicle_pose, Position* predicted_vehicle_pose);
    void Robot(const TrackReqAuxInfo& aux_info, const float predicted_time_horizion,
               const Position& cur_vehicle_pose, Position* predicted_vehicle_pose);
    void ImproveModel(const TrackReqAuxInfo& aux_info, const float predicted_time_horizion,
                      const Position& cur_vehicle_pose, Position* predicted_vehicle_pose);
    void TestModel(const TrackReqAuxInfo& aux_info, const float predicted_time_horizion,
                      const Position& cur_vehicle_pose, Position* predicted_vehicle_pose);
    void ImuYawModel(const TrackReqAuxInfo& aux_info,const float predicted_time_horizion,
                      const Position& cur_vehicle_pose, Position* predicted_vehicle_pose);
    bool m_is_init;
    bool m_WheelCnt_first_hit;
    VehicleParameters m_veh_para;
    TrackReqAuxInfo m_last_auxinfo;
};

}  // namespace apa