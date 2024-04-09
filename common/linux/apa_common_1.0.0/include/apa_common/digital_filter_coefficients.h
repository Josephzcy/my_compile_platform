// Copyright [2019] - MiniEye INC.
#pragma once

#include <vector>

/**
 * @file DigitalFilterCoefficients
 * @brief The DigitalFilterCoefficients class is used to generate coefficients for digital filter.
 */

namespace apa {

/**
 * @brief Get low-pass coefficients for digital filter.
 * @param ts Time interval between signals.
 * @param cutoff_freq Cutoff of frequency to filter high-frequency signals out.
 * @param denominators Denominator coefficients for digital filter.
 * @param numerators Numerators coefficients for digital filter.
 */
void LpfCoefficients(const float ts, const float cutoff_freq,
                     std::vector<float> *denominators, std::vector<float> *numerators);

/**
 * @brief Get low-pass coefficients for ZOH digital filter.
 * @param ts sampling time.
 * @param settling_time time required for an output to reach and remain within a ginven error band.
 * @param dead_time time delay.
 * @param denominators Denominator coefficients for digital filter.
 * @param numerators Numerators coefficients for digital filter.
 */
void LpFirstOrderCoefficients(const float ts, const float settling_time, const float dead_time,
                              std::vector<float> *denominators, std::vector<float> *numerators);


}  // namespace apa