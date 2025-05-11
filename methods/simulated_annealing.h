#pragma once
#include "../matrix_utils.h"
#include <random>
#include <cmath>

namespace SimulatedAnnealing {
    inline std::vector<int> GenerateRandomSolution(int n, std::mt19937& gen) {
        std::vector<int> solution(n);
        std::uniform_int_distribution<int> dist(0, 1);
        for (int i = 0; i < n; ++i) {
            solution[i] = (dist(gen) == 0) ? -1 : 1;
        }
        return solution;
    }

    inline std::vector<int> GetRandomNeighbor(const std::vector<int>& solution, std::mt19937& gen) {
        std::uniform_int_distribution<int> dist(0, solution.size() - 1);
        int flip_index = dist(gen);
        
        auto neighbor = solution;
        neighbor[flip_index] *= -1;
        return neighbor;
    }

    inline std::pair<std::vector<int>, double> Solve(const Matrix& Q, double initial_temp = 1000.0, 
                                             double cooling_rate = 0.995, int max_iterations = 10000, 
                                             int seed = 42) {
        std::mt19937 gen(seed);
        int n = Q.size();
        
        auto current_solution = GenerateRandomSolution(n, gen);
        double current_value = MatrixUtils::EvaluateSolution(Q, current_solution);
        
        auto best_solution = current_solution;
        double best_value = current_value;
        
        double temperature = initial_temp;
        
        for (int i = 0; i < max_iterations; ++i) {
            auto neighbor = GetRandomNeighbor(current_solution, gen);
            double neighbor_value = MatrixUtils::EvaluateSolution(Q, neighbor);
            
            double delta = neighbor_value - current_value;
            
            if (delta < 0) {
                current_solution = neighbor;
                current_value = neighbor_value;
                
                if (neighbor_value < best_value) {
                    best_solution = neighbor;
                    best_value = neighbor_value;
                }
            } else {
                std::uniform_real_distribution<double> prob_dist(0.0, 1.0);
                double probability = exp(-delta / temperature);
                
                if (prob_dist(gen) < probability) {
                    current_solution = neighbor;
                    current_value = neighbor_value;
                }
            }
            
            temperature *= cooling_rate;
            
            if (temperature < 1e-6) {
                break;
            }
        }
        
        return {best_solution, best_value};
    }
}
