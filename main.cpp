#include <iostream>
#include <chrono>
#include <fstream>
#include "matrix_utils.h"
#include "methods/brute_force.h"
#include "methods/local_search.h"

void runExperiments() {
    std::ofstream report_file("report.csv");
    report_file << "n,instance,algorithm,value,time_ms\n";
    
    const int min_n = 5;
    const int max_n = 50;
    const int step_n = 5;
    const int instances_per_n = 10;
    
    for (int n = min_n; n <= max_n; n += step_n) {
        for (int instance = 0; instance < instances_per_n; ++instance) {
            int seed = n * 100 + instance;
            auto Q = MatrixUtils::GenerateSymmetricMatrix(n, seed);
            
            // Brute Force (only for small n)
            if (n <= 20) {
                auto start = std::chrono::high_resolution_clock::now();
                auto [bf_solution, bf_value] = BruteForce::Solve(Q);
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                report_file << n << "," << instance << ",BruteForce," << bf_value << "," << duration.count() << "\n";
            }

            // Local Search
            auto start = std::chrono::high_resolution_clock::now();
            auto [ls_solution, ls_value] = LocalSearch::Solve(Q, 1000, seed);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            report_file << n << "," << instance << ",LocalSearch," << ls_value << "," << duration.count() << "\n";
            
            std::cout << "Completed n=" << n << " instance=" << instance << std::endl;
        }
    }
    
    report_file.close();
}

int main() {
    runExperiments();
    std::cout << "Experiments completed. Results saved to report.csv" << std::endl;
    return 0;
}
