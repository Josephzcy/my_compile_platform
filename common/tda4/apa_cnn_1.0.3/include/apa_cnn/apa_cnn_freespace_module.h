#ifndef APA_CNN_FREESPACE_MODULE_H
#define APA_CNN_FREESPACE_MODULE_H

#include <opencv2/opencv.hpp>
#include "apa_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

namespace cnn_freespace
{

#if defined(TDA4)
    void ApaCnnFreespaceInit();
    int runSingleFrame(uint64_t src_y, uint64_t src_uv, int frameid, std::vector<std::vector<cv::Point2f> >& freespacePoints);
    void checkContourPoints(cv::Mat& originImage, std::vector<std::vector<cv::Point2f> >& freespacePoints,int frameId);
#else
    void ApaCnnFreespaceInit(int pipeline_num);
    int runSingleFrame(cv::Mat& image, int frameid, std::vector<std::vector<cv::Point2f> >& freespacePoints);
#endif
}


#ifdef __cplusplus
};
#endif

#endif
