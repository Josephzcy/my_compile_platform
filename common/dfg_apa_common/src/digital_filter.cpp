// Copyright [2019] - MiniEye INC.
#include <cmath>
#include "glog/logging.h"
#include "digital_filter.h"

namespace apa {

const float kFloatEpsilon = 1.0e-6;

    DigitalFilter::DigitalFilter() {
        m_dead_zone = 0.0;
        m_last = 0.0;
    }

    DigitalFilter::DigitalFilter(const std::vector<float> &denominators, const std::vector<float> &numerators) {
        set_coefficients(denominators, numerators);
        m_dead_zone = 0.0;
        m_last = 0.0;
    }

    void DigitalFilter::set_denominators(const std::vector<float> &denominators) {
        m_denominators = denominators;
        m_y_values.resize(m_denominators.size(), 0.0);
    }

    const std::vector<float> &DigitalFilter::get_denominators() const {
        return m_denominators;
    }

    void DigitalFilter::set_numerators(const std::vector<float> &numerators) {
        m_numerators = numerators;
        m_x_values.resize(m_numerators.size(), 0.0);
    }

    const std::vector<float> &DigitalFilter::get_numerators() const {
        return m_numerators;
    }

    void DigitalFilter::set_coefficients(const std::vector<float> &denominators, const std::vector<float> &numerators) {
        set_denominators(denominators);
        set_numerators(numerators);
    }

    void DigitalFilter::set_dead_zone(const float deadzone) {
        m_dead_zone = deadzone;
        VLOG(2) << "setting digital filter dead zone = " << m_dead_zone;
    }

    void DigitalFilter::reset_values() {
        std::fill(m_x_values.begin(), m_x_values.end(), 0.0);
        std::fill(m_y_values.begin(), m_y_values.end(), 0.0);
    }

    const std::deque<float> &DigitalFilter::get_inputs_queue() const {
        return m_x_values;
    }

    const std::deque<float> &DigitalFilter::get_outputs_queue() const {
        return m_y_values;
    }

    float DigitalFilter::UpdateLast(const float input) {
        const float diff = std::fabs(input - m_last);
        if (diff > m_dead_zone) {
            return m_last;
        }
        m_last = input;
        return input;
    }

    float DigitalFilter::Compute(const std::deque<float> &values, const std::vector<float> &coefficients,
                                 const std::size_t coeff_start, const std::size_t coeff_end) {
        CHECK_LE(coeff_start, coeff_end);
        CHECK_LT(coeff_end, coefficients.size());
        CHECK(coeff_end - coeff_start + 1 == values.size());
        float sum = 0.0;
        auto i = coeff_start;
        for (const auto value : values) {
            sum += value * coefficients[i];
            ++i;
        }
        return sum;
    }

    float DigitalFilter::Filter(const float x_insert) {
        if (m_numerators.empty() || m_denominators.empty()) {
            LOG(ERROR) << "Empty m_numerators or m_denominators";
            return 0.0;
        }
        m_x_values.pop_back();
        m_x_values.push_front(x_insert);
        const float x_side = Compute(m_x_values, m_numerators, 0, m_numerators.size() - 1);
        m_y_values.pop_back();
        const float y_side = Compute(m_y_values, m_denominators, 1, m_denominators.size() - 1);
        // m_denominators.size() - 2, but pop_back, so m_denominators.size() - 1;
        float y_insert = 0.0;
        if (std::fabs(m_denominators.front()) > kFloatEpsilon) {
            y_insert = (x_side - y_side) / m_denominators.front();
        }
        m_y_values.push_front(y_insert);
        return UpdateLast(y_insert);
    }
}  // namespace apa