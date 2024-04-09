#include <iostream>
#include "common/ui/lane_simple_ui.h"

void LaneSimpleUI::Draw(LaneSimpleDrawReq* req, LaneSimpleDrawRsp* rsp)
{
    if (req->mode == kModeBirdView) {
        // + get bird view image
        GetBirdViewImage(req, &(rsp->drawed_image));
    } else {
        GetPerspectiveViewImage(req, &(rsp->drawed_image));
    }
}

void LaneSimpleUI::GetBirdViewImage(LaneSimpleDrawReq* req, cv::Mat* out_image)
{
    int cols = (req->lateral_range_end - req->lateral_range_start) / req->lateral_res;
    int rows = (req->longitudianl_range_end - req->longitudianl_range_start) / req->longitudianl_res;
    if (req->input_image.cols == 0 || req->input_image.rows == 0) {
        *out_image = cv::Mat::zeros(rows, cols, CV_8UC3);
    } else {
        if (cols != req->input_image.cols || rows != req->input_image.rows) {
            std::cerr << "size not match, use default image instead" << std::endl;
            *out_image = cv::Mat::zeros(rows, cols, CV_8UC3);
        } else {
            *out_image = req->input_image;
        }
    }

    cv::Point prev_pt, curr_pt;
    cv::Scalar color = req->color;
    for (size_t i = 0; i < req->bird_view_poly_coeffs.size(); ++i) {
        const std::vector<float>& poly_coeff = req->bird_view_poly_coeffs[i];
        // birdview 使用的是绝对距离
        bool has_prev = false;
        for (float long_dist = req->longitudianl_range_start;
                long_dist < req->longitudianl_range_end;
                long_dist += req->longitudianl_res) {
            float x_dist = 0;
            for (size_t j = 0; j < poly_coeff.size(); ++j) {
                x_dist += pow(long_dist, j) * poly_coeff[j];
            }
            float dist_gap = long_dist - req->longitudianl_range_start;
            int y_pixel = static_cast<int>(dist_gap / req->longitudianl_res);
            int x_pixel = static_cast<int>(x_dist / req->lateral_res);
            curr_pt = cv::Point(x_pixel + cols/2, rows - 1 - y_pixel);
            if (InvalidPixel(*out_image, curr_pt)) {
                has_prev = false;
                continue;
            }
            if (has_prev) {
                cv::line(*out_image, prev_pt, curr_pt, color, 2);
            }
            prev_pt = curr_pt;
            has_prev = true;
        }
    }
}

void LaneSimpleUI::GetPerspectiveViewImage(LaneSimpleDrawReq* req, cv::Mat* out_image)
{
    if (req->input_image.cols == 0 || req->input_image.rows == 0) {
        std::cerr << "input image (perspective image) is null";
        return;
    }
    *out_image = req->input_image;

    int start_row = out_image->rows * 1.5 / 4;
    if (req->vanishing_pt.y > 0)
        start_row = req->vanishing_pt.y + 30;

    int end_row = out_image->rows * 3.0 / 4;

    // draw vanishing pt
    int left = req->vanishing_pt.x - 8;
    left = (left >= 0 ? left : 0);
    int right = req->vanishing_pt.x + 8;
    right = (right < out_image->cols? right : out_image->cols - 1);
    cv::Point beg_pt(left, req->vanishing_pt.y);
    cv::Point end_pt(right, req->vanishing_pt.y);
    cv::line(*out_image, beg_pt, end_pt, cv::Scalar(0, 0, 255), 4);
    int top = req->vanishing_pt.y - 8;
    int bottom = req->vanishing_pt.y + 8;
    top = (top >= 0 ? top : 0);
    bottom = (bottom < out_image->rows ? bottom : out_image->rows - 1);
    beg_pt = cv::Point(req->vanishing_pt.x, top);
    end_pt = cv::Point(req->vanishing_pt.x, bottom);
    cv::line(*out_image, beg_pt, end_pt, cv::Scalar(0, 0, 255), 4);

    // draw curve
    cv::Point prev_pt, curr_pt;
    cv::Scalar color = req->color;
    for (size_t i = 0; i < req->perspective_view_poly_coeffs.size(); ++i) {
        const std::vector<float>& poly_coeff = req->perspective_view_poly_coeffs[i];
        // perspective view 使用的是像素坐标
        bool has_prev = false;
        for (int row = start_row; row < end_row; ++row) {
            float x_dist = 0;
            for (size_t j = 0; j < poly_coeff.size(); ++j) {
                x_dist += pow(row, j) * poly_coeff[j];
            }
            int col = static_cast<int>(x_dist);
            curr_pt = cv::Point(col, row);
            if (InvalidPixel(*out_image, curr_pt)) {
                has_prev = false;
                continue;
            }
            if (has_prev) {
                cv::line(*out_image, prev_pt, curr_pt, color, 2);
            }
            prev_pt = curr_pt;
            has_prev = true;
        }
        // TODO(Devin): draw other property
    }
}

bool LaneSimpleUI::InvalidPixel(const cv::Mat& image, const cv::Point& pt)
{
    if (pt.x < 0 || pt.x >= image.cols ||
        pt.y < 0 || pt.y >= image.rows)
        return true;
    return false;
}
