#include <cmath>
#include "common/math/polyfit.h"

int polyfit(const std::vector<double>&  dependent_values,
        const std::vector<double>& independent_values,
        int order,
        std::vector<double>* coefficients)
{
    int max_order = order + 1;
    std::vector<double> b(max_order, 0.0);
    std::vector<double> p(max_order * 2 + 1, 0.0);
    std::vector<double> a(max_order*max_order*2, 0.0);

    if (static_cast<int>(dependent_values.size()) <= order) {
        return -1;
    }

    double x = 0;
    double y = 0;
    double powx = 0;
    for (size_t i = 0; i < dependent_values.size(); ++i) {
        x = dependent_values[i];
        y = independent_values[i];
        powx = 1;
        for (int j = 0; j < max_order; ++j) {
            b[j] = b[j] + (y * powx);
            powx = powx * x;
        }
    }

    // initialize the powx array
    p[0] = dependent_values.size();

    // compute the sum of the powers of x
    for (size_t i = 0; i < dependent_values.size(); ++i) {
        x = dependent_values[i];
        powx = dependent_values[i];
        int end_j = max_order * 2 + 1;
        for (int j = 0; j < end_j; ++j) {
            p[j] = p[j] + powx;
            powx = powx * x;
        }
    }

    // initialize the reduction matrix
    for (int i = 0; i < max_order; ++i) {
        for (int j = 0; j < max_order; ++j) {
            a[i * 2 * max_order + j] = p[i + j];
        }
        a[i * 2 * max_order + i + max_order] = 1;
    }

    int order2 = 2 * max_order;
    // move the identity matrix portion of the redux matrix
    for (int i = 0; i < max_order; ++i) {
        x = a[i * 2 * max_order + i];
        if (fabs(x) > 1e-5) {
            for (int k = 0; k < order2; ++k) {
                a[i * order2 + k] /= x;
            }

            for (int j = 0; j < max_order; ++j) {
                if (j != i) {
                    y = a[j * order2 + i];
                    for (int k = 0; k < order2; ++k) {
                        a[j * order2 + k] -= y * a[i * order2 + k];

                    }
                }
            }
        } else {
            // can't work with singular matrices
            return -1;
        }
    }

    // calculate and identify the coefficients
    for (int i = 0; i < max_order; ++i) {
        for (int j = 0; j < max_order; ++j) {
            x = 0;
            for (int k = 0; k < max_order; ++k) {
                x += (a[i * order2 + k + max_order] * b[k]);
            }
            (*coefficients)[i] = x;
        }
    }
    return 0;
}
