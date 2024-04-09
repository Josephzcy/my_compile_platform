/// @file draw.h
/// @brief
/// @author zac (zac@minieye.cc)
/// @date 2015-10-10
/// Copyright (C) 2015 - MiniEye INC.

#ifndef  COMMON_UI_DRIVE_UI_H
#define  COMMON_UI_DRIVE_UI_H

#include "common/ui/drive_ui_param.h"


class DriveUI
{
public:
    DriveUI() { }

    virtual ~DriveUI() { }

    void Init();

    void Draw(const DrawRequest *req, DrawResponse *rsp);

private:
    void BlendMat(cv::Mat &mat1, cv::Mat &mat2, cv::Mat &dst);

    void OverlayImage(const cv::Mat &background, const cv::Mat &foreground, cv::Mat &output, cv::Point2i location,
                      double alpha = 1.0);

    void DrawText(cv::Mat &mat, const cv::Point &pos, double size, int thickness, const char *str,
                  const cv::Scalar &color = CV_RGB(255, 255, 255));

    void DrawTextInCenter(cv::Mat &mat, const cv::Rect &rect, double size, int thickness, const char *str,
                          const cv::Scalar &color = CV_RGB(255, 255, 255));

    void DrawNum(cv::Mat &mat, const cv::Point &pos, const int num, bool left2right = true);

    void DrawBigCarRect(cv::Mat &mat, const cv::Rect &rect, const cv::Scalar &rect_color, const cv::Scalar &line_color);

    void DrawSmallCarRect(cv::Mat &mat, const cv::Rect &rect, const cv::Scalar &color = cvScalar(32, 250, 253));

    void DoBeep();

    void DrawUI(cv::Mat &mat, double speed);

    void DrawScene(const DrawRequest *req, DrawResponse *rsp);

private:
    static const int MASK_SIZE = 3;
    static const int LOCATION_SIZE = 3;
    static const int NUM_SIZE = 10;
    static const int SPEED_SIZE = 9;
    static const int SPEED_MAX = 200;
    static const int NUM_W = 30;
    static const int NUM_H = 40;
    static const int NUM_GAP = 1;
    static const cv::Point SPEED_NUM_POS;
    static const cv::Point SPEED_UNIT_POS;
    static const cv::Point LATITUDE_POS; // 纬度
    static const cv::Point LONGITUDE_POS; // 经度
    static const int LOCATION_VALUE_LIMIT = 500;

    static const CvScalar CAR_RECT_COLOR;
    static const CvScalar CAR_LINE_COLOR;
    static const CvScalar HIT_CAR_RECT_COLOR;
    static const CvScalar HIT_CAR_LINE_COLOR;
    static const CvScalar HIT_CAR_DIS_CLOSE_COLOR;
    static const CvScalar HIT_CAR_DIS_WARN_COLOR;

    static const CvScalar LANE_COLOR_HIT;
    static const cv::Rect LANE_ALERT_RECT;

    char m_str_buffer[1000];

    cv::Mat m_mask_mat[MASK_SIZE];
    cv::Mat m_location_mat[LOCATION_SIZE];
    cv::Mat m_num_mat[NUM_SIZE];
    cv::Mat m_speed_mat[SPEED_SIZE];
    cv::Mat m_danger_mat;

    // hit alert
    HitAlertType m_last_hit_type;
    int m_beep_cnt;
};

#endif  // COMMON_UI_DRIVE_UI_H
