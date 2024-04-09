/// @file median_filter.h
/// @brief A simple median filter.
/// @author zhangqi (zhangqi@minieye.cc)
/// @date 2016-10-28
/// Copyright (C) 2016 - MiniEye INC.

#ifndef COMMON_FILTER_MEDIAN_FILTER_H_
#define COMMON_FILTER_MEDIAN_FILTER_H_

#include <sstream>
#include <deque>
#include <string>
#include <vector>
#include <algorithm>

template<class T>
class MedianFilter {
 public:
    MedianFilter<T>(size_t size) : m_size(size) {}

    // Push an element into the buffer.
    void Set(const T &val) {
        m_hist_values.push_back(val);
        if (m_hist_values.size() > m_size) {
            m_hist_values.pop_front();
        }
    }

    // Return the last element of the buffer.
    // Don't call this method before a call of Set.
    T GetLast() const {
        return *(m_hist_values.end() - 1);
    }

    // Return the median value of the buffer.
    // Don't call this method before a call of Set.
    T GetMedian() const {
        return GetMedian(m_size);
    }

    // Return the median value of the last cnt elements.
    // Don't call this method before a call of Set.
    T GetMedian(size_t cnt) const {
        size_t n = std::min(m_hist_values.size(), cnt);
        std::vector<T> samples(m_hist_values.end() - n, m_hist_values.end());
        std::sort(samples.begin(), samples.end());
        return samples[n / 2];
    }

    // Set the current value and get the median value.
    T Filter(const T &val) {
        Set(val);
        return GetMedian();
    }

    std::string dump() const {
        std::stringstream ss;
        for (size_t i = 0; i < m_hist_values.size(); ++i) {
            if (i > 0) {
                ss << ", ";
            }
            ss << m_hist_values[i];
        }
        return ss.str();
    }

 private:
    size_t m_size;
    std::deque<T> m_hist_values;
};

#endif  // COMMON_FILTER_MEDIAN_FILTER_H_
