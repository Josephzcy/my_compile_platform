/// @file drive_ui_param.h
/// @brief
/// @author zac (zac@minieye.cc)
/// @date 2015-11-09
/// Copyright (C) 2015 - MiniEye INC.

#ifndef  COMMON_UI_LANE_UI_PARAM_H
#define  COMMON_UI_LANE_UI_PARAM_H

#include "opencv2/opencv.hpp"


enum LaneTypeEnum
{
    ENUM_LANE_SOLID = 0,
    ENUM_LANE_DOTTED = 1,
    //ENUM_LANE_SOLID_2 = 2,
    //ENUM_LANE_SOLID_DOTTED,
    //ENUM_LANE_DOTTED_SOLID,
    //ENUM_LANE_DOTTED_2,
};

enum DeviateTypeEnum
{
    ENUM_DEVIATE_NO,
    ENUM_DEVIATE_LEFT,
    ENUM_DEVIATE_RIGHT,
};

enum LaneColorTypeEnum
{
    ENUM_LANE_COLOR_NONE,
    ENUM_LANE_COLOR_WHITE,
    ENUM_LANE_COLOR_YELLOW,
};

struct LaneDataType
{
    LaneTypeEnum lane_type;
    float width;
    LaneColorTypeEnum lane_color;
    cv::Point pos_top;
    cv::Point pos_bottom;
};

struct DrawLaneRequest
{
    DrawLaneRequest() : left_dis(0), right_dis(0), width(-1), hit(0), deviate(ENUM_DEVIATE_NO), deviate_index(-1),
                        speed(0), save(false) {
    }

    cv::Mat img;
    std::vector<LaneDataType> lanes;
    float left_dis;
    float right_dis;
    float width;
    int hit; // 0 no 1 yes
    DeviateTypeEnum deviate;
    int deviate_index;

    // speed
    double speed;

    //// location
    //double latitude;
    //double longitude;

    // save
    bool save;
    std::string dir_name;
    std::string image_name;
};

struct DrawLaneResponse
{
    cv::Mat img;
};


#endif  // COMMON_UI_LANE_UI_PARAM_H
