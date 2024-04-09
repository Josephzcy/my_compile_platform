// Copyright [2019] - MiniEye INC.
#pragma once
#include <msgpack.hpp>
#include <string>
#include <vector>
#include "common/base/singleton.h"
#include "opencv2/opencv.hpp"

namespace apa {

struct DewarpTable {
    int xres;           // mm
    int yres;           // mm
    int range_x_start;  // mm
    int range_x_end;    // mm
    int range_y_start;  // mm
    int range_y_end;    // mm
    int crop_width;
    int crop_height;
    int origin_width;
    int origin_height;
    std::string dewarp_table[4];  // 0 - front, 1 - rear, 2 - left, 3 - right
    std::string inv_dewarp_table[4];
    MSGPACK_DEFINE_MAP(xres, yres, range_x_start, range_x_end, range_y_start,
                       range_y_end, crop_width, crop_height, origin_width,
                       origin_height, dewarp_table, inv_dewarp_table);
};

class CoordinateMapping : public SingletonBase<CoordinateMapping> {
   public:
    friend class SingletonBase<CoordinateMapping>;

    void Init(const std::string& mapping_config_file);

    // --------------------------------------------------------------------------
    /// @brief World2Raw
    ///
    /// @param world_pt 单位mm
    /// @param raw_pt
    ///
    /// @retval
    // --------------------------------------------------------------------------
    bool World2Raw(int8_t camera_idx, const cv::Point2f& world_pt,
                   cv::Point2f* raw_pt);

    bool Raw2World(int8_t camera_idx, const cv::Point& raw_pt,
                   cv::Point2f* world_pt);

   private:
    CoordinateMapping() : m_initialized(false) {}
    virtual ~CoordinateMapping() {}
    void LoadDewarpTable(const std::string& file_name,
                         std::vector<cv::Point2f>* points);
    void LoadInvDewarpTable(const std::string& file_name,
                            std::vector<cv::Point2f>* points);

   private:
    bool m_initialized;
    DewarpTable m_dewarp_config;
    std::vector<cv::Point2f> m_dewarp_tables[4];
    std::vector<cv::Point2f> m_inv_dewarp_tables[4];
    float m_scale_x;
    float m_scale_y;
};
}  // namespace apa
