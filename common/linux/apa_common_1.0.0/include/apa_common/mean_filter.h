// Copyright [2019] - MiniEye INC.
#pragma once

#include <stdint.h>
#include <deque>
#include <utility>
#include <vector>

namespace apa {

/**
 * @class MeanFilter
 * @brief The MeanFilter class is used to smoothen a series of noisy numbers,
 * such as sensor data or the output of a function that we wish to be smoother.
 * 
 * This is achieved by keeping track of the last k measurements
 * (where k is the windos size), and returning the average of all but the 
 * minimum and maximum measurements, which are likely to be outliers.
 */

class MeanFilter {
 public:
    /**
     * @brief Initializes a MeanFilter with a given window size
     * @param window_size the window_size of the meanfilter.
     * older measurements are discarded.
     */
    explicit MeanFilter(const uint16_t window_size);

    MeanFilter() : m_window_size(0),
                   m_sum(0.f),
                   m_time(0) {}

    ~MeanFilter() = default;

    /**
     * @brief Processed a new measurement in amortized constant time.
     * @param measurement the measurement to be processed by the filter.
     */
    float Update(const float measurement);

    std::size_t Size() { return m_values.size();}

 private:
    void RemoveEarliest();
    void Insert(const float value);
    float GetMin() const;
    float GetMax() const;
    bool ShouldPopOldestCandidate(const uint16_t old_time) const;
    uint16_t m_window_size;
    float m_sum;
    uint16_t m_time;
    // front = earliest
    std::deque<float> m_values;
    // front = min
    std::deque<std::pair<uint16_t, float>> m_min_canditates;
    // front = max
    std::deque<std::pair<uint16_t, float>> m_max_canditates;
    bool m_initialized;
};

}  // namespace apa