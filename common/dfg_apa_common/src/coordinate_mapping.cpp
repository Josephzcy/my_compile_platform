// Copyright [2019] - MiniEye INC.
#include "coordinate_mapping.h"
#include <fstream>
#include <string>
#include "apa_interface.h"
#include "glog/logging.h"
#include "json.hpp"

namespace apa {
void CoordinateMapping::Init(const std::string& mapping_config_file) {
    if (m_initialized) {
        LOG(ERROR) << "already initialized.";
    }
    nlohmann::json config_json;
    std::ifstream config_stream(mapping_config_file, std::ifstream::in);
    config_stream >> config_json;

    std::vector<uint8_t> v_msgpack = nlohmann::json::to_msgpack(config_json);
    const char* ptr = reinterpret_cast<const char*>(&v_msgpack[0]);
    msgpack::object_handle oh = msgpack::unpack(ptr, v_msgpack.size());
    msgpack::object obj = oh.get();
    obj.convert(m_dewarp_config);
    config_stream.close();

    m_scale_x = m_dewarp_config.crop_width * 1.0 / m_dewarp_config.origin_width;
    m_scale_y =
        m_dewarp_config.crop_height * 1.0 / m_dewarp_config.origin_height;

    for (int8_t i = 0; i < 4; ++i) {
        LoadDewarpTable(m_dewarp_config.dewarp_table[i], &m_dewarp_tables[i]);
    }
    // inv dewarp
    for (int8_t i = 0; i < 4; ++i) {
        LoadInvDewarpTable(m_dewarp_config.inv_dewarp_table[i],
                           &m_inv_dewarp_tables[i]);
    }
    m_initialized = true;
}

bool CoordinateMapping::World2Raw(int8_t camera_idx,
                                  const cv::Point2f& world_pt,
                                  cv::Point2f* raw_pt) {
    int cols = (m_dewarp_config.range_x_end - m_dewarp_config.range_x_start) /
               m_dewarp_config.xres;
    int i = (m_dewarp_config.range_y_end - world_pt.y) / m_dewarp_config.yres;
    int j = (world_pt.x - m_dewarp_config.range_x_start) / m_dewarp_config.xres;
    int offset = i * cols + j;
    if (static_cast<uint32_t>(offset) >= m_dewarp_tables[camera_idx].size()) {
        // LOG(ERROR) << "out of range : camera_idx = "
        //            << static_cast<int>(camera_idx)
        //            << ", world_pt = " << world_pt;
        return false;
    }
    // raw_pt->x = static_cast<int>(
    //     m_dewarp_tables[camera_idx][offset].x * m_scale_x + 0.5);
    // raw_pt->y = static_cast<int>(
    //     m_dewarp_tables[camera_idx][offset].y * m_scale_y + 0.5);
    raw_pt->x =
        m_dewarp_tables[camera_idx][offset].x * m_scale_x;
    raw_pt->y =
        m_dewarp_tables[camera_idx][offset].y * m_scale_y;
    return true;
}

bool CoordinateMapping::Raw2World(int8_t camera_idx, const cv::Point& raw_pt,
                                  cv::Point2f* world_pt) {
    int x = static_cast<int>(raw_pt.x / m_scale_x + 0.5);
    int y = static_cast<int>(raw_pt.y / m_scale_y + 0.5);
    if (x < 0 || x >= m_dewarp_config.origin_width || y < 0 ||
        y >= m_dewarp_config.origin_height)
        return false;

    int offset = y * m_dewarp_config.origin_width + x;
    world_pt->x = m_inv_dewarp_tables[camera_idx][offset].x;
    world_pt->y = m_inv_dewarp_tables[camera_idx][offset].y;
    return true;
}

void CoordinateMapping::LoadDewarpTable(const std::string& file_name,
                                        std::vector<cv::Point2f>* points) {
    points->clear();
    std::ifstream in;
    in.open(file_name, std::ios::in | std::ios::binary);
    if (!in.good()) {
        LOG(ERROR) << "open config file failed : " << file_name;
        return;
    }

    cv::Point2f pt;
    while (in.read(reinterpret_cast<char*>(&pt), sizeof(pt))) {
        points->push_back(pt);
    }
    in.close();
}

void CoordinateMapping::LoadInvDewarpTable(const std::string& file_name,
                                           std::vector<cv::Point2f>* points) {
    points->clear();
    std::ifstream in;
    in.open(file_name, std::ios::in | std::ios::binary);
    if (!in.good()) {
        LOG(ERROR) << "open config file failed : " << file_name;
        return;
    }

    cv::Point2f pt;
    while (in.read(reinterpret_cast<char*>(&pt), sizeof(pt))) {
        points->push_back(pt);
    }
    in.close();
}
}  // namespace apa
