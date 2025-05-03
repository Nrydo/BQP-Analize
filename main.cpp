#include <iostream>
#include <chrono>
#include <fstream>
#include "matrix_utils.h"

void runExperiments() {
    const int min_n = 5;
    const int max_n = 50;
    const int step_n = 5;
    const int instances_per_n = 10;
    
    for (int n = min_n; n <= max_n; n += step_n) {
        for (int instance = 0; instance < instances_per_n; ++instance) {
            int seed = n * 100 + instance;
            auto Q = MatrixUtils::GenerateSymmetricMatrix(n, seed);
            
            // Methods
            
            std::cout << "Completed n=" << n << " instance=" << instance << std::endl;
        }
    }
}

int main() {
    runExperiments();
    return 0;
}
