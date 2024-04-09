// Copyright [2019] - MiniEye INC.
#pragma once

#include <stdint.h>
#include <deque>
#include <utility>
#include <vector>

namespace apa {

/**
 * @class DigitalFilter, IIR
 * @brief The DigitalFilter class is used to pass signals with a frequency, lower than a certain cutoff frequency
 * and attenuates signals with frequencies higher than the cutoff frequency.
 */

class DigitalFilter {
 public:
    DigitalFilter();

    /**
     * @brief Initializes a DigitalFilter with given denominators and numerators.
     * @param denominators the denominators of the digital filter.
     * @param numerators the numerators of the digital filter.
     */
    DigitalFilter(const std::vector<float> &denominators, const std::vector<float> &numerators);

    ~DigitalFilter() = default;

    /**
     * @brief Processes a new measurement with the filter.
     * @param m_x_insert the new input measurement.
     */
    float Filter(const float x_insert);
    /**
     * @desc: Filter by the input m_x_insert
     * Input: new value of m_x_insert.
     * Remove x[n - 1], insert m_x_insert into x[0], remove y[n - 1].
     * Solve den[0] * y + den[1] * y[0] + ... + den[n - 1] * y[n - 2] =
     *       num[0] * [0] + num[1] * x[1] + ... + num[n - 1] * x[n - 1] for y
     * Insert y into y[0]
     * Output: y[0]
     */

    void set_denominators(const std::vector<float> &denominators);

    const std::vector<float> &get_denominators() const;

    void set_numerators(const std::vector<float> &numerators);

    const std::vector<float> &get_numerators() const;

    void set_coefficients(const std::vector<float> &denominators, const std::vector<float> &numerators);

    /**
     * @brief set filter dead zone.
     * @param deadzone the filter dead zone.
     */
    void set_dead_zone(const float deadzone);

    /**
     * @brief re-set the x_values and y_values.
     */
    void reset_values();

    const std::deque<float> &get_inputs_queue() const;

    const std::deque<float> &get_outputs_queue() const;

 private:
    /**
     * @desc: Update the last-filtered value, if the difference is less than m_dead_zone.
     */
    float UpdateLast(const float input);
    float Compute(const std::deque<float> &values, const std::vector<float> &coefficients,
                  const std::size_t coeff_start, const std::size_t coeff_end);
    //  Front is latest, back is oldest.
    std::deque<float> m_x_values;

    //  Front is latest, back is oldest.
    std::deque<float> m_y_values;

    //  Coefficients with y values.
    std::vector<float> m_denominators;

    //  Coefficients with x values.
    std::vector<float> m_numerators;

    //  threshold of updating last-filterd value.
    float m_dead_zone;

    //  last-filterd value.
    float m_last;
};

}  // namespace apa