/// @file lane_ui.h
/// @brief
/// @author zac (zac@minieye.cc)
/// @date 2015-11-09
/// Copyright (C) 2015 - MiniEye INC.

#ifndef  COMMON_UI_LANE_UI_H
#define  COMMON_UI_LANE_UI_H

#include "common/ui/lane_ui_param.h"


class LaneUI
{
public:
    LaneUI() { }

    virtual ~LaneUI() { }

    void Init();

    void Draw(const DrawLaneRequest *req, DrawLaneResponse *rsp);

private:
    void DrawDottedLine(cv::Mat &img, const cv::Point &pt1, const cv::Point &pt2, CvScalar color, int thickness,
                        int lenghOfDots, int lineType, int leftToRight);

    void BlendMat(cv::Mat &mat1, const cv::Mat &mat2, cv::Mat &dst);

    void DrawText(cv::Mat &mat, const cv::Point &pos, double size, int thickness, const char *str,
                  const cv::Scalar &color = CV_RGB(255, 255, 255));

    void DrawTextInCenter(cv::Mat &mat, const cv::Rect &rect, double size, int thickness, const char *str);

    void DrawNum(cv::Mat &mat, const cv::Point &pos, const int num, bool left2right = true);

    void DrawLaneWidthImg(cv::Mat &img, const cv::Mat &width_mat, const cv::Point &pos, float width);

    void DrawLanePropertyText(cv::Mat &img, const cv::Point &pos, float width, LaneColorTypeEnum lane_color);

    void DrawLaneSolid(cv::Mat &img, const LaneDataType &lane);

    void DrawLaneDotted(cv::Mat &img, const LaneDataType &lane);

    //void DrawLaneSolid2(cv::Mat &img, const LaneDataType &lane);

    void DrawLanes(cv::Mat &img, const std::vector<LaneDataType> &lanes);

    void DrawLanesProperty(cv::Mat &img, const std::vector<LaneDataType> &lanes);

    CvScalar GetLaneColor(LaneTypeEnum lane_type, bool dark = false);

    //CvScalar GetLaneColor(const std::vector<LaneDataType> &lanes, bool left, bool dark = false);

    void DrawUI(cv::Mat &mat, int speed);

    void DrawLaneScene(const DrawLaneRequest *_request, DrawLaneResponse *_response);

private:
    static const int MASK_SIZE = 3;
    //static const int LOCATION_SIZE = 3;
    static const int NUM_SIZE = 10;
    static const int SPEED_SIZE = 9;
    static const int SPEED_MAX = 200;
    static const int NUM_W = 30;
    static const int NUM_H = 40;
    static const int NUM_GAP = 1;
    static const cv::Point SPEED_NUM_POS;
    static const cv::Point SPEED_UNIT_POS;
    //static const cv::Point LATITUDE_POS; // 纬度
    //static const cv::Point LONGITUDE_POS; // 经度

    static const int LANE_DIS_FULL = 50;
    static const int LANE_DIS_LIMIT = 10000000;

    // color
    static const CvScalar LANE_COLOR_0;
    static const CvScalar LANE_COLOR_1;
    //static const CvScalar LANE_COLOR_2;

    static const CvScalar LANE_COLOR_RED;

    static const CvScalar LANE_COLOR_HIT;
    static const CvScalar LANE_COLOR_DEVIATE_0;
    static const CvScalar LANE_COLOR_DEVIATE_1;
    //static const CvScalar LANE_COLOR_DEVIATE_2;

    static const cv::Rect LANE_WIDTH_RECT;
    static const cv::Rect LANE_WIDTH_INNER_RECT;
    static const cv::Rect LANE_DIS_LEFT_RECT;
    static const cv::Rect LANE_DIS_RIGHT_RECT;
    static const cv::Rect LANE_HIT_ALERT_RECT;
    static const cv::Rect LANE_DEVIATE_ALERT_RECT;
    static const cv::Rect LANE_ALERT_RECT;

    char m_str_buffer[1000];

    cv::Mat m_mask_mat[MASK_SIZE];
    //cv::Mat m_location_mat[LOCATION_SIZE];
    cv::Mat m_num_mat[NUM_SIZE];
    cv::Mat m_speed_mat[SPEED_SIZE];

    cv::Mat m_lane_width_mat;
    cv::Mat m_lane_thickness_mat[3];
    cv::Mat m_lane_dis_mat[3][2];
};

#endif  // COMMON_UI_LANE_UI_H
