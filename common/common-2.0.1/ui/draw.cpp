#ifdef WIN32
#include "common/base/stdint.h"
#endif
#include "common/base/log_level.h"
#include "draw.h"

void DrawBigCarRect(cv::Mat&mat,
        const cv::Rect &rect,
        const cv::Scalar &rect_color,
        const cv::Scalar &line_color)
{
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
}

void VehicleDraw(const DrawRequest* req, DrawResponse* rsp)
{
    rsp->image = req->image;
    cv::Mat& img = rsp->image;
    int length = (int)(req->rects).size();
    for (int i = 0; i < length; ++i) {
         const cv::Rect& b_rect = (req->rects)[i].b_rect;
         DrawBigCarRect(img, b_rect, cv::Scalar(64, 55, 240), cv::Scalar(64, 55, 240));
    }
}

void DrawDottedLine(cv::Mat &img, const cv::Point &pt1, const cv::Point &pt2, cv::Scalar color,
        int thickness, int lenghOfDots, int lineType, int leftToRight)
{
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

void LaneDraw(const DrawLaneRequest* req, DrawLaneResponse* rsp)
{
    rsp->img = req->img;
    cv::Mat& img = rsp->img;

    const std::vector<LaneDataType>& lanes = req->lanes;

    for (uint32_t i = 0; i < lanes.size(); ++i) {
        if (lanes[i].lane_type == ENUM_LANE_SOLID) {
            cv::line(img, lanes[i].pos_top, lanes[i].pos_bottom, cv::Scalar(108, 248, 59), 3, 8);
        } else {
            DrawDottedLine(img, lanes[i].pos_top, lanes[i].pos_bottom, cv::Scalar(108, 248, 59), 2, 20, 8, 0);
        }
    }
    if (req->save) {

        std::string save_file = req->dir_name;
        std::string image_name = req->image_name.substr(0, req->image_name.length() - 4) + ".jpg";
        cv::imwrite(save_file + "/" + image_name, rsp->img);
        LOG(ERROR) << "save image : " << save_file + "/" + image_name;
    }
}
