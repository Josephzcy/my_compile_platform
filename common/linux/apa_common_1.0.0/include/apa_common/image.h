
/// @file image.h
/// @brief
/// @author Devin (devin@minieye.cc)
/// @date 2017-01-18
/// Copyright (C) 2017 - MiniEye INC.

#pragma once
#include <string>
#include <typeinfo>
#include "opencv2/opencv.hpp"
#include "mem.h"

enum ImageType {
    kImageRotated = 0,
};

class Image {
 public:
    Image() : m_buff(NULL), m_buff_size(0) {}

    virtual ~Image();
    Image(int rows, int cols, int type);
    explicit Image(const std::string& image_name);
    const Image& operator=(const Image& lhs);
    void Reset(int rows, int cols, int type);

    void GetMat(cv::Mat* mat);
    const cv::Mat* GetMat() const;

    int Rows() const;
    int Cols() const;
    int Type() const;
    int Channels() const;

    // accessor
    template <typename T>
    T* Ptr(int y, int x, int channel = 0);

    template <typename T>
    T At(int y, int x, int channel = 0) const;

    void SwapOut(void** buff, uint32_t* buff_size, cv::Mat* mat);
    virtual void Swap(Image* lhs);

    virtual Image* Resize(float scale_ratio);

    void Show(const std::string& name, bool waited = false);

 private:
    void Clear();

 private:
    void* m_buff;
    uint32_t m_buff_size;
    cv::Mat m_data_mat;
};

template <typename T>
T* Image::Ptr(int y, int x, int channel) {
    // typeid(T).name() not match m_data_mat.type()
    if (sizeof(T) != m_data_mat.elemSize1()) {
        return NULL;
    }
    return m_data_mat.ptr<T>(y, x) + channel;
}

template <typename T>
T Image::At(int y, int x, int channel) const {
    return *(m_data_mat.ptr<T>(y, x) + channel);
}
