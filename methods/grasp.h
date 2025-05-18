#pragma once
#include "../matrix_utils.h"
#include <random>
#include <algorithm>
#include <vector>
#include <utility>

namespace GRASP {
    inline std::vector<int> ConstructGreedyRandomizedSolution(const Matrix& Q, double alpha, std::mt19937& gen) {
        int n = Q.size();
        std::vector<int> solution(n, 0);
        std::vector<int> unassigned;
        
        for (int i = 0; i < n; ++i) {
            unassigned.push_back(i);
        }
        
        while (!unassigned.empty()) {
            std::vector<std::pair<int, double>> candidates;
            
            for (int idx : unassigned) {
                // Evaluate both possibilities for current variable
                solution[idx] = 1;
                double value_pos = MatrixUtils::EvaluateSolution(Q, solution);
                
                solution[idx] = -1;
                double value_neg = MatrixUtils::EvaluateSolution(Q, solution);
                
                // Reset to unassigned
                solution[idx] = 0;
                
                double best_value = std::min(value_pos, value_neg);
                candidates.emplace_back(idx, best_value);
            }
            
            // Sort candidates by their best possible value
            std::sort(candidates.begin(), candidates.end(), 
                     [](const auto& a, const auto& b) { return a.second < b.second; });
            
            // Determine RCL (Restricted Candidate List)
            int rcl_size = std::max(1, static_cast<int>(alpha * candidates.size()));
            std::uniform_int_distribution<int> dist(0, rcl_size - 1);
            int selected = dist(gen);
            
            int chosen_idx = candidates[selected].first;
            
            // Assign the best value for chosen variable
            solution[chosen_idx] = 1;
            double value_pos = MatrixUtils::EvaluateSolution(Q, solution);
            
            solution[chosen_idx] = -1;
            double value_neg = MatrixUtils::EvaluateSolution(Q, solution);
            
            solution[chosen_idx] = (value_pos < value_neg) ? 1 : -1;
            
            // Remove assigned variable from unassigned list
            unassigned.erase(std::remove(unassigned.begin(), unassigned.end(), chosen_idx), unassigned.end());
        }
        
        return solution;
    }

    inline std::pair<std::vector<int>, double> LocalSearch(const Matrix& Q, const std::vector<int>& initial_solution, 
                                                   int max_iterations = 1000) {
        auto current_solution = initial_solution;
        double current_value = MatrixUtils::EvaluateSolution(Q, current_solution);
        
        bool improved = true;
        int iterations = 0;
        
        while (improved && iterations < max_iterations) {
            improved = false;
            
            for (size_t i = 0; i < current_solution.size(); ++i) {
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

    inline std::pair<std::vector<int>, double> Solve(const Matrix& Q, int max_iterations = 100, 
                                             double alpha = 0.3, int seed = 42) {
        std::mt19937 gen(seed);
        int n = Q.size();
        
        auto best_solution = ConstructGreedyRandomizedSolution(Q, alpha, gen);
        double best_value = MatrixUtils::EvaluateSolution(Q, best_solution);
        
        for (int i = 0; i < max_iterations; ++i) {
            auto solution = ConstructGreedyRandomizedSolution(Q, alpha, gen);
            auto [improved_solution, improved_value] = LocalSearch(Q, solution);
            
            if (improved_value < best_value) {
                best_solution = improved_solution;
                best_value = improved_value;
            }
        }
        
        return {best_solution, best_value};
    }
}