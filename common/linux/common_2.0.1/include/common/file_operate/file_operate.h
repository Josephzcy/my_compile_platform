// Copyright(c) 2015 MiniEye INC. All Rights Reserved.
// author: yexi
// email: lucien@minieye.cc
// date: 2015.7.27

#ifndef COMMON_SYSTEM_FILE_OPERATE_H_
#define COMMON_SYSTEM_FILE_OPERATE_H_

#include <string>
#include "opencv2/opencv.hpp"
#include "common/base/stdint.h"


class FileOperate {
 public:
    FileOperate() {
        m_save_switch = 0;
    }

    bool SaveImage(const std::string& image_name, const cv::Mat& image);
    bool FindSavePath();
    void SetSwitchOn(int state);
    int GetSwitchState();

    // @brief 删除该目录下所有的文件和子目录
    void RemoveDir(const std::string& dir);
    int MakeDir(const std::string& dir);

 private:
    std::string m_image_save_path;
    int m_save_switch;
    uint32_t m_save_frame_no;
};

#endif  // COMMON_SYSTEM_FILE_OPERATE_H_
