#include <math.h>
#include <vector>
#include "opencv2/opencv.hpp"
#include "common/base/stdint.h"
#include "common/math/polyfit.hpp"
#include "gtest/gtest.h"

class PolyFitTest : public ::testing::Test
{
public:
    void Draw(cv::Mat* image, const std::vector<double>& x,
            const std::vector<double>& y,
            const cv::Scalar& color)
    {
        cv::Point2f pt1;
        pt1.x = y[0];
        pt1.y = x[0];
        for (size_t i = 1; i < x.size(); ++i) {
            cv::Point2f pt2;
            pt2.x = y[i];
            pt2.y = x[i];
            cv::line(*image, pt1, pt2, color, 1);
            pt1 = pt2;
        }
    }
private:
};

TEST_F(PolyFitTest, PolyTest)
{
    int rows = 600;
    int cols = 600;
    cv::Mat canvas(rows, rows, CV_8UC3);
    std::vector<double> x(rows);
    std::vector<double> y(rows);
    srand(time(NULL));
    for (int i = 0; i < rows; ++i) {
        x[i] = i;
        y[i] = 100 + 2 * x[i] + (-1.0 / 300) * pow(x[i], 2);

    }
    Draw(&canvas, x, y, cv::Scalar(0, 0, 255));
    cv::imshow("polyfit", canvas);
    cv::waitKey(-1);

    std::vector<double> coeffs = math::polyfit(x, y, 3);

    std::vector<double> xx(rows);
    std::vector<double> yy(rows);
    for (int i = 0; i < rows; ++i) {
        xx[i] = i;
        yy[i] = coeffs[0] + coeffs[1] * xx[i] + coeffs[2] * pow(xx[i], 2);
    }
    Draw(&canvas, xx, yy, cv::Scalar(255, 0, 0));

    cv::imshow("polyfit", canvas);
    cv::waitKey(-1);
}
