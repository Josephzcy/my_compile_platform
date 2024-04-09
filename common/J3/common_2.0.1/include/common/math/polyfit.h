#ifndef  POLYFIT_H
#define  POLYFIT_H

#include <vector>
#include "common/base/stdint.h"

int polyfit(const std::vector<double>&  dependent_values,
        const std::vector<double>& independent_values,
        int order,
        std::vector<double>* coefficients);


#endif  // POLYFIT_H
