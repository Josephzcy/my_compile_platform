/// @file lane_ui.cpp
/// @brief
/// @author zac (zac@minieye.cc)
/// @date 2015-11-09
/// Copyright (C) 2015 - MiniEye INC.

#include <limits>
#include <stdio.h>
#include "common/string/string_number.h"
#include "common/base/log_level.h"
#include "common/ui/lane_ui.h"


const cv::Point LaneUI::SPEED_NUM_POS = cv::Point(266, 648);
const cv::Point LaneUI::SPEED_UNIT_POS = cv::Point(272, 646);
//const cv::Point LaneUI::LATITUDE_POS = cv::Point(1056, 622);
//const cv::Point LaneUI::LONGITUDE_POS = cv::Point(1056, 646);

const CvScalar LaneUI::LANE_COLOR_0 = CV_RGB(52, 128, 255);
const CvScalar LaneUI::LANE_COLOR_1 = CV_RGB(108, 248, 59);
//const CvScalar LaneUI::LANE_COLOR_2 = CV_RGB(253, 120, 32);

const CvScalar LaneUI::LANE_COLOR_RED = CV_RGB(240, 55, 64);

const CvScalar LaneUI::LANE_COLOR_HIT = CV_RGB(152, 41, 47);
const CvScalar LaneUI::LANE_COLOR_DEVIATE_0 = CV_RGB(39, 84, 161);
const CvScalar LaneUI::LANE_COLOR_DEVIATE_1 = CV_RGB(72, 156, 44);
//const CvScalar LaneUI::LANE_COLOR_DEVIATE_2 = CV_RGB(158, 78, 26);

const cv::Rect LaneUI::LANE_WIDTH_RECT = cv::Rect(550, 516, 176, 60);
const cv::Rect LaneUI::LANE_WIDTH_INNER_RECT = cv::Rect(558, 516, 160, 60);
const cv::Rect LaneUI::LANE_DIS_LEFT_RECT = cv::Rect(0, 516, 70, 60);
const cv::Rect LaneUI::LANE_DIS_RIGHT_RECT = cv::Rect(1210, 516, 70, 60);
const cv::Rect LaneUI::LANE_HIT_ALERT_RECT = cv::Rect(410, 600, 230, 60);
const cv::Rect LaneUI::LANE_DEVIATE_ALERT_RECT = cv::Rect(640, 600, 230, 60);
const cv::Rect LaneUI::LANE_ALERT_RECT = cv::Rect(410, 600, 460, 60);


void LaneUI::Init() {
#if defined(__ANDROID__) || defined(__gnu_linux__)
    std::string image_dir_path = "./common/resource/image";
#elif defined(__APPLE__)
    std::string image_dir_path = "./resource/image";
#else
    std::string image_dir_path = "../../common/resource/image";
#endif
    // mask
    for (int i = 0; i < MASK_SIZE; ++i) {
        sprintf(m_str_buffer, (image_dir_path + "/mask/%d.png").c_str(), i);
        m_mask_mat[i] = cv::imread(m_str_buffer, CV_LOAD_IMAGE_UNCHANGED);
    }
    //// locatin
    //for (int i = 0; i < LOCATION_SIZE; ++i) {
    //    sprintf(m_str_buffer, (image_dir_path + "/location/%d.png").c_str(), i);
    //    m_location_mat[i] = cv::imread(m_str_buffer, CV_LOAD_IMAGE_UNCHANGED);
    //}
    // speed
    for (int i = 0; i < SPEED_SIZE; ++i) {
        sprintf(m_str_buffer, (image_dir_path + "/speed/%d.png").c_str(), i);
        m_speed_mat[i] = cv::imread(m_str_buffer, CV_LOAD_IMAGE_UNCHANGED);
    }
    // nubmer
    for (int i = 0; i < NUM_SIZE; ++i) {
        sprintf(m_str_buffer, (image_dir_path + "/num/%d.png").c_str(), i);
        m_num_mat[i] = cv::imread(m_str_buffer, CV_LOAD_IMAGE_UNCHANGED);
    }

    m_lane_width_mat = cv::imread((image_dir_path + "/lane/lane_width.png").c_str(), CV_LOAD_IMAGE_UNCHANGED);

    for (int i = 0; i < 2; ++i) {
        sprintf(m_str_buffer, (image_dir_path + "/lane/lane_thickness_%d.png").c_str(), i);
        m_lane_thickness_mat[i] = cv::imread(m_str_buffer, CV_LOAD_IMAGE_UNCHANGED);
    }

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            sprintf(m_str_buffer, (image_dir_path + "/lane/lane_dis_%d_%d.png").c_str(), i, j);
            m_lane_dis_mat[i][j] = cv::imread(m_str_buffer, CV_LOAD_IMAGE_UNCHANGED);
        }
    }
}

void LaneUI::DrawDottedLine(cv::Mat &img, const cv::Point &pt1, const cv::Point &pt2, CvScalar color, int thickness,
                            int lenghOfDots, int lineType, int leftToRight) {
    cv::LineIterator iterator(img, pt1, pt2, lineType, leftToRight);
    int count = iterator.count;
    int offset, x, y;
    int channels = img.channels();

    for (int i = 0; i < count; i = i + (lenghOfDots * 2 - 1)) {
        if (i + lenghOfDots > count)
            break;

        offset = iterator.ptr - (uchar *) (img.data);
        y = offset / img.step;
        x = (offset - y * img.step) / (channels * sizeof(uchar) /* size of pixel */);

        cv::Point lTemp1(x, y);
        for (int j = 0; j < lenghOfDots - 1; j++)    //I want to know have the last of these in the iterator
            ++iterator;

        offset = iterator.ptr - (uchar *) (img.data);
        y = offset / img.step;
        x = (offset - y * img.step) / (channels * sizeof(uchar) /* size of pixel */);

        cv::Point lTemp2(x, y);
        cv::line(img, lTemp1, lTemp2, color, thickness, lineType);
        for (int j = 0; j < lenghOfDots; j++)
            ++iterator;
    }
}

void LaneUI::BlendMat(cv::Mat &mat1, const cv::Mat &mat2, cv::Mat &dst) {
    CV_Assert(mat1.channels() == 4);
    CV_Assert(mat2.channels() == 4);
    CV_Assert(dst.channels() == 4);

    cv::Mat_<cv::Vec4b> _mat1 = mat1;
    const cv::Mat_<cv::Vec4b> _mat2 = mat2;
    cv::Mat_<cv::Vec4b> _dst = dst;

    for (int i = 0; i < mat1.rows; ++i)
        for (int j = 0; j < mat1.cols; ++j) {
            double alpha = _mat2(i, j)[3];
            _dst(i, j)[0] = _mat1(i, j)[0] * (1 - alpha / 255) + _mat2(i, j)[0] * (alpha / 255);
            _dst(i, j)[1] = _mat1(i, j)[1] * (1 - alpha / 255) + _mat2(i, j)[1] * (alpha / 255);
            _dst(i, j)[2] = _mat1(i, j)[2] * (1 - alpha / 255) + _mat2(i, j)[2] * (alpha / 255);
            _dst(i, j)[3] = 0;
        }
    dst = _dst;
}

void LaneUI::DrawText(cv::Mat &mat, const cv::Point &pos, double size, int thickness, const char *str,
                      const cv::Scalar &color) {
    cv::putText(mat, str, pos, CV_FONT_HERSHEY_DUPLEX, size, color, thickness, CV_AA);
}

void LaneUI::DrawTextInCenter(cv::Mat &mat, const cv::Rect &rect, double size, int thickness, const char *str) {
    int base_line;
    cv::Size text_size = cv::getTextSize(str, CV_FONT_HERSHEY_DUPLEX, size, thickness, &base_line);
    int x = rect.x + (rect.width - text_size.width) / 2;
    int y = rect.y + (rect.height + text_size.height) / 2;
    cv::putText(mat, str, cv::Point(x, y), CV_FONT_HERSHEY_DUPLEX, size, CV_RGB(255, 255, 255), thickness, CV_AA);
}

void LaneUI::DrawNum(cv::Mat &mat, const cv::Point &pos, const int num, bool left2right) {
    // cal number cnt
    int cnt = 0;
    int tmp = num;
    do {
        ++cnt;
        tmp /= 10;
    } while (tmp > 0);

    // cal pos
    cv::Point leftPos(pos);
    cv::Point rightPos(pos);
    if (left2right) {
        rightPos.x += NUM_W;
        rightPos.x += (NUM_W + NUM_GAP) * (cnt - 1);
    } else {
        leftPos.x -= NUM_W - 1;
        leftPos.x -= (NUM_W + NUM_GAP) * (cnt - 1);
    }

    // verify
    if (leftPos.x < 0 || rightPos.x >= mat.cols || leftPos.y < NUM_H - 1 || leftPos.y >= mat.rows)
        return;

    // draw from right
    tmp = num;
    do {
        int x = tmp % 10;
        tmp /= 10;

        cv::Mat imgRoi = mat(cv::Rect(rightPos.x - NUM_W + 1, rightPos.y - NUM_H + 1, NUM_W, NUM_H));
        BlendMat(imgRoi, m_num_mat[x], imgRoi);

        rightPos.x -= NUM_W + NUM_GAP;
    } while (tmp > 0);
}

void LaneUI::DrawLaneWidthImg(cv::Mat &img, const cv::Mat &width_mat, const cv::Point &pos, float width) {
    if (width > 0.0) {
        cv::Rect rect = cv::Rect(pos.x + 20, pos.y - 5, 24, 10);
        if (rect.x < 0 || rect.y < 0 || rect.x + rect.width > img.size().width ||
            rect.y + rect.height > img.size().height)
            return;
        cv::Mat to_mat = img(rect);
        BlendMat(to_mat, width_mat, to_mat);
    }
}

void LaneUI::DrawLanePropertyText(cv::Mat &img, const cv::Point &pos, float width, LaneColorTypeEnum lane_color) {
    if (width > 0.0) {
        sprintf(m_str_buffer, "%.0lf", round(width));
        DrawTextInCenter(img, cv::Rect(pos.x + 20, pos.y - 5, 24, 10), 0.4, 1, m_str_buffer);
    }
    if (lane_color == ENUM_LANE_COLOR_WHITE)
        DrawText(img, cv::Point(pos.x + 50, pos.y + 2), 0.4, 1, "white");
    else if (lane_color == ENUM_LANE_COLOR_YELLOW)
        DrawText(img, cv::Point(pos.x + 50, pos.y + 2), 0.4, 1, "yellow");
}

void LaneUI::DrawLaneSolid(cv::Mat &img, const LaneDataType &lane) {
    //cv::line(img, lane.pos_top, lane.pos_bottom, LANE_COLOR_0, 2, CV_AA);
    cv::line(img, lane.pos_top, lane.pos_bottom, GetLaneColor(ENUM_LANE_SOLID), 3, 8);

    DrawLaneWidthImg(img, m_lane_thickness_mat[0], lane.pos_bottom, lane.width);
}

void LaneUI::DrawLaneDotted(cv::Mat &img, const LaneDataType &lane) {
    //cv::line(img, lane.pos_top, lane.pos_bottom, LANE_COLOR_1, 3, 8);
    DrawDottedLine(img, lane.pos_top, lane.pos_bottom, GetLaneColor(ENUM_LANE_DOTTED), 2, 20, 8, 0);
    DrawLaneWidthImg(img, m_lane_thickness_mat[1], lane.pos_bottom, lane.width);
}

//void LaneUI::DrawLaneSolid2(cv::Mat &img, const LaneDataType &lane) {
//    cv::line(img, cv::Point(lane.pos_top.x - 4, lane.pos_top.y), cv::Point(lane.pos_bottom.x - 4, lane.pos_bottom.y),
//             LANE_COLOR_2, 1, CV_AA);
//    cv::line(img, cv::Point(lane.pos_top.x + 4, lane.pos_top.y), cv::Point(lane.pos_bottom.x + 4, lane.pos_bottom.y),
//             LANE_COLOR_2, 1, CV_AA);
//
//    DrawLaneWidthImg(img, m_lane_thickness_mat[2], lane.pos_bottom, lane.width);
//}

void LaneUI::DrawLanes(cv::Mat &img, const std::vector<LaneDataType> &lanes) {
    for (int i = 0; i < (int) lanes.size(); ++i) {
        switch (lanes[i].lane_type) {
            case ENUM_LANE_SOLID:
                DrawLaneSolid(img, lanes[i]);
                break;
            case ENUM_LANE_DOTTED:
                DrawLaneDotted(img, lanes[i]);
                break;
                //case ENUM_LANE_SOLID_2:
                //    DrawLaneSolid2(img, lanes[i]);
                //    break;
            default:
                break;
        }
    }
}

void LaneUI::DrawLanesProperty(cv::Mat &img, const std::vector<LaneDataType> &lanes) {
    for (int i = 0; i < (int) lanes.size(); ++i) {
        switch (lanes[i].lane_type) {
            case ENUM_LANE_SOLID:
                DrawLanePropertyText(img, lanes[i].pos_bottom, lanes[i].width, lanes[i].lane_color);
                break;
            case ENUM_LANE_DOTTED:
                DrawLanePropertyText(img, lanes[i].pos_bottom, lanes[i].width, lanes[i].lane_color);
                break;
                //case ENUM_LANE_SOLID_2:
                //    DrawLanePropertyText(img, lanes[i].pos_bottom, lanes[i].width, lanes[i].is_yellow);
                //    break;
            default:
                break;
        }
    }
}

CvScalar LaneUI::GetLaneColor(LaneTypeEnum lane_type, bool dark) {
    return dark ? LANE_COLOR_DEVIATE_1 : LANE_COLOR_1;
    switch (lane_type) {
        case ENUM_LANE_SOLID:
            return dark ? LANE_COLOR_DEVIATE_0 : LANE_COLOR_0;
        case ENUM_LANE_DOTTED:
            return dark ? LANE_COLOR_DEVIATE_1 : LANE_COLOR_1;
            //case ENUM_LANE_SOLID_2:
            //    return dark ? LANE_COLOR_DEVIATE_2 : LANE_COLOR_2;
        default:
            return dark ? LANE_COLOR_DEVIATE_0 : LANE_COLOR_0;
    }
}

void LaneUI::DrawUI(cv::Mat &mat, int speed) {
    // mask
    int mask_height = 0;

    cv::Mat mask_image0_mat = mat(cv::Rect(0, mask_height, m_mask_mat[0].cols, m_mask_mat[0].rows));
    cv::addWeighted(mask_image0_mat, 0.7, m_mask_mat[0], 1.0, 0, mask_image0_mat);
    mask_height += m_mask_mat[0].rows;

    cv::Mat mask_image1_mat = mat(cv::Rect(0, mask_height, m_mask_mat[1].cols, m_mask_mat[1].rows));
    cv::addWeighted(mask_image1_mat, 0.2, m_mask_mat[1], 1.0, 0, mask_image1_mat);
    mask_height += m_mask_mat[1].rows;

    cv::Mat mask_image2_mat = mat(cv::Rect(0, mask_height, m_mask_mat[2].cols, m_mask_mat[2].rows));
    m_mask_mat[2].copyTo(mask_image2_mat);
    //BlendMat(mat, maskMat, mat);

    // speed
    int speed_index = (int) round(speed * 1.0 / SPEED_MAX * SPEED_SIZE);
    speed_index = std::min(speed_index, SPEED_SIZE - 1);
    cv::Mat image_speed = mat(cv::Rect(100, 600, m_speed_mat[speed_index].cols, m_speed_mat[speed_index].rows));
    BlendMat(image_speed, m_speed_mat[speed_index], image_speed);

    //// location
    //static int location_index = 0;
    //location_index %= LOCATION_SIZE;
    //cv::Mat image_location = mat(
    //        cv::Rect(980, 600, m_location_mat[location_index].cols, m_location_mat[location_index].rows));
    //BlendMat(image_location, m_location_mat[location_index], image_location);
    //++location_index;

    DrawNum(mat, SPEED_NUM_POS, speed, false);
}

void LaneUI::DrawLaneScene(const DrawLaneRequest *_request, DrawLaneResponse *_response) {
    if (_request == NULL || _response == NULL)
        return;
    const DrawLaneRequest &req = *_request;
    DrawLaneResponse &rsp = *_response;

    rsp.img = req.img;
    cv::Mat &img = rsp.img;
    cv::cvtColor(img, img, CV_BGR2BGRA);

    DrawUI(img, req.speed);//, req.latitude, req.longitude);

    DrawLanes(img, req.lanes);

    cv::Mat roi;
    cv::Mat rectMat;
    double alpha = 0.3;

    // lane width
    if (req.width >= 0.0) {
        cv::Mat lane_width_to_mat = img(LANE_WIDTH_RECT);
        BlendMat(lane_width_to_mat, m_lane_width_mat, lane_width_to_mat);
        roi = img(LANE_WIDTH_INNER_RECT);
        rectMat = cv::Mat(roi.size(), CV_8UC4, LANE_COLOR_RED);
        cv::addWeighted(rectMat, alpha, roi, 1.0 - alpha, 0.0, roi);
    }

    // left dis
    if (fabs(req.left_dis) < LANE_DIS_LIMIT) {
        cv::Mat lane_dis_to_mat = img(cv::Rect(70, 516, 8, 60));
        BlendMat(lane_dis_to_mat, m_lane_dis_mat[1][0], lane_dis_to_mat);

        if (req.left_dis < LANE_DIS_FULL) {
            int width = req.left_dis > 0 ? (int) (round(req.left_dis / LANE_DIS_FULL * 70)) : 0;
            roi = img(cv::Rect(width, 516, 70 - width, 60));
            rectMat = cv::Mat(roi.size(), CV_8UC4, GetLaneColor(ENUM_LANE_SOLID));
            cv::addWeighted(rectMat, alpha, roi, 1.0 - alpha, 0.0, roi);
        }
    }

    // right dis
    if (fabs(req.right_dis) < LANE_DIS_LIMIT) {
        cv::Mat lane_dis_to_mat = img(cv::Rect(1202, 516, 8, 60));
        BlendMat(lane_dis_to_mat, m_lane_dis_mat[1][1], lane_dis_to_mat);

        if (req.right_dis < LANE_DIS_FULL) {
            int width = req.right_dis > 0 ? (int) (round(req.right_dis / LANE_DIS_FULL * 70)) : 0;
            roi = img(cv::Rect(1210, 516, 70 - width, 60));
            rectMat = cv::Mat(roi.size(), CV_8UC4, GetLaneColor(ENUM_LANE_SOLID));
            cv::addWeighted(rectMat, alpha, roi, 1.0 - alpha, 0.0, roi);
        }
    }

    // alert
    bool show_hit = req.hit == 1;
    bool show_deviate = (req.deviate_index >= 0 && req.deviate_index < (int) req.lanes.size()) &&
                        (req.deviate == ENUM_DEVIATE_LEFT || req.deviate == ENUM_DEVIATE_RIGHT);

    if (show_hit) {
        roi = img(show_deviate ? LANE_HIT_ALERT_RECT : LANE_ALERT_RECT);
        rectMat = cv::Mat(roi.size(), CV_8UC4, LANE_COLOR_HIT);
        rectMat.copyTo(roi);
        //cv::addWeighted(rectMat, alpha, roi, 1.0 - alpha, 0.0, roi);
    }
    if (show_deviate) {
        roi = img(show_hit ? LANE_DEVIATE_ALERT_RECT : LANE_ALERT_RECT);
        rectMat = cv::Mat(roi.size(), CV_8UC4, GetLaneColor(req.lanes[req.deviate_index].lane_type, true));
        rectMat.copyTo(roi);
    }

    cv::cvtColor(img, img, CV_BGRA2BGR);

    DrawLanesProperty(img, req.lanes);

    // lane width
    if ((req.width >= 0.0) && req.width < 600 ) {
        sprintf(m_str_buffer, "%.0f", round(req.width));
        DrawTextInCenter(img, LANE_WIDTH_INNER_RECT, 0.8, 1, m_str_buffer);
    }

    // left dis
    if (fabs(req.left_dis) < LANE_DIS_LIMIT) {
        sprintf(m_str_buffer, "%.0f", round(req.left_dis));
        DrawTextInCenter(img, LANE_DIS_LEFT_RECT, 0.8, 1, m_str_buffer);
    }

    // right dis
    if (fabs(req.right_dis) < LANE_DIS_LIMIT) {
        sprintf(m_str_buffer, "%.0f", round(req.right_dis));
        DrawTextInCenter(img, LANE_DIS_RIGHT_RECT, 0.8, 1, m_str_buffer);
    }

    // alert
    if (show_hit) {
        DrawTextInCenter(img, show_deviate ? LANE_HIT_ALERT_RECT : LANE_ALERT_RECT, 0.8, 1, "Collission Alert");
    }
    if (show_deviate) {
        if (req.deviate == ENUM_DEVIATE_LEFT)
            DrawTextInCenter(img, show_hit ? LANE_DEVIATE_ALERT_RECT : LANE_ALERT_RECT, 0.8, 1, "Left Deviate");
        else
            DrawTextInCenter(img, show_hit ? LANE_DEVIATE_ALERT_RECT : LANE_ALERT_RECT, 0.8, 1, "Right Deviate");
    }

    DrawText(img, SPEED_UNIT_POS, 0.6, 1, "KM/H");
    //sprintf(m_str_buffer, "%.6lf", req.latitude);
    //DrawText(img, LATITUDE_POS, 0.5, 1, m_str_buffer);
    //sprintf(m_str_buffer, "%.6lf", req.longitude);
    //DrawText(img, LONGITUDE_POS, 0.5, 1, m_str_buffer);
}

void LaneUI::Draw(const DrawLaneRequest *req, DrawLaneResponse *rsp) {
    DrawLaneScene(req, rsp);

    if (req->save) {
        std::string save_file = req->dir_name;
//        DrawText(rsp->img, cv::Point(25, 50), 1.0, 1, req->image_name.c_str(), CV_RGB(255, 0, 0));
        std::string image_name = req->image_name.substr(0, req->image_name.length() - 4) + ".jpg";
#ifndef __gnu_linux__
        cv::imwrite(save_file + "\\" + image_name, rsp->img);
#else
        cv::imwrite(save_file + "/" + image_name, rsp->img);
#endif
    }
}
