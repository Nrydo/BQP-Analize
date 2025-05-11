#pragma once
#include "../matrix_utils.h"

namespace Greedy {
    inline std::pair<std::vector<int>, double> Solve(const Matrix& Q) {
        int n = Q.size();
        std::vector<int> solution(n, 1);
        
        for (int i = 0; i < n; ++i) {
            // Evaluate both options for current variable
            solution[i] = 1;
            double value_pos = MatrixUtils::EvaluateSolution(Q, solution);
            
            solution[i] = -1;
            double value_neg = MatrixUtils::EvaluateSolution(Q, solution);
            
            // Choose the better option
            solution[i] = (value_pos < value_neg) ? 1 : -1;
        }
        
        double final_value = MatrixUtils::EvaluateSolution(Q, solution);
        return {solution, final_value};
    }
}
