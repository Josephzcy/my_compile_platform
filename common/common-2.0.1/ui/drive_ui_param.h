/// @file drive_ui_param.h
/// @brief
/// @author zac (zac@minieye.cc)
/// @date 2015-10-14
/// Copyright (C) 2015 - MiniEye INC.

#ifndef  COMMON_UI_DRIVE_UI_PARAM_H
#define  COMMON_UI_DRIVE_UI_PARAM_H

#include "opencv2/opencv.hpp"
#include <string>

enum ShowRectType
{
    kShowNone = 0,
    kShowBig,
    kShowSmall,
    kShowAll,
};

enum HitAlertType
{
    kHitAlertNone = 0,
    kHitAlertClose,
    kHitAlertWarn,
    kHitAlertDanger,
};

struct CarRect
{
    std::string classify_type;
    cv::Rect rect;
    cv::Rect b_rect;
};

struct DrawRequest
{
    DrawRequest() : speed(0), latitude(10000), longitude(10000), show_type(kShowNone), save(false),
                    hit_type(kHitAlertNone) {
    }

    void SetShowMark(int mark) {
        show_type = static_cast<ShowRectType>(mark);
    }

    cv::Mat image;

    // speed
    double speed;

    // location
    double latitude;
    double longitude;

    // car
    ShowRectType show_type;
    std::vector<CarRect> rects;

    // save
    bool save;
    std::string dir_name;
    std::string image_name;

    // hit alert
    HitAlertType hit_type;
    int hit_car_index; // index of rects
    double hit_car_dis;
    double hit_car_time;
};

struct DrawResponse
{
    cv::Mat image;
};


#endif  // COMMON_UI_DRIVE_UI_PARAM_H
