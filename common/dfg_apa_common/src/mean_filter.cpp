// Copyright [2019] - MiniEye INC.
#include <limits>
#include "mean_filter.h"
#include "glog/logging.h"

namespace apa {

using MF = MeanFilter;
using TimeValue = std::pair<uint16_t, float>;

const uint16_t kMaxWindowSize = std::numeric_limits<uint16_t>::max() / 2;

MF::MeanFilter(const uint16_t window_size) {
    LOG(ERROR) << "window_size = " << window_size;
    CHECK_GT(window_size, 0);
    CHECK_LE(window_size, kMaxWindowSize);
    m_initialized = true;
    m_window_size = window_size;
}

float MF::GetMin() const {
    if (m_min_canditates.empty()) {
        return std::numeric_limits<float>::infinity();
    } else {
        return m_min_canditates.front().second;
    }
}

float MF::GetMax() const {
    if (m_max_canditates.empty()) {
        return std::numeric_limits<float>::infinity();
    } else {
        return m_max_canditates.front().second;
    }
}

void MF::Insert(const float value) {
    m_values.push_back(value);
    m_sum += value;
    while (m_min_canditates.size() > 0 && m_min_canditates.back().second > value) {
        m_min_canditates.pop_back();
    }
    m_min_canditates.push_back(std::make_pair(m_time, value));
    while (m_max_canditates.size() > 0 && m_max_canditates.back().second < value) {
        m_max_canditates.pop_back();
    }
    m_max_canditates.push_back(std::make_pair(m_time, value));
}



bool MF::ShouldPopOldestCandidate(const uint16_t old_time) const {
    if (old_time < m_window_size) {
        CHECK_LE(m_time, old_time + m_window_size);
        return m_time == old_time + m_window_size;
    } else if (m_time < m_window_size) {
        CHECK_GE(old_time, m_time + m_window_size);
        return old_time == m_time + m_window_size;
    } else {
        return false;
    }
}

void MF::RemoveEarliest() {
    CHECK_EQ(m_values.size(), m_window_size);
    float removed = m_values.front();
    m_values.pop_front();
    m_sum -= removed;
    if (ShouldPopOldestCandidate(m_min_canditates.front().first)) {
        m_min_canditates.pop_front();
    }
    if (ShouldPopOldestCandidate(m_max_canditates.front().first)) {
        m_max_canditates.pop_front();
    }
}

float MF::Update(const float measurement) {
    CHECK(m_initialized);
    CHECK_LE(m_values.size(), m_window_size);
    CHECK_LE(m_min_canditates.size(), m_window_size);
    CHECK_LE(m_max_canditates.size(), m_window_size);
    ++m_time;
    m_time %= static_cast<uint16_t>(2 * m_window_size);
    if (m_values.size() == m_window_size) {
        RemoveEarliest();
    }
    Insert(measurement);
    if (m_values.size() > 2) {
        return (m_sum - GetMax() - GetMin()) / static_cast<float>(m_values.size() - 2);
    } else {
        return m_sum / static_cast<float>(m_values.size());
    }
}

}  // namespace apa