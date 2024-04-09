#ifndef  LANE_SIMPLE_UI_H
#define  LANE_SIMPLE_UI_H
#include <vector>
#include "opencv2/opencv.hpp"

// @brief this class if mainly for lane draw simply,
//        without logo, just print some usefull information
// ----------- struct for interface ---------------
enum ViewMode
{
    kModeBirdView = 1,
    kModePerspectiveView,
};

struct LaneSimpleDrawReq
{
    ViewMode mode; // view mode

    // when mode == kModeBirdView, the following parameters must be given
    float lateral_range_start; // coordinate for lateral scope's start
    float lateral_range_end;   // corrdinate for lateral scope's end
    float lateral_res;         // lateral's resolution
    float longitudianl_range_start; // coordinate for longitudianl scope's start
    float longitudianl_range_end; // coordinate for longitudianl scope's end
    float longitudianl_res;       // longitudianl resolution
    std::vector<std::vector<float> > bird_view_poly_coeffs; // poly coeff

    // when mode == kModePerspectiveView, the following parameters must be given
    cv::Mat input_image; // origin image, this image could be obtained from camera directly
    std::vector<std::vector<float> > perspective_view_poly_coeffs; // poly coeff
    cv::Point vanishing_pt; // vanishing pt
    cv::Scalar color;
};

struct LaneSimpleDrawRsp
{
    cv::Mat drawed_image;
};

// ----------- struct for interface ---------------
class LaneSimpleUI
{
public:
    LaneSimpleUI() {}

    ~LaneSimpleUI() {}

    void Draw(LaneSimpleDrawReq* req, LaneSimpleDrawRsp* rsp);

private:
    void GetBirdViewImage(LaneSimpleDrawReq* req, cv::Mat* out_image);
    void GetPerspectiveViewImage(LaneSimpleDrawReq* req, cv::Mat* out_image);
    bool InvalidPixel(const cv::Mat& image, const cv::Point& pt);

private:
};



#endif  // LANE_SIMPLE_UI_H
