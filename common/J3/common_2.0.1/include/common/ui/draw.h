#ifndef  DRAW_H
#define  DRAW_H

// @brief 只是简单的画线与画框

#include "lane_ui_param.h"
#include "drive_ui_param.h"

void DrawBigCarRect(cv::Mat&mat,
        const cv::Rect &rect,
        const cv::Scalar &rect_color,
        const cv::Scalar &line_color);

void VehicleDraw(const DrawRequest* req, DrawResponse* rsp);

void DrawLanes(cv::Mat &img, const std::vector<LaneDataType> &lanes);

void DrawDottedLine(cv::Mat &img, const cv::Point &pt1, const cv::Point &pt2, cv::Scalar color,
        int thickness, int lenghOfDots, int lineType, int leftToRight);

void LaneDraw(const DrawLaneRequest* req, DrawLaneResponse* rsp);


#endif  // DRAW_H
