#pragma once
#include "../matrix_utils.h"
#include <random>

namespace LocalSearch {
    inline std::vector<int> GenerateRandomSolution(int n, std::mt19937& gen) {
        std::vector<int> solution(n);
        std::uniform_int_distribution<int> dist(0, 1);
        for (int i = 0; i < n; ++i) {
            solution[i] = (dist(gen) == 0) ? -1 : 1;
        }
        return solution;
    }

    inline std::pair<std::vector<int>, double> Solve(const Matrix& Q, int max_iterations = 1000, int seed = 42) {
        std::mt19937 gen(seed);
        int n = Q.size();
        
        auto current_solution = GenerateRandomSolution(n, gen);
        double current_value = MatrixUtils::EvaluateSolution(Q, current_solution);
        
        bool improved = true;
        int iterations = 0;
        
        while (improved && iterations < max_iterations) {
            improved = false;
            
            for (int i = 0; i < n; ++i) {
                auto neighbor = current_solution;
                neighbor[i] *= -1;
                
                double neighbor_value = MatrixUtils::EvaluateSolution(Q, neighbor);
                
                if (neighbor_value < current_value) {
                    current_solution = neighbor;
                    current_value = neighbor_value;
                    improved = true;
                }
            }
            
            iterations++;
        }
        
        return {current_solution, current_value};
    }
}
