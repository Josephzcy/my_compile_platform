// Copyright(c) 2015 MiniEye INC. All Rights Reserved.
// author: yexi
// email: lucien@minieye.cc
// date: 2015.7.27

#if defined(__gnu_linux__) || defined(ANDROID)
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#elif defined(_WIN32)
#include <direct.h>
#include <io.h>
#endif

#include <time.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>

#include "common/file_operate/file_operate.h"
#include "common/system/system_information.h"
#include "common/string/algorithm.h"
#include "common/base/log_level.h"

#define MAX_PATH_LEN 256

#if defined(_WIN32)
#define ACCESS(file_name, access_mode) _access(file_name, access_mode)
#define MKDIR(path) _mkdir(path)
#else
#define ACCESS(file_name, access_mode) access(file_name, access_mode)
#define MKDIR(path) mkdir(path, S_IRWXU | S_IRWXG)
#endif

// Return all entry names under a directory.
std::vector<std::string> GetDirents(const std::string& dirpath) {
    std::vector<std::string> entries;

    DIR* dp = opendir(dirpath.c_str());
    if (dp == NULL) {
        return entries;
    }

    while (true) {
        struct dirent* entry = readdir(dp);
        if (entry == NULL) {
            break;
        }

        const char* name = entry->d_name;
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
            continue;
        }

        entries.push_back(name);
    }

    closedir(dp);
    return entries;
}

// Return values:
//   -1: error
//    0: nonexist
//    1: directory
//    2: file (including symlinks, pipes...)
int GetPathType(const std::string& path) {
    struct stat st;
    int retval = lstat(path.c_str(), &st);
    if (retval == -1) {
        int errsv = errno;
        if (errsv != ENOENT) {
            std::cerr << "Error: cannot read the stat of '" << path
                      << "': " << strerror(errsv) << std::endl;
            return -1;
        } else {
            return 0;
        }
    }

    if (S_ISDIR(st.st_mode)) {
        return 1;
    } else  {
        return 2;
    }
}

// Remove a file or directory if it exists.
// Return true on success, false on failure.
bool RemovePathIfExists(const std::string& path, bool is_dir) {
    int retval = is_dir ? rmdir(path.c_str()) : remove(path.c_str());
    if (retval == -1) {
        int errsv = errno;
        if (errsv != ENOENT) {
            std::cerr << "Error: cannot remove path '" << path
                      << "': " << strerror(errsv) << std::endl;
            return false;
        }
    }

    return true;
}

static std::string GetImageSaveFolder() {
    time_t temp_time;
    time(&temp_time);
    struct tm current_time;
    gmtime_r(&temp_time, &current_time);

    std::stringstream ss;
    ss << current_time.tm_year << "-"
       << current_time.tm_mon  << "-"
       << current_time.tm_mday << "-"
       << current_time.tm_hour << "-"
       << current_time.tm_min  << "-"
       << current_time.tm_sec;
    return ss.str();
}

bool FileOperate::FindSavePath() {
    std::string result_path;

#if defined(__gnu_linux__) || defined(ANDROID)
    std::string dir = "/media/" + GetUserName();
    if (GetPathType(dir) != 2) {  // if not a directory
        dir = ".";
    }

    VLOG(DEBUG_LOG) << "dir = " << dir;
    if (chdir(dir.c_str()) != 0) {
        VLOG(DEBUG_LOG) << "chdir error: " << strerror(errno);
        return false;
    }

    std::vector<std::string> entries = GetDirents(dir);
    if (entries.size() == 0) {  // 找不到sd文件夹
        return false;
    }

    result_path = dir + "/" + entries[0] + "/" + GetImageSaveFolder();
    mode_t mode = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
    if (mkdir(result_path.c_str(), mode) == -1) {
        VLOG(DEBUG_LOG) << "can't create directory";
        return false;
    }
#elif defined(_WIN32)
    // minieye_01上的路径为准
    result_path = "D:/" + GetImageSaveFolder();
    if (_mkdir(result_path.str().c_str()) == -1) {
        VLOG(DEBUG_LOG) << "can't create directory";
        return false;
    }
#endif

    m_image_save_path = result_path + "/";
    return true;
}

bool FileOperate::SaveImage(const std::string& image_name,
                            const cv::Mat& image) {
    if (m_image_save_path.empty()) {
        return false;
    }

    char temp_str[10];
    snprintf(temp_str, sizeof(temp_str), "%09d", m_save_frame_no);
    std::string index_path = temp_str;
    index_path = m_image_save_path + image_name + "_" + index_path + ".jpg";
    if (!cv::imwrite(index_path, image)) {
        VLOG(DEBUG_LOG) << image_name << "  image save failed";
        return false;
    }

    ++m_save_frame_no;
    return true;
}

void FileOperate::SetSwitchOn(int state) {
    m_save_switch = state;
    m_save_frame_no = 0;
}

int FileOperate::GetSwitchState() {
    return m_save_switch;
}

// Recursively remove a directory.
void FileOperate::RemoveDir(const std::string& dir) {
    std::vector<std::string> entries = GetDirents(dir);
    for (const std::string& entry_name : entries) {
        const std::string entry_path = dir + "/" + entry_name;
        int path_type = GetPathType(entry_path);
        if (path_type < 1) {  // nonexist or error
            continue;
        } else if (path_type == 1) {  // directory
            RemoveDir(entry_path);
        } else {  // file
            RemovePathIfExists(entry_path, false);
        }
    }

    RemovePathIfExists(dir, true);
}

int FileOperate::MakeDir(const std::string& dir) 
{
    uint32_t dir_path_len = dir.length();
    if (dir_path_len > MAX_PATH_LEN) {
        return -1;
    }
    std::string tmp_path = "";
    for (uint32_t i = 0; i < dir.length(); ++i) {
        tmp_path += dir.at(i);
        if (tmp_path.back() == '\\' || tmp_path.back() == '/') {
            if (ACCESS(tmp_path.c_str(), 0) != 0) {
                int ret = MKDIR(tmp_path.c_str());
                if (ret != 0)
                    return ret;
            }
        }
    }
    return 0;
}
