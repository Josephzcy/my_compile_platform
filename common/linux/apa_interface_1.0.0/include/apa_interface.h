// Copyright [2019] - MiniEye INC.
#pragma once
#include <deque>
#include <vector>
#include <string>
#include "msgpack.hpp"
#include "opencv2/opencv.hpp"

// #define M_PI 3.14159265358979
// #define M_PI_2 1.5707963267949

enum class PARK_SIDE {
    INVALID = 0,
    LEFT,
    RIGHT
};

enum class PARKING_MODE
{
    INVALID = 0,
    VERTICAL,
    HORIZON,
    OBLIQUE
};

    // Planning Status, 0|defalt, 1|search spots, 2|waiting control, 3|has planing path, 4|replanning, 5|finish planing
enum class PLANNING_STATUS{
    DEFALT = 0,
    SEARCH_SPOTS,
    WAITING_CONTROL,
    HAS_PLANNING_PATH,
    HANG_ON,
    REPLANNING,
    FINISHED_PLANNING
};

// ------------------------------- aux func
// --------------------------------------
namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
    namespace adaptor {

    // msgpack cv::Rect
    template <>
    struct convert<cv::Rect> {
        msgpack::object const& operator()(msgpack::object const& o,
                                          cv::Rect& v) const {
            if (o.type != msgpack::type::ARRAY) throw msgpack::type_error();
            if (o.via.array.size != 4) throw msgpack::type_error();
            v = cv::Rect(
                o.via.array.ptr[0].as<int>(), o.via.array.ptr[1].as<int>(),
                o.via.array.ptr[2].as<int>(), o.via.array.ptr[3].as<int>());
            return o;
        }
    };

    template <>
    struct pack<cv::Rect> {
        template <typename Stream>
        packer<Stream>& operator()(msgpack::packer<Stream>& o,
                                   cv::Rect const& v) const {
            // packing member variables as an array
            o.pack_array(4);
            o.pack(v.x);
            o.pack(v.y);
            o.pack(v.width);
            o.pack(v.height);
            return o;
        }
    };

    // msgpack cv::Point2f
    template <>
    struct convert<cv::Point2f> {
        msgpack::object const& operator()(msgpack::object const& o,
                                          cv::Point2f& v) const {
            if (o.type != msgpack::type::ARRAY) throw msgpack::type_error();
            if (o.via.array.size != 2) throw msgpack::type_error();
            v = cv::Point2f(o.via.array.ptr[0].as<float>(),
                            o.via.array.ptr[1].as<float>());
            return o;
        }
    };

    template <>
    struct pack<cv::Point2f> {
        template <typename Stream>
        packer<Stream>& operator()(msgpack::packer<Stream>& o,
                                   cv::Point2f const& v) const {
            // packing member variables as an array
            o.pack_array(2);
            o.pack(v.x);
            o.pack(v.y);
            return o;
        }
    };

    // msgpack cv::Point
    template <>
    struct convert<cv::Point> {
        msgpack::object const& operator()(msgpack::object const& o,
                                          cv::Point& v) const {
            if (o.type != msgpack::type::ARRAY) throw msgpack::type_error();
            if (o.via.array.size != 2) throw msgpack::type_error();
            v = cv::Point(o.via.array.ptr[0].as<int>(),
                          o.via.array.ptr[1].as<int>());
            return o;
        }
    };

    template <>
    struct pack<cv::Point> {
        template <typename Stream>
        packer<Stream>& operator()(msgpack::packer<Stream>& o,
                                   cv::Point const& v) const {
            // packing member variables as an array
            o.pack_array(2);
            o.pack(v.x);
            o.pack(v.y);
            return o;
        }
    };

    }  // namespace adaptor
}  // MSGPACK_API_VERSION_NAMESPACE
}  // namespace msgpack

// ----------------------------- aux func
// ----------------------------------------------
struct ScaleWindow {
    float scale_x;
    float scale_y;
    float offset_x;
    float offset_y;
    int16_t width;
    int16_t height;
    MSGPACK_DEFINE_MAP(scale_x, scale_y, offset_x, offset_y, width, height);
};

struct SegMask {
    cv::Mat mask;
    ScaleWindow seg_info;
};

enum CameraLabel {
    kCameraFront = 0,
    kCameraRear = 1,
    kCameraLeft = 2,
    kCameraRight = 3
};

using SegMaskPtr = uint64_t;
// --------------------------------------------------------------------------
/// @brief 障碍物分割结果
// --------------------------------------------------------------------------
struct APACNNObstRes {
    int8_t camera_label;  // 摄像头序号

    // 0 - background, 1 - person, 2 - cone&lock, 3 - guardrail
    SegMaskPtr obstacles;
    // 0 - background, 1 - freespace, 2 - vehicle
    SegMaskPtr freespace;
    APACNNObstRes() : camera_label(-1), obstacles(0) {}

    APACNNObstRes(const APACNNObstRes& lhs) { *this = lhs; }

    const APACNNObstRes& operator=(const APACNNObstRes& lhs) {
        camera_label = lhs.camera_label;
        obstacles = lhs.obstacles;
        freespace = lhs.freespace;
        return *this;
    }
    MSGPACK_DEFINE_MAP(camera_label, obstacles, freespace);
};

typedef std::deque<APACNNObstRes> APACNNObstResList;

// --------------------------------------------------------------------------
/// @brief 车位线分割结果
// --------------------------------------------------------------------------
struct APACNNRes {
    // 0 - background, 1 - parking lines, 2 - corner points
    SegMaskPtr key_species;
    APACNNRes() : key_species(0) {}

    const APACNNRes& operator=(const APACNNRes& lhs) {
        key_species = lhs.key_species;
        return *this;
    }
    MSGPACK_DEFINE_MAP(key_species);
};

struct BirdViewROI {
    float xres;
    float yres;
    float startx;
    float starty;
    float endx;
    float endy;
    float crop_startx;
    float crop_starty;
    float crop_endx;
    float crop_endy;
    MSGPACK_DEFINE_MAP(xres, yres, startx, starty, endx, endy, crop_startx,
                       crop_starty, crop_endx, crop_endy);
};

// --------------------------------------------------------------------------
/// @brief 方向向量
// --------------------------------------------------------------------------
struct DirectVector2D {
    float x;
    float y;
    float len;
    DirectVector2D() : x(0), y(0), len(0) {}

    DirectVector2D(const DirectVector2D& lhs) { *this = lhs; }

    const DirectVector2D& operator=(const DirectVector2D& lhs) {
        x = lhs.x;
        y = lhs.y;
        len = lhs.len;
        return *this;
    }

    const bool operator==(const DirectVector2D& lhs) const {
        return (this->x == lhs.x && this->y == lhs.y && len == lhs.len);
    }

    MSGPACK_DEFINE_MAP(x, y, len);
};

// --------------------------------------------------------------------------
/// @brief 停车位角点
// --------------------------------------------------------------------------
struct ParkingSpaceCornerPt {        // 角点
    uint32_t id;                     // 编号
    cv::Point2f corner_pt;           // 车辆坐标系点坐标，目前为（右前上）
    cv::Point2f img_pt;              // 俯视图坐标系点坐标
    cv::Point2f odom_pt;             // odom坐标系下角点坐标，目前为（右前上）
    std::deque<uint32_t> neighbors;  //相连点ID
    uint32_t group_id;
    uint8_t confidence;
    ParkingSpaceCornerPt() { neighbors.clear(); }

    float DistanceTo(const ParkingSpaceCornerPt &other)
    {
        float dx = other.corner_pt.x - this->corner_pt.x;
        float dy = other.corner_pt.y - this->corner_pt.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    const bool operator==(const ParkingSpaceCornerPt& lhs) const {
        return (this->corner_pt == lhs.corner_pt);
    }

    MSGPACK_DEFINE_MAP(id, corner_pt, img_pt, neighbors, group_id, confidence);
};

typedef std::deque<ParkingSpaceCornerPt> CornerPtList;

// --------------------------------------------------------------------------
/// @brief 停车位样式信息
// --------------------------------------------------------------------------
struct ParkingSpaceSpecs {
    float angle;
    float width;
    float height;
    MSGPACK_DEFINE_MAP(angle, width, height);
};

// --------------------------------------------------------------------------
/// @brief 停车位分块区域
// --------------------------------------------------------------------------
struct ParkingSpaceGroup {  // 分块
    uint32_t id;            // 编号
    CornerPtList ps_corner_pts;
    ParkingSpaceSpecs pattern;
    ParkingSpaceGroup() : id(0) { ps_corner_pts.clear(); }

    ParkingSpaceGroup(const ParkingSpaceGroup& lhs) { *this = lhs; }

    const ParkingSpaceGroup& operator=(const ParkingSpaceGroup& lhs) {
        id = lhs.id;
        ps_corner_pts = lhs.ps_corner_pts;
        pattern = lhs.pattern;
        return *this;
    }

    MSGPACK_DEFINE_MAP(id, ps_corner_pts, pattern);
};

typedef std::deque<ParkingSpaceGroup> SlotGroupList;

// --------------------------------------------------------------------------
/// @brief 跟踪模块的运动输入
// shift mode:
// 0x0 : 无效invalid
// 0x1 : P  泊车档
// 0x2 : R  倒车档
// 0x3 : N  空挡
// 0x4 : D  前进档
// 0x5 : M  燃油特有
// 0x6 : S  燃油特有    前进挡
// 0x8-0xF : reserved  倒档
// 修正：（新）kGearMode 档位模式 0 invalid, 1 Neutral, 2 Driving, 3 Reverse, 4 Parking，无效值：-1000；
// --------------------------------------------------------------------------
struct TrackReqAuxInfo {
    uint64_t tm_ms;      // 运动信息时戳
    int shift_mode;      // 档位信息
    float speed_kmph;    // 速度 km/h
    float steer_deg;     // 方向盘转角 deg
    float wheel_spd_fl;  // 左前轮线速度 km/h
    float wheel_spd_fr;  // 右前轮线速度 km/h
    float wheel_spd_rl;  // 左后轮线速度 km/h
    float wheel_spd_rr;  // 右后轮线速度 km/h
    /*
    ds_cal_mode: use to select vehicle model.
        1|RearCentereKinematicBicycleModel
        2|BicycleModel
        3|Geometry
        4|Robot
        5|ImproveModel
        other is all invalid.
    */
    uint8_t ds_cal_mode;
    uint16_t wheel_pulse_counter_fl;  // 左前轮轮速脉冲计数
    uint16_t wheel_pulse_counter_fr;  // 右前轮轮速脉冲计数
    uint16_t wheel_pulse_counter_rl;  // 左后轮轮速脉冲计数
    uint16_t wheel_pulse_counter_rr;  // 右后轮轮速脉冲计数
    float longitudinal_acceleration;  //  纵向加速度  m/s2
    float lateral_acceleration;  //  横向加速度  m/s2
    float yaw_rate;  //  横摆角速度  deg/s
    float driver_torque;  //  驾驶员施加扭矩  Nm

    TrackReqAuxInfo()
        : tm_ms(0),
          shift_mode(0),
          speed_kmph(0),
          steer_deg(0),
          wheel_spd_fl(0),
          wheel_spd_fr(0),
          wheel_spd_rl(0),
          wheel_spd_rr(0),
          ds_cal_mode(0),
          wheel_pulse_counter_fl(0),
          wheel_pulse_counter_fr(0),
          wheel_pulse_counter_rl(0),
          wheel_pulse_counter_rr(0),
          longitudinal_acceleration(0),
          lateral_acceleration(0),
          yaw_rate(0),
          driver_torque(0) {}

    TrackReqAuxInfo(const TrackReqAuxInfo& lhs) { *this = lhs; }

    const TrackReqAuxInfo& operator=(const TrackReqAuxInfo& lhs) {
        tm_ms = lhs.tm_ms;
        shift_mode = lhs.shift_mode;
        speed_kmph = lhs.speed_kmph;
        steer_deg = lhs.steer_deg;
        wheel_spd_fl = lhs.wheel_spd_fl;
        wheel_spd_fr = lhs.wheel_spd_fr;
        wheel_spd_rl = lhs.wheel_spd_rl;
        wheel_spd_rr = lhs.wheel_spd_rr;
        ds_cal_mode = lhs.ds_cal_mode;
        wheel_pulse_counter_fl = lhs.wheel_pulse_counter_fl;
        wheel_pulse_counter_fr = lhs.wheel_pulse_counter_fr;
        wheel_pulse_counter_rl = lhs.wheel_pulse_counter_rl;
        wheel_pulse_counter_rr = lhs.wheel_pulse_counter_rr;
        longitudinal_acceleration = lhs.longitudinal_acceleration;
        lateral_acceleration = lhs.lateral_acceleration;
        yaw_rate = lhs.yaw_rate;
        driver_torque = lhs.driver_torque;
        return *this;
    }

    const std::string ToString() {
        std::string ret;
        ret = "tm_ms = " + std::to_string(static_cast<int>(tm_ms)) + "\n"
            + "shift_mode = " + std::to_string(shift_mode) + "\n"
            + "speed_kmph = " + std::to_string(speed_kmph) + "\n"
            + "steer_deg = " + std::to_string(steer_deg) + "\n"
            + "wheel_spd_fl = " + std::to_string(wheel_spd_fl) + "\n"
            + "wheel_spd_fr = " + std::to_string(wheel_spd_fr) + "\n"
            + "wheel_spd_rl = " + std::to_string(wheel_spd_rl) + "\n"
            + "wheel_spd_rr = " + std::to_string(wheel_spd_rr) + "\n"
            + "ds_cal_mode = " + std::to_string(static_cast<int>(ds_cal_mode)) + "\n"
            + "wheel_pulse_counter_fl = " + std::to_string(wheel_pulse_counter_fl) + "\n"
            + "wheel_pulse_counter_fr = " + std::to_string(wheel_pulse_counter_fr) + "\n"
            + "wheel_pulse_counter_rl = " + std::to_string(wheel_pulse_counter_rl) + "\n"
            + "wheel_pulse_counter_rr = " + std::to_string(wheel_pulse_counter_rr) + "\n"
            + "longitudinal_acceleration = " + std::to_string(longitudinal_acceleration) + "\n"
            + "lateral_acceleration = " + std::to_string(lateral_acceleration) + "\n"
            + "yaw_rate = " + std::to_string(yaw_rate) + "\n"
            + "driver_torque = " + std::to_string(driver_torque) + "\n";
        return ret;
    }

    MSGPACK_DEFINE_MAP(tm_ms, shift_mode, speed_kmph, steer_deg, wheel_spd_fl,
                       wheel_spd_fr, wheel_spd_rl, wheel_spd_rr, ds_cal_mode,
                       wheel_pulse_counter_fl, wheel_pulse_counter_fr, wheel_pulse_counter_rl, wheel_pulse_counter_rr,
                       longitudinal_acceleration, lateral_acceleration, yaw_rate, driver_torque);
};

// --------------------------------------------------------------------------
/// @brief 跟踪模块的结果输出
// --------------------------------------------------------------------------
struct CornerPtTrackRes {
    uint32_t id;  // 编号
    ParkingSpaceSpecs pattern;
    std::deque<ParkingSpaceCornerPt> refined_pts;  // 跟踪修正后的角点
    std::deque<ParkingSpaceCornerPt> predict_pts;  // 预测的角点

    CornerPtTrackRes() {}

    CornerPtTrackRes(const CornerPtTrackRes& lhs) { *this = lhs; }

    const CornerPtTrackRes& operator=(const CornerPtTrackRes& lhs) {
        id = lhs.id;
        pattern = lhs.pattern;
        refined_pts = lhs.refined_pts;
        predict_pts = lhs.predict_pts;
        return *this;
    }

    MSGPACK_DEFINE_MAP(id, pattern, refined_pts, predict_pts);
};

typedef std::deque<CornerPtTrackRes> TrackResList;

enum ParkSlotType {
    kUnknown = 0,
    kSimpleRect = 1,
    kParallelRect = 2,
    kDiamond = 3,
    kSlantedRect = 4
};

enum ParkingMode {
    kStop = 0,
    kSearching = 1,
    kParking = 2
};

struct ParkingSpace
{
    uint32_t             id;
    ParkingSpaceCornerPt corner_pts[4]; // 停车位的四个角点
    uint8_t              type;
    bool                 occupied;
    float                  width;  // 单位m
    float                  length; // 单位m

    ParkingSpace() : id(0), type(0), occupied(false), width(0), length(0)
    {
    }

    ParkingSpaceCornerPt GetCenter() const
    {
        ParkingSpaceCornerPt res = this->corner_pts[0];
        int size = sizeof(this->corner_pts) / sizeof(corner_pts[0]);

        for (int i = 1; i < size - 1; i++)
        {
            res.confidence += corner_pts[i].confidence;
            res.corner_pt.x += corner_pts[i].corner_pt.x;
            res.corner_pt.y += corner_pts[i].corner_pt.y;
            res.img_pt.x += corner_pts[i].img_pt.x;
            res.img_pt.y += corner_pts[i].img_pt.y;
        }

        res.id = this->id;
        res.corner_pt.x = res.corner_pt.x / size;
        res.corner_pt.y = res.corner_pt.y / size;
        res.img_pt.x = res.img_pt.x / size;
        res.img_pt.y = res.img_pt.y / size;
        res.confidence = res.confidence / size;
        return res;
    }

    std::string ToString() const {
        std::ostringstream oss;
        oss << " id = " << id;
        for (uint8_t i = 0; i < 4; i++) {
            oss << " p"<< i << corner_pts[i].corner_pt;
        }
        oss << " type = " << static_cast<uint32_t>(type) << " occupied = " << occupied;
        oss << " width = " << width << " length = " << length;
        return oss.str();
    }

    MSGPACK_DEFINE_MAP(id, corner_pts, type, occupied, width, length);
};

typedef std::deque<ParkingSpace> ParkingSpaceList;

struct ParkingSpaceDetectRes {
    ParkingSpaceList det_parking_spaces;  // 检测到的停车位信息
    CornerPtList feedback_pts;            // 反馈给track的points
    CornerPtList delete_pts;              // 反馈给track的points

    ParkingSpaceDetectRes() { det_parking_spaces.clear(); }

    ParkingSpaceDetectRes(const ParkingSpaceDetectRes& lhs) { *this = lhs; }

    const ParkingSpaceDetectRes& operator=(const ParkingSpaceDetectRes& lhs) {
        det_parking_spaces = lhs.det_parking_spaces;
        feedback_pts = lhs.feedback_pts;
        delete_pts = lhs.delete_pts;
        return *this;
    }

    MSGPACK_DEFINE_MAP(det_parking_spaces, feedback_pts, delete_pts);
};

enum ObstacleType {
    kVehicle = 0,
    kPerson = 1,
    kConeAndLock = 2,
    kGuardrail = 3,
    kUnknownObs = 4
};

struct ApaObstacle {
    cv::Point2f boundary_pts[4];  // 外接举行框,正视图坐标
    std::vector<cv::Point2f> world_pt_by_veh;  // 相对车身中心的世界坐标
    std::vector<cv::Point2f> world_pt_by_ori;  // 相对出发点的世界坐标
    std::vector<cv::Point2f> bird_view_pts;
    // 世界坐标对应到鸟瞰图坐标（相对车身中心）
    float dist;                    // 最近点距离车身中心的距离
    int8_t type;                   // 障碍物类别
    cv::Point2f mindist_world_pt;  // 最近距离点的世界坐标(相对车身中心)
    cv::Point2f mindist_bird_view_pt;  //最近距离点 鸟瞰图坐标
    ApaObstacle() {
        dist = -1;
        type = -1;
    }

    ApaObstacle(const ApaObstacle& lhs) { *this = lhs; }

    const ApaObstacle& operator=(const ApaObstacle& lhs) {
        dist = lhs.dist;
        type = lhs.type;
        mindist_world_pt = lhs.mindist_world_pt;
        mindist_bird_view_pt = lhs.mindist_bird_view_pt;
        world_pt_by_veh = lhs.world_pt_by_veh;
        world_pt_by_ori = lhs.world_pt_by_ori;
        bird_view_pts = lhs.bird_view_pts;
        for (int8_t i = 0; i < 4; ++i) {
            boundary_pts[i] = lhs.boundary_pts[i];
        }
        return *this;
    }
    MSGPACK_DEFINE_MAP(boundary_pts, dist, type, world_pt_by_veh,
                       world_pt_by_ori, bird_view_pts, mindist_world_pt,
                       mindist_bird_view_pt);
};
struct ObstaclesDetectRes {
    std::deque<ApaObstacle> objs;
    MSGPACK_DEFINE_MAP(objs);
};

struct ApaObsPoint {
    cv::Point2f veh_pt;       //相对车身中心的世界坐标
    cv::Point2f ori_pt;       //相对出发点的世界坐标
    cv::Point2f birdview_pt;  // 鸟瞰图上的坐标
    float dist;               //到车身的距离
    int obs_type;             //障碍物点类别
    MSGPACK_DEFINE_MAP(veh_pt, ori_pt, obs_type, dist, birdview_pt);
};

struct ObsPointList {
    std::deque<ApaObsPoint> objs;
    MSGPACK_DEFINE_MAP(objs);
};

struct UltrasonicRadarInfo {
    uint64_t tm_ms;
    double short_dis_radar_fl;
    double short_dis_radar_flm;
    double short_dis_radar_frm;
    double short_dis_radar_fr;
    double short_dis_radar_bl;
    double short_dis_radar_blm;
    double short_dis_radar_brm;
    double short_dis_radar_br;
    double long_dis_radar_fl;
    double long_dis_radar_fr;
    double long_dis_radar_bl;
    double long_dis_radar_br;
    UltrasonicRadarInfo()
        : tm_ms(0),
        short_dis_radar_fl(-1),
        short_dis_radar_flm(-1),
        short_dis_radar_frm(-1),
        short_dis_radar_fr(-1),
        short_dis_radar_bl(-1),
        short_dis_radar_blm(-1),
        short_dis_radar_brm(-1),
        short_dis_radar_br(-1),
        long_dis_radar_fl(-1),
        long_dis_radar_fr(-1),
        long_dis_radar_bl(-1),
        long_dis_radar_br(-1) {}
    const UltrasonicRadarInfo& operator=(const UltrasonicRadarInfo& lhs) {
        tm_ms = lhs.tm_ms;
        short_dis_radar_fl = lhs.short_dis_radar_fl;
        short_dis_radar_flm = lhs.short_dis_radar_flm;
        short_dis_radar_frm = lhs.short_dis_radar_frm;
        short_dis_radar_fr = lhs.short_dis_radar_fr;
        short_dis_radar_bl = lhs.short_dis_radar_bl;
        short_dis_radar_blm = lhs.short_dis_radar_blm;
        short_dis_radar_brm = lhs.short_dis_radar_brm;
        short_dis_radar_br = lhs.short_dis_radar_br;
        long_dis_radar_fl = lhs.long_dis_radar_fl;
        long_dis_radar_fr = lhs.long_dis_radar_fr;
        long_dis_radar_bl = lhs.long_dis_radar_bl;
        long_dis_radar_br = lhs.long_dis_radar_br;
        return *this;
    }

    const std::string ToString() {
        std::string ret;
        ret = "tm_ms = " + std::to_string(static_cast<int>(tm_ms)) + "\n"
            + "short_dis_radar_fl = " + std::to_string(short_dis_radar_fl) + "\n"
            + "short_dis_radar_flm = " + std::to_string(short_dis_radar_flm) + "\n"
            + "short_dis_radar_frm = " + std::to_string(short_dis_radar_frm) + "\n"
            + "short_dis_radar_fr = " + std::to_string(short_dis_radar_fr) + "\n"
            + "short_dis_radar_bl = " + std::to_string(short_dis_radar_bl) + "\n"
            + "short_dis_radar_blm = " + std::to_string(short_dis_radar_blm) + "\n"
            + "short_dis_radar_brm = " + std::to_string(short_dis_radar_brm) + "\n"
            + "short_dis_radar_br = " + std::to_string(short_dis_radar_br) + "\n"
            + "long_dis_radar_fl = " + std::to_string(long_dis_radar_fl) + "\n"
            + "long_dis_radar_fr = " + std::to_string(long_dis_radar_fr) + "\n"
            + "long_dis_radar_bl = " + std::to_string(long_dis_radar_bl) + "\n"
            + "long_dis_radar_br = " + std::to_string(long_dis_radar_br) + "\n";
        return ret;
    }
};

// todo： 范雨待定，是否采用 proto msg
struct FreespaceObstacle {
    uint64_t timestamp; //ms
    enum Type
    {
        UNKNOW = 0,
        DYNAMIC, // 动态
        STATIC   // 静态
    } type{ UNKNOW };
    uint32_t id{ 0U };
    std::vector<cv::Point2f> obstacle_edge_points;
};

struct FreespaceObstacles{
    // todo：范雨待定，是否采用 proto msg
    uint64 timestamp;
    std::vector<FreespaceObstacle> obstacles;
};


// odometery
struct Pose2 {
    float x;              // m
    float y;              // m
    float theta;          // rad
    uint64_t time_stamp;  // time stamp ms
    Pose2() : x(0), y(0), theta(0), time_stamp(0) {}

    Pose2(Pose2 const& pos)
        : x(pos.x), y(pos.y), theta(pos.theta), time_stamp(pos.time_stamp) {}

    //  update for 2d pose. maintain theta in range of [-M_PI,M_PI].
    void Update(float dx, float dy, float d_theta) {
        x += dx;
        y += dy;
        theta += d_theta;
        if (theta > M_PI) {
            theta -= 2.0 * M_PI;
        }
        if (theta < -M_PI) {
            theta += 2.0 * M_PI;
        }
    }

    Pose2& operator=(const Pose2& rval) {
        x = rval.x;
        y = rval.y;
        theta = rval.theta;
        time_stamp = rval.time_stamp;
        return *this;
    }
    MSGPACK_DEFINE_MAP(x, y, theta, time_stamp);
};

typedef std::vector<Pose2> PoseList;

struct ApaOdometry {
    // todo： 范雨待定，是否采用 proto msg
    uint64 timestamp_us;
    uint64 frame_id;
    Pose2 current_pose;
    PoseList history_pose;
    float velocity{ 0.f };
    float distance{ 0.f };
};

struct MatchPtMeas
{
    cv::Point2f pt_meas_v;
    cv::Point2f pt_pred_w;
    uint8_t conf;
    MatchPtMeas() : conf(0) {}
    MatchPtMeas(const MatchPtMeas& lhs) { *this = lhs; }
    const MatchPtMeas& operator=(const MatchPtMeas& lhs) {
        pt_meas_v = lhs.pt_meas_v;
        pt_pred_w = lhs.pt_pred_w;
        conf = lhs.conf;
        return *this;
    }
    MSGPACK_DEFINE_MAP(pt_meas_v, pt_pred_w, conf);
};

struct SingleUltrasonicFixParameters {
    float
        offset_x_;  //传感器安装位置相对车体中心,垂直于侧车身方向的距离,车头右侧方向为正
    float
        offset_y_;  //传感器安装位置相对车体中心,水平于侧车身方向的距离,车头前向为正
    float offset_z_;  //传感器安装位置相对地面高度,恒正
    float alfa_;  //传感器安装位置水平安装角度,车头右侧方向为正
    float beta_;  //传感器安装位置垂直安装角度,地面方向为负
    SingleUltrasonicFixParameters()
        : offset_x_(0),
          offset_y_(0),
          offset_z_(0),
          alfa_(-M_PI),
          beta_(-M_PI) {}
    MSGPACK_DEFINE_MAP(offset_x_, offset_y_, offset_z_, alfa_, beta_);
};

struct UltrasonicFixParameters {
    SingleUltrasonicFixParameters m_UltrasonicFixParameters[12];
    MSGPACK_DEFINE_MAP(m_UltrasonicFixParameters);
    //传感器安装位置数组的下标从0到11,依次代表着如下传感器
    //[0]    short_dis_radar_fl;
    //[1]    short_dis_radar_flm;
    //[2]    short_dis_radar_frm;
    //[3]    short_dis_radar_fr;
    //[4]    short_dis_radar_bl;
    //[5]    short_dis_radar_blm;
    //[6]    short_dis_radar_brm;
    //[7]    short_dis_radar_br;
    //[8]    long_dis_radar_fl;
    //[9]    long_dis_radar_fr;
    //[10]    long_dis_radar_bl;
    //[11]    long_dis_radar_br;
};

struct URadarRange {
    std::vector<float> st_alpha;
    std::vector<float> ed_alpha;
    float mindist, maxdist;
    bool valid() {
        if (st_alpha.size() != ed_alpha.size()) {
            return false;
        }
        if (mindist >= maxdist) {
            return false;
        }
        return true;
    }
    MSGPACK_DEFINE_MAP(st_alpha, ed_alpha, mindist, maxdist);
};
struct URadarRangelist {
    std::vector<URadarRange> objs;
    MSGPACK_DEFINE_MAP(objs)
};

struct CanInfo {
    std::vector<float> canValue;
    // canValue.size = 24
    /*CAN_MSG_TYPE_INVALID, CAN_MSG_TYPE_STEERING_WHEEL_ANGLE,
         CAN_MSG_TYPE_STEERING_WHEEL_SPEED, CAN_MSG_TYPE_YAW_RATE,
         CAN_MSG_TYPE_YAW_RATE_OFFSET, CAN_MSG_TYPE_WIPER_GEAR,
         CAN_MSG_TYPE_WHEELSPEED_FL, CAN_MSG_TYPE_WHEELSPEED_FR,
         CAN_MSG_TYPE_WHEELSPEED_RL, CAN_MSG_TYPE_WHEELSPEED_RR,
         CAN_MSG_TYPE_TARGET_GEAR, CAN_MSG_TYPE_APA_STATUS,
         CAN_MSG_TYPE_SHORT_DIS_RADAR_FL, CAN_MSG_TYPE_SHORT_DIS_RADAR_FLM,
         CAN_MSG_TYPE_SHORT_DIS_RADAR_FRM, CAN_MSG_TYPE_SHORT_DIS_RADAR_FR,
         CAN_MSG_TYPE_SHORT_DIS_RADAR_BL, CAN_MSG_TYPE_SHORT_DIS_RADAR_BLM,
         CAN_MSG_TYPE_SHORT_DIS_RADAR_BRM, CAN_MSG_TYPE_SHORT_DIS_RADAR_BR,
         CAN_MSG_TYPE_LONG_DIS_RADAR_FL, CAN_MSG_TYPE_LONG_DIS_RADAR_FR,
         CAN_MSG_TYPE_LONG_DIS_RADAR_BL, CAN_MSG_TYPE_LONG_DIS_RADAR_BR*/
    uint64_t ts;
    float speed;
    int turnlump;
    const CanInfo& operator=(const CanInfo& lhs) {
        canValue = lhs.canValue;
        ts = lhs.ts;
        speed = lhs.speed;
        turnlump = lhs.turnlump;
        return *this;
    }

    bool operator<(const CanInfo& obj) { return this->ts < obj.ts; }
    bool operator<=(const CanInfo& obj) { return this->ts <= obj.ts; }
    bool operator>=(const CanInfo& obj) { return this->ts >= obj.ts; }
    bool operator>(const CanInfo& obj) { return this->ts > obj.ts; }
    MSGPACK_DEFINE_MAP(canValue, ts, speed, turnlump);
};

struct VehicleParameters {
    uint8_t vehicle_plaform;     // 0|invalid 1|skoda
    float length;  // length of vehicle, m
    float width;  // width of vehicle, m
    float height;  // height of vehicle, m
    float min_turn_radius;  // minimum turning radius of vehicle, m
    float max_steer_angle;  // maximum steer angle of vehicle, deg
    float max_steer_angle_rate;  // maximum steer angle rate of vehicle, deg/s
    float steer_ratio;  // steer_ratio
    float wheel_base;  // wheel base, m
    float front_wheel_distance;  // front_wheel_distance, m
    float rear_wheel_distance;  // rear_wheel_distance, m
    float front_suspension_length;  // front_suspension_length, m
    float rear_suspension_length;  // rear_suspension_length, m
    float wheel_rolling_ratio;  // one counter is 0.0248 m distance
    float front_edge_to_center;  // front_edge_to_center, m
    float back_edge_to_center;  // back_edge_to_center, m
    float left_edge_to_center;  // left_edge_to_center, m
    float right_edge_to_center;  // right_edge_to_center, m
    float steer_dead_time;  // steer_dead_time
    float steer_rise_time;  // steer_rise_time
    float steer_peak_time;  // steer_peak_time
    float steer_settling_time;  // steer_settling_time

    VehicleParameters()
        : vehicle_plaform(0),
          length(-1),
          width(-1),
          height(-1),
          min_turn_radius(-1),
          max_steer_angle(-1),
          steer_ratio(-1),
          wheel_base(-1),
          front_wheel_distance(-1),
          rear_wheel_distance(-1),
          front_suspension_length(-1),
          rear_suspension_length(-1),
          wheel_rolling_ratio(-1),
          front_edge_to_center(-1),
          back_edge_to_center(-1),
          left_edge_to_center(-1),
          right_edge_to_center(-1),
          steer_dead_time(-1),
          steer_rise_time(-1),
          steer_peak_time(-1),
          steer_settling_time(-1) {}

    VehicleParameters(const VehicleParameters& lhs) { *this = lhs; }

    const VehicleParameters& operator=(const VehicleParameters& lhs) {
        vehicle_plaform = lhs.vehicle_plaform;
        length = lhs.length;
        width = lhs.width;
        height = lhs.height;
        min_turn_radius = lhs.min_turn_radius;
        max_steer_angle = lhs.max_steer_angle;
        steer_ratio = lhs.steer_ratio;
        wheel_base = lhs.wheel_base;
        front_wheel_distance = lhs.front_wheel_distance;
        rear_wheel_distance = lhs.rear_wheel_distance;
        front_suspension_length = lhs.front_suspension_length;
        rear_suspension_length = lhs.rear_suspension_length;
        wheel_rolling_ratio = lhs.wheel_rolling_ratio;
        front_edge_to_center = lhs.front_edge_to_center;
        back_edge_to_center = lhs.back_edge_to_center;
        left_edge_to_center = lhs.left_edge_to_center;
        right_edge_to_center = lhs.right_edge_to_center;
        steer_dead_time = lhs.steer_dead_time;
        steer_rise_time = lhs.steer_rise_time;
        steer_peak_time = lhs.steer_peak_time;
        steer_settling_time = lhs.steer_settling_time;
        return *this;
    }

    const std::string ToString() {
        std::string ret;
        ret = "vehicle_plaform = " + std::to_string(static_cast<int>(vehicle_plaform)) + "\n"
            + "length = " + std::to_string(length) + "\n"
            + "width = " + std::to_string(width) + "\n"
            + "height = " + std::to_string(height) + "\n"
            + "min_turn_radius = " + std::to_string(min_turn_radius) + "\n"
            + "max_steer_angle = " + std::to_string(max_steer_angle) + "\n"
            + "steer_ratio = " + std::to_string(steer_ratio) + "\n"
            + "wheel_base = " + std::to_string(wheel_base) + "\n"
            + "front_wheel_distance = " + std::to_string(front_wheel_distance) + "\n"
            + "rear_wheel_distance = " + std::to_string(rear_wheel_distance) + "\n"
            + "front_suspension_length = " + std::to_string(front_suspension_length) + "\n"
            + "rear_suspension_length = " + std::to_string(rear_suspension_length) + "\n"
            + "wheel_rolling_ratio = " + std::to_string(wheel_rolling_ratio) + "\n"
            + "front_edge_to_center = " + std::to_string(front_edge_to_center) + "\n"
            + "back_edge_to_center = " + std::to_string(back_edge_to_center) + "\n"
            + "left_edge_to_center = " + std::to_string(left_edge_to_center) + "\n"
            + "right_edge_to_center = " + std::to_string(right_edge_to_center) + "\n"
            + "steer_dead_time = " + std::to_string(steer_dead_time) + "\n";
            + "steer_rise_time = " + std::to_string(steer_rise_time) + "\n"
            + "steer_peak_time = " + std::to_string(steer_peak_time) + "\n"
            + "steer_settling_time = " + std::to_string(steer_settling_time) + "\n";
        return ret;
    }

    MSGPACK_DEFINE_MAP(vehicle_plaform, length, width, height, min_turn_radius,
                       max_steer_angle, steer_ratio, wheel_base, front_wheel_distance,
                       rear_wheel_distance, front_suspension_length, rear_suspension_length, wheel_rolling_ratio,
                       front_edge_to_center, back_edge_to_center, left_edge_to_center, right_edge_to_center,
                       steer_dead_time, steer_rise_time, steer_peak_time, steer_settling_time);
};

enum CURVE_SIDE
{
    INVALID = 0,
    LEFT,
    RIGHT,
    STRAIGHT
};

//enum MOVE_DIRECT{
//    INVALID = 0,
//    BACKWARD,
//    FORWARD
//};

struct Path
{
    int move_direct; // 0|inviad 1|backward 2|forward
    float   turn_radius;
    float   move_distance;


    Path() : move_direct(0),
             turn_radius(0.f),
             move_distance(0.f)
    {
    }

    Path(const int direct, const float r, const float s) :
        move_direct(direct),
        turn_radius(r),
        move_distance(s)
    {
    }

    Path(const Path& lhs) { *this = lhs; }

    const Path& operator=(const Path& lhs)
    {
        move_direct   = lhs.move_direct;
        turn_radius   = lhs.turn_radius;
        move_distance = lhs.move_distance;
        return *this;
    }

    const std::string ToString()
    {
        std::string ret;
        ret = "MovingDirect = " + std::to_string(static_cast<int>(move_direct)) + ", TurnningRadius = "
              + std::to_string(turn_radius) + ", MovingDistance = " + std::to_string(move_distance) + " ;";
        return ret;
    }
};

typedef std::vector<Path> Paths;

struct ImuMeasurment{
    uint64_t tm_us;
    double acc_x;
    double acc_y;
    double acc_z;
    double roll_rate;
    double pitch_rate;
    double yaw_rate;    //减去噪声之后
};
