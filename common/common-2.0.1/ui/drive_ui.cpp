/// Copyright (C) 2015 - MiniEye INC.

#include <stdio.h>
#include "common/string/string_number.h"
#include "common/base/log_level.h"
#include "common/ui/drive_ui.h"


const cv::Point DriveUI::SPEED_NUM_POS = cv::Point(266, 648);
const cv::Point DriveUI::SPEED_UNIT_POS = cv::Point(272, 646);
const cv::Point DriveUI::LATITUDE_POS = cv::Point(1056, 622);
const cv::Point DriveUI::LONGITUDE_POS = cv::Point(1056, 646);

const CvScalar DriveUI::CAR_RECT_COLOR = cvScalar(252, 239, 0);
const CvScalar DriveUI::CAR_LINE_COLOR = cvScalar(255, 250, 158);
const CvScalar DriveUI::HIT_CAR_RECT_COLOR = cvScalar(64, 55, 240);
const CvScalar DriveUI::HIT_CAR_LINE_COLOR = cvScalar(64, 55, 240);
const CvScalar DriveUI::HIT_CAR_DIS_CLOSE_COLOR = cvScalar(33, 238, 241);
const CvScalar DriveUI::HIT_CAR_DIS_WARN_COLOR = cvScalar(64, 55, 240);

const CvScalar DriveUI::LANE_COLOR_HIT = cvScalar(47, 41, 152);
const cv::Rect DriveUI::LANE_ALERT_RECT = cv::Rect(410, 600, 460, 60);


void DriveUI::Init() {
#if defined(ANDROID) || defined(__gnu_linux__)
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
    // locatin
    for (int i = 0; i < LOCATION_SIZE; ++i) {
        sprintf(m_str_buffer, (image_dir_path + "/location/%d.png").c_str(), i);
        m_location_mat[i] = cv::imread(m_str_buffer, CV_LOAD_IMAGE_UNCHANGED);
    }
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

    m_danger_mat = cv::imread(image_dir_path + "/hit/danger.png", CV_LOAD_IMAGE_UNCHANGED);

    m_last_hit_type = kHitAlertNone;
    m_beep_cnt = 0;
}

void DriveUI::BlendMat(cv::Mat &mat1, cv::Mat &mat2, cv::Mat &dst) {
    CV_Assert(mat1.channels() == 4);
    CV_Assert(mat2.channels() == 4);
    CV_Assert(dst.channels() == 4);

    cv::Mat_<cv::Vec4b> _mat1 = mat1;
    cv::Mat_<cv::Vec4b> _mat2 = mat2;
    cv::Mat_<cv::Vec4b> _dst = dst;

    for (int i = 0; i < mat1.rows; ++i)
        for (int j = 0; j < mat1.cols; ++j) {
            double alpha = _mat2(i, j)[3];
            _dst(i, j)[0] = (uchar) (_mat1(i, j)[0] * (1 - alpha / 255) + _mat2(i, j)[0] * (alpha / 255));
            _dst(i, j)[1] = (uchar) (_mat1(i, j)[1] * (1 - alpha / 255) + _mat2(i, j)[1] * (alpha / 255));
            _dst(i, j)[2] = (uchar) (_mat1(i, j)[2] * (1 - alpha / 255) + _mat2(i, j)[2] * (alpha / 255));
            _dst(i, j)[3] = 0;
        }
    dst = _dst;
}

//The background is in BGR colour space.
//The foreground is in BGRA colour space.
void DriveUI::OverlayImage(const cv::Mat &background, const cv::Mat &foreground, cv::Mat &output, cv::Point2i location,
                           double alpha) {
    background.copyTo(output);

    // start at the row indicated by location, or at row 0 if location.y is negative.
    for (int y = std::max(location.y, 0); y < background.rows; ++y) {
        int fy = y - location.y; // because of the translation

        // we are done of we have processed all rows of the foreground image.
        if (fy >= foreground.rows)
            break;

        // start at the column indicated by location,

        // or at column 0 if location.x is negative.
        for (int x = std::max(location.x, 0); x < background.cols; ++x) {
            int fx = x - location.x; // because of the translation.

            // we are done with this row if the column is outside of the foreground image.
            if (fx >= foreground.cols)
                break;

            // determine the opacity of the foregrond pixel, using its fourth (alpha) channel.
            double opacity =
                    ((double) foreground.data[fy * foreground.step + fx * foreground.channels() + 3]) / 255. * alpha;

            // and now combine the background and foreground pixel, using the opacity,

            // but only if opacity > 0.
            for (int c = 0; opacity > 0 && c < output.channels(); ++c) {
                unsigned char foreground_px = foreground.data[fy * foreground.step + fx * foreground.channels() + c];
                unsigned char background_px = background.data[y * background.step + x * background.channels() + c];
                output.data[y * output.step + output.channels() * x + c] = (uchar) (
                        background_px * (1. - opacity) + foreground_px * opacity);
            }
        }
    }
}

void DriveUI::DrawText(cv::Mat &mat, const cv::Point &pos, double size, int thickness, const char *str,
                       const cv::Scalar &color) {
    cv::putText(mat, str, pos, CV_FONT_HERSHEY_DUPLEX, size, color, thickness, CV_AA);
}

void DriveUI::DrawTextInCenter(cv::Mat &mat, const cv::Rect &rect, double size, int thickness, const char *str,
                               const cv::Scalar &color) {
    int base_line;
    cv::Size text_size = cv::getTextSize(str, CV_FONT_HERSHEY_DUPLEX, size, thickness, &base_line);
    int x = rect.x + (rect.width - text_size.width) / 2;
    int y = rect.y + (rect.height + text_size.height) / 2;
    cv::putText(mat, str, cv::Point(x, y), CV_FONT_HERSHEY_DUPLEX, size, color, thickness, CV_AA);
}

void DriveUI::DrawNum(cv::Mat &mat, const cv::Point &pos, const int num, bool left2right) {
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

void DriveUI::DrawBigCarRect(cv::Mat &mat, const cv::Rect &rect, const cv::Scalar &rect_color,
                             const cv::Scalar &line_color) {
    int thickness = 1;
    int line_type = CV_AA;

    int x = rect.x;
    int y = rect.y;
    int width = rect.width;
    int height = rect.height;
    cv::Point br = rect.br() - cv::Point(1, 1);

    cv::Rect roi_rc = cv::Rect(x + thickness, y + thickness, width - 2 * thickness, height - 2 * thickness);
    if (roi_rc.x + roi_rc.width >= mat.cols)
        roi_rc.width = mat.cols - 1 - roi_rc.x;
    if (roi_rc.y + roi_rc.height >= mat.rows)
        roi_rc.height = mat.rows - 1 - roi_rc.y;
    cv::Mat roi = mat(roi_rc);
    cv::Mat rect_mat(roi.size(), CV_8UC3, rect_color);
    double alpha = 0.1;
    cv::addWeighted(rect_mat, alpha, roi, 1.0 - alpha, 0.0, roi);

    int line_len = std::min(width, height) / 2;
    cv::line(mat, rect.tl(), rect.tl() + cv::Point(10, 0), line_color, thickness, line_type);
    cv::line(mat, rect.tl(), rect.tl() + cv::Point(0, 10), line_color, thickness, line_type);
    cv::line(mat, cv::Point(x + width - 1, y), cv::Point(x + width - 11, y), line_color, thickness, line_type);
    cv::line(mat, cv::Point(x + width - 1, y), cv::Point(x + width - 1, y + 10), line_color, thickness, line_type);
    cv::line(mat, cv::Point(x, y + height - 1), cv::Point(x, y + height - 11), line_color, thickness, line_type);
    cv::line(mat, cv::Point(x, y + height - 1), cv::Point(x + 10, y + height - 1), line_color, thickness, line_type);
    cv::line(mat, br, br - cv::Point(line_len, 0), line_color, thickness, line_type);
    cv::line(mat, br, br - cv::Point(0, line_len), line_color, thickness, line_type);

    // blue rect
    cv::rectangle(mat, rect.tl(), rect.tl() + cv::Point(thickness + 6, thickness + 6), line_color, -1);

    //// pos
    //sprintf(m_str_buffer, "(%d,%d)", rect.x, rect.y);
    //DrawText(mat, cv::Point(rect.x + 10, rect.y + 20), 0.5, 1, m_str_buffer);
}

void DriveUI::DrawSmallCarRect(cv::Mat &mat, const cv::Rect &rect, const cv::Scalar &color) {
    cv::rectangle(mat, rect.tl(), rect.br(), color, 1);
}

//#ifdef _WIN32
//#include <windows.h>
//#endif

void DriveUI::DoBeep() {
#ifdef _WIN32
    //Beep(523, 500); // 523 hertz (C5) for 500 milliseconds
    std::cout<<'\a';
#else
#endif
}

void DriveUI::DrawUI(cv::Mat &mat, double speed) {
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
    int speed_index = (int) round(speed / SPEED_MAX * SPEED_SIZE);
    speed_index = std::min(speed_index, SPEED_SIZE - 1);
    cv::Mat image_speed = mat(cv::Rect(100, 600, m_speed_mat[speed_index].cols, m_speed_mat[speed_index].rows));
    BlendMat(image_speed, m_speed_mat[speed_index], image_speed);

    // location
    static int location_index = 0;
    location_index %= LOCATION_SIZE;
    cv::Mat image_location = mat(
            cv::Rect(980, 600, m_location_mat[location_index].cols, m_location_mat[location_index].rows));
    BlendMat(image_location, m_location_mat[location_index], image_location);
    ++location_index;

    DrawNum(mat, SPEED_NUM_POS, (int) (round(speed)), false);

}

void DriveUI::DrawScene(const DrawRequest *_request, DrawResponse *_response) {
    const DrawRequest &req = *_request;
    DrawResponse &rsp = *_response;

    rsp.image = req.image;
    cv::Mat &img = rsp.image;
    cv::cvtColor(img, img, CV_BGR2BGRA);
    DrawUI(img, req.speed);

    // hit alert car
    bool show_hit = (req.hit_type == kHitAlertClose || req.hit_type == kHitAlertWarn ||
                     req.hit_type == kHitAlertDanger);
    if (show_hit) {
        cv::Mat roi = img(LANE_ALERT_RECT);
        cv::Mat rectMat = cv::Mat(roi.size(), CV_8UC4, LANE_COLOR_HIT);
        rectMat.copyTo(roi);
    }

    cv::cvtColor(img, img, CV_BGRA2BGR);

    //sprintf(m_str_buffer, "%d", speed);
    //DrawText(img, SPEED_NUM_POS, 1.6, 2, m_str_buffer);
    DrawText(img, SPEED_UNIT_POS, 0.6, 1, "KM/H");
    if (fabs(req.latitude) >= LOCATION_VALUE_LIMIT || fabs(req.longitude) >= LOCATION_VALUE_LIMIT) {
        DrawText(img, LATITUDE_POS, 0.5, 1, "----");
        DrawText(img, LONGITUDE_POS, 0.5, 1, "----");
    } else {
        sprintf(m_str_buffer, "%.6lf", req.latitude);
        DrawText(img, LATITUDE_POS, 0.5, 1, m_str_buffer);
        sprintf(m_str_buffer, "%.6lf", req.longitude);
        DrawText(img, LONGITUDE_POS, 0.5, 1, m_str_buffer);
    }

    // hit alert
    if (show_hit) {
        DrawTextInCenter(img, LANE_ALERT_RECT, 0.8, 1, "Collission Alert");
    }

    bool b_hit = false;
    int length = (int) req.rects.size();
    for (int i = 0; i < length; ++i) {
        if (i == req.hit_car_index && show_hit) {
            // hit alert car
            const cv::Rect &b_rect = req.rects[i].b_rect;

            // red rect
            DrawBigCarRect(img, b_rect, HIT_CAR_RECT_COLOR, HIT_CAR_LINE_COLOR);

            double size_factor = std::min(b_rect.width, b_rect.height) / 200.0;

            // dis
            CvScalar dis_color = HIT_CAR_DIS_WARN_COLOR;
            if (req.hit_type == kHitAlertClose)
                dis_color = HIT_CAR_DIS_CLOSE_COLOR;
            sprintf(m_str_buffer, "%.1lfm", req.hit_car_dis);
            DrawText(img, cv::Point(b_rect.x + 3, b_rect.y + b_rect.height - 5), 0.6 * size_factor, 1, m_str_buffer,
                     dis_color);

            // beep
            bool b_beep = false;
            if (req.hit_type == kHitAlertWarn) {
                if (m_last_hit_type != kHitAlertWarn)
                    m_beep_cnt = 0;
                b_beep = m_beep_cnt <= 0;
            } else if (req.hit_type == kHitAlertDanger) {
                if (m_last_hit_type != kHitAlertDanger)
                    m_beep_cnt = 0;
                b_beep = true;
            } else {
                m_beep_cnt = 0;
            }
            if (b_beep) {
                DoBeep();
                ++m_beep_cnt;
            }

            if (req.hit_type == kHitAlertDanger) {
                // danger image
                int width = m_danger_mat.size().width;
                int x = b_rect.x + (b_rect.width - width) / 2;
                int y = b_rect.y + (b_rect.height - width) / 2;
                //cv::Mat img_mat = img(cv::Rect(x, y, width, width));
                //cv::addWeighted(img_mat, 0.2, m_danger_mat, 1.0, 0, img_mat);
                OverlayImage(img, m_danger_mat, img, cv::Point2i(x, y), m_beep_cnt & 1 ? 1.0 : 0.5);
            } else {
                // time
                sprintf(m_str_buffer, "%.1lf", req.hit_car_time);
                DrawTextInCenter(img, b_rect, 2.0 * size_factor, 2, m_str_buffer, dis_color);
            }

            b_hit = true;
        } else {
            const cv::Rect &b_rect = req.rects[i].b_rect;
            const cv::Rect &rect = req.rects[i].rect;
            if (req.show_type == kShowBig || req.show_type == kShowAll)
                DrawBigCarRect(img, b_rect, CAR_RECT_COLOR, CAR_LINE_COLOR);
            if (req.show_type == kShowSmall || req.show_type == kShowAll)
                DrawSmallCarRect(img, rect);
            if (!req.rects[i].classify_type.empty()) {
                cv::Rect draw_rect = req.show_type == kShowBig ? b_rect : rect;
                DrawTextInCenter(img, draw_rect, 0.4, 1, req.rects[i].classify_type.c_str());
            }
        }
    }

    if (b_hit) {
        m_last_hit_type = req.hit_type;
    } else {
        m_last_hit_type = kHitAlertNone;
        m_beep_cnt = 0;
    }
}

void DriveUI::Draw(const DrawRequest *req, DrawResponse *rsp) {
    DrawScene(req, rsp);

    if (req->save) {
        std::string save_file = req->dir_name;
        DrawText(rsp->image, cvPoint(25, 50), 1.0, 1, req->image_name.c_str(), CV_RGB(255, 0, 0));
        cv::imwrite(save_file + "\\" + req->image_name.substr(0, req->image_name.length() - 4) + ".jpg",
                    rsp->image);
    }
}


