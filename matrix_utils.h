#pragma once
#include <vector>
#include <random>
#include <iostream>
#include <iomanip>

using Matrix = std::vector<std::vector<double>>;

namespace MatrixUtils {
    inline Matrix GenerateSymmetricMatrix(int n, int seed = 42) {
        std::mt19937 gen(seed);
        std::uniform_real_distribution<double> dist(-1.0, 1.0);
        
        Matrix Q(n, std::vector<double>(n));
        for (int i = 0; i < n; ++i) {
            for (int j = i; j < n; ++j) {
                Q[i][j] = dist(gen);
                if (i != j) {
                    Q[j][i] = Q[i][j];
                }
            }
        }
        return Q;
    }

    inline double EvaluateSolution(const Matrix& Q, const std::vector<int>& solution) {
        double result = 0.0;
        int n = Q.size();
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                result += Q[i][j] * solution[i] * solution[j];
            }
        }
        return result;
    }

    inline void PrintMatrix(const Matrix& Q) {
        for (const auto& row : Q) {
            for (double val : row) {
                std::cout << std::setw(8) << std::fixed << std::setprecision(4) << val;
            }
            std::cout << std::endl;
        }
    }
}
