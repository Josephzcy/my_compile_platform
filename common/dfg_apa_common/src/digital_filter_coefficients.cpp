// Copyright [2019] - MiniEye INC.
#include <cmath>
#include <vector>
#include "glog/logging.h"
#include "digital_filter_coefficients.h"

namespace apa {

void LpfCoefficients(const float ts, const float cutoff_freq,
                     std::vector<float> *denominators, std::vector<float> *numerators) {
    denominators->clear();
    numerators->clear();
    denominators->reserve(3);  // reserve增加了容器的capacity但是size没有变，而resize改变了capacity同时改变了size
    numerators->reserve(3);

    float wa = 2.0 * M_PI * cutoff_freq;  //  Analog frequency in rad/s
    float alpha = wa * ts / 2.0;  //  tan(Wd / 2), Wd is discrete frequency
    float alpha_sqr = alpha * alpha;
    float tmp_term = std::sqrt(2.0) * alpha + alpha_sqr;
    float gain = alpha_sqr / (1.0 + tmp_term);

    denominators->push_back(1.0);
    denominators->push_back(2.0 * (alpha_sqr - 1.0) / (1.0 + tmp_term));
    denominators->push_back((1.0 - std::sqrt(2.0) * alpha + alpha_sqr) / (1.0 + tmp_term));

    numerators->push_back(gain);
    numerators->push_back(2.0 * gain);
    numerators->push_back(gain);

    return;
}

void LpFirstOrderCoefficients(const float ts, const float settling_time, const float dead_time,
                              std::vector<float> *denominators, std::vector<float> *numerators) {
    //  sanity check
    if (ts <= 0.0 || settling_time < 0.0 || dead_time < 0.0) {
        LOG(ERROR) <<  "time can not be negative";
        return;
    }

    const std::size_t k_d = static_cast<int>(dead_time / ts);
    float a_term;
    denominators->clear();
    numerators->clear();
    denominators->reserve(2);
    numerators->reserve(k_d);

    if (settling_time == 0.0) {
        a_term = 0.0;
    } else {
        a_term = exp(-1 * ts / settling_time);
    }

    denominators->push_back(1.0);
    denominators->push_back(-a_term);
    numerators->insert(numerators->end(), k_d -1, 0.0);
    numerators->push_back(1 - a_term);
}

}  // namespace apa