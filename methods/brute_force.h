#pragma once
#include "../matrix_utils.h"
#include <climits>

namespace BruteForce {
    inline void GenerateAllSolutions(int n, int index, std::vector<int>& current, 
                            const Matrix& Q, double& best_value, std::vector<int>& best_solution) {
        if (index == n) {
            double current_value = MatrixUtils::EvaluateSolution(Q, current);
            if (current_value < best_value) {
                best_value = current_value;
                best_solution = current;
            }
            return;
        }
        
        current[index] = 1;
        GenerateAllSolutions(n, index + 1, current, Q, best_value, best_solution);
        
        current[index] = -1;
        GenerateAllSolutions(n, index + 1, current, Q, best_value, best_solution);
    }

    inline std::pair<std::vector<int>, double> Solve(const Matrix& Q) {
        int n = Q.size();
        std::vector<int> current(n);
        std::vector<int> best_solution(n);
        double best_value = std::numeric_limits<double>::max();
        
        GenerateAllSolutions(n, 0, current, Q, best_value, best_solution);
        
        return {best_solution, best_value};
    }
}
