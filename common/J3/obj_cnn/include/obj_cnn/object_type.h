// Copyright 2022 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OBJECT_TYPE_H_
#define INCLUDE_OBJ_CNN_OBJECT_TYPE_H_

#include <algorithm>
#include <functional>
#include <string>
#include <vector>

#include "minieye_jsonhpp/json.hpp"
#include "obj_cnn/debug.h"
#include "opencv2/core/types.hpp"

namespace obj_cnn {

// 目标检测分类
enum class ObjectType {
  kNone = 0,
  kVehicle = 1,
  kPed = 2,
  kBike = 3,
  kCone = 4,

  kVehicleWheel = 5,
  kVehiclePlate = 6,
  kPedHead = 7,

  kSmallTrafficSign = 10,
  kBigTrafficSign = 11,
  kTrafficLight = 12,
  kAnimals = 13,
};

JCNN_DECLARE_DEBUG_STRING_TRAIT(ObjectType);

// 车辆姿态分类 perception::Object::VehiclePose
enum class VehiclePoseType {
  kNegative = 0,      // 背景
  kLeftTail = 1,      // 左侧同向车
  kMidTail = 2,       // 中间同向车
  kRightTail = 3,     // 右侧同向车
  kLeftHead = 4,      // 左侧对向车
  kMidHead = 5,       // 中间对向车
  kRightHead = 6,     // 右侧对向车
  kLeftSide = 7,      // 朝左横向车
  kRightSide = 8,     // 朝右横向车
  kLeftCutIn = 9,     // 左侧切入
  kRightCutIn = 10,   // 右侧切入
  kLeftCutOut = 11,   // 左侧切出
  kRightCutOut = 12,  // 右侧切出
  kOccluded = 13,  // 遮挡姿态：车头遮挡、车位遮挡、横向遮挡
  kSide = 14,
  kUnknown = 20,  // 位置姿态
};

JCNN_DECLARE_DEBUG_STRING_TRAIT(VehiclePoseType);

/// Occluded judgement from network.
///
/// This is intend to generate a bit-mark of the three occluded judgements.
///
/// If any contradiction, such as a car is kShelter and not kShlterLeft, network
/// output is not reliable.
///
/// Mirror of perception::Object::OccludedType.
enum class VehicleOccluded {
  kOccluded = 1,
  kOccludedLeft = 2,
  kOccludedRight = 4,
};

/// 车型分类
enum class VehicleType {
  kNegative = 0,        // 背景
  kBus = 1,             // 大巴
  kCar = 2,             // 小轿车,suv
  kMiniBus = 3,         // 面包车
  kBucketTruck = 4,     // 斗卡
  kContainerTruck = 5,  // 箱卡
  kTricycle = 6,        // 三轮车
  kTanker = 7,  // 油罐车,晒水车（车身带有圆形,椭圆形,半圆形的罐）
  kCementTankTruck = 8,  // 水泥罐车
  kPickup = 9,           // 皮卡
  kSedimentTruck = 10,   // 渣土车
  kIveco = 11,           // 依维柯
  kSpecialCar = 12,      // 异型车
  kCityAuto = 13,        // 市政车
  kUnknown = 14,         // 未知车辆

  kWrecker = 15,          // 小型拖车，清障车
  kFlatTransporter = 16,  // 大型平板拖车
  kTractorHead = 17,      // 卡车车头,牵引车车头
  kSpecialFlatTransporter = 18,  // 大型特种拖车（相对于大型平板拖车会多挡板）
  kCarCarrier = 19,  // 轿运车

  // 盖布车（车身上的货物全部被布盖着，同时对车辆尾部进行了遮挡）
  kTruckWithTrap = 20,
  kEngineeringVehicle = 21,  // 工程车
  kSweeper = 22,             // 扫地车
  kRoadServiceCar = 23,      // 道路维修车
  kSanitationTruck = 24,     // 环卫车
  kGarbageTruck = 25,        // 垃圾车

  // Car/MiniBus/Pickup/Iveco
  //
  // This is only a marker for parse process, result of operator will not
  // contains this type.
  kSmallCarLike = 200,

  // BucketTruck/ContainerTruck/Tanker/SpecialFlatTransport/SpecialCar/
  //
  // This is only a marker for parse process, result of operator will not
  // contains this type.
  kTrunkLike = 201,
};

JCNN_DECLARE_DEBUG_STRING_TRAIT(VehicleType);

// 红绿灯状态
enum class TrafficLightStatus {
  kTLSUnknown = 0,
  kTLSRed = 1,     // 红灯
  kTLSGreen = 2,   // 绿灯
  kTLSYellow = 3,  // 黄灯
};

JCNN_DECLARE_DEBUG_STRING_TRAIT(TrafficLightStatus);

struct Monocular3D {
  Monocular3D() {}

  Monocular3D(cv::Point2f ctr_pt_2d_, cv::Point3f box3d_, float z3d_, float alpha_)
      : ctr_pt_2d(ctr_pt_2d_), box3d(box3d_), z3d(z3d_), alpha(alpha_) {}

  cv::Point2f ctr_pt_2d;
  cv::Point3f box3d;
  float z3d;
  float alpha;
};

// One (untyped) detection hit.
struct ObjectDetection {
  ObjectDetection() {}

  ObjectDetection(Monocular3D monocular3d_, cv::Rect2f position_, float confidence_)
      : monocular3d(monocular3d_), position(position_), confidence(confidence_) {}

  Monocular3D monocular3d;
  cv::Rect2f position;
  float confidence = 0.0f;

  minieye_jsonhpp::json ToJsonValue() const {
    return minieye_jsonhpp::json{{"x", position.x},
                                 {"y", position.y},
                                 {"width", position.width},
                                 {"height", position.height},
                                 {"confidence", confidence},
                                 {"x3d", monocular3d.ctr_pt_2d.x},
                                 {"y3d", monocular3d.ctr_pt_2d.y},
                                 {"z3d", monocular3d.z3d},
                                 {"l3d", monocular3d.box3d.x},
                                 {"w3d", monocular3d.box3d.y},
                                 {"h3d", monocular3d.box3d.z},
                                 {"alpha", monocular3d.alpha}};
  }

  void FromJson(const minieye_jsonhpp::json &object) {
    position.x = FindOr(object, "x", 0);
    position.y = FindOr(object, "y", 0);
    position.width = FindOr(object, "width", 0);
    position.height = FindOr(object, "height", 0);
    confidence = FindOr(object, "confidence", 0);
    monocular3d.ctr_pt_2d.x = FindOr(object, "x3d", 0);
    monocular3d.ctr_pt_2d.y = FindOr(object, "y3d", 0);
    monocular3d.z3d = FindOr(object, "z3d", 0);
    monocular3d.box3d.x = FindOr(object, "l3d", 0);
    monocular3d.box3d.y = FindOr(object, "w3d", 0);
    monocular3d.box3d.z = FindOr(object, "h3d", 0);
    monocular3d.alpha = FindOr(object, "alpha", 0);
  }

  template <typename T>
  T FindOr(const minieye_jsonhpp::json &object, std::string const &name,
           T const &or_else) {
    if (object.contains(name)) return object[name];
    return or_else;
  }
};

// A group of ObjectDetection with same hit type.
struct ObjectDetectionTypedSet {
  std::vector<ObjectDetection> detections;
  int hit_type = 0;

  minieye_jsonhpp::json ToJsonValue() const {
    std::vector<minieye_jsonhpp::json> json_detections(detections.size());

    std::transform(
        detections.begin(), detections.end(), json_detections.begin(),
        [](const ObjectDetection &obj) { return obj.ToJsonValue(); });

    return minieye_jsonhpp::json{{"hitType", hit_type},
                                 {"detections", json_detections}};
  }

  void FromJson(const minieye_jsonhpp::json &object) {
    hit_type = object["hitType"];

    const auto &json_detections = object["detections"];

    detections.resize(json_detections.size());

    std::transform(json_detections.begin(), json_detections.end(),
                   detections.begin(), [](const minieye_jsonhpp::json &object) {
                     ObjectDetection det;
                     det.FromJson(object);
                     return det;
                   });
  }
};

struct VehicleAttribute {
  VehiclePoseType pose = VehiclePoseType::kNegative;
  float pose_confidence = 0;
  VehicleType type = VehicleType::kNegative;
  float type_confidence = 0;
  cv::Rect regression_rect;
  std::vector<cv::Point2f> regression_points;
  std::vector<float> match_feature;
  std::vector<float> trace_feature;

  /// Bitmask of types in VehicleOccluded.
  int occluded_mask = 0;

  /// If network can not give a good type for vehicle, this is the list of
  /// possible types. Or this always only contains `VehicleAttribute.type`.
  std::vector<VehicleType> possible_types;
};

enum class PedCls {
  kBg = 0,        // 背景
  kFull = 1,      // 完整行人
  kOccluded = 2,  // 遮挡行人
  kCyclist = 3,   // 骑车行人
};

enum class PedPose {
  kUnknow = 0,    // 未知
  kForward = 1,   // 前
  kBackward = 2,  // 后
  kLeft = 3,      // 左
  kRight = 4,     // 右
};

struct PedAttribute {
  PedCls cls = PedCls::kBg;
  PedPose pose = PedPose::kUnknow;
  std::vector<float> feature;
  cv::Rect2f regression_rect;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OBJECT_TYPE_H_
