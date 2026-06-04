#include <iostream>
#include <iomanip>
#include <Eigen/Dense>
#include <chrono>
#include <time.h>
#include <sys/resource.h>

#define EIGEN_DONT_PARALLELIZE

double getProcessCpuTime() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_utime.tv_sec + (usage.ru_utime.tv_usec / 1000000.0);
}

double compute_matrix_sum(const int index) {
    const int rows = 100;
    const int cols = 100;

    Eigen::MatrixXd A(rows, cols);
    Eigen::MatrixXd B(rows, cols);

    // Fill Matrix A using a nested for loop
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Populate with a formula, e.g., i + j
            A(i, j) = static_cast<double>(i/7581.0 + j/5961.2);
        }
    }

    // Fill Matrix B using a nested for loop
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Populate with a different formula, e.g., i * j
            B(i, j) = static_cast<double>(1.0/(i+1262.3) + (index% (i+4) + index % (j+3)) / (i+j+2192.3) + 1.0/(j+3167.3));
        }
    }

    // 1. Matrix Multiplication (100x100 multiplied by 100x100)
    Eigen::MatrixXd C = A * B;

    // 2. Summing all the cells of the resulting matrix
    return C.sum();
}

double run_multi_threaded_test_omp(int num_threads) {

    auto start_time = std::chrono::high_resolution_clock::now();
    auto start_cpu_time = getProcessCpuTime();
   	

    const int num_chunks = 1000;
    std::vector<double> results(num_chunks);

    #pragma omp parallel for \
            default(none) shared (results, num_chunks) \
            num_threads(num_threads)  schedule (dynamic, 3)
    for (int i = 0; i < num_chunks; ++i) {
        results[i] = compute_matrix_sum(i);
    }

    double result = std::accumulate(results.begin(), results.end(), 0.0);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto end_cpu_time = getProcessCpuTime();

    std::chrono::duration<double> diff = end_time - start_time;
    auto cpu_time_diff = end_cpu_time - start_cpu_time;
    
    std::cout << std::setw(10) << num_threads << " | " 
              << std::setprecision(6)
              << std::setw(12) << diff.count() << "s | "
              << std::setw(12) << cpu_time_diff << "s | "
              << "Result: " << std::fixed << std::setprecision(12) << result << "\n";
              
    return diff.count();
}

double run_single_threaded_test() {
  double total_sum=0;

  for (int i=0; i<1000; i++)
    total_sum += compute_matrix_sum(i);

  return total_sum;
}


// result should be: 375053

int main() {
    Eigen::setNbThreads(1);
    // 1. Single-threaded Baseline
    std::cout << "Running Baseline (1 Thread)..." << std::endl;
    auto start_cpu_time = getProcessCpuTime();
    auto start_baseline = std::chrono::high_resolution_clock::now();
    double pi_baseline = run_single_threaded_test();
    auto end_cpu_time = getProcessCpuTime();
    auto end_baseline = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_baseline = end_baseline - start_baseline;
    double t_baseline = diff_baseline.count();
    double baseline_cpu = end_cpu_time - start_cpu_time;
    
    std::cout << "Baseline Time: " << t_baseline << "s. CPU_Time " << baseline_cpu << " s(Pi: " << pi_baseline << ")\n" << std::endl;

    // 2. Comparative Benchmarks
    std::cout << "Threads    | Time          | CPU Time      | Verification" << std::endl;
    std::cout << "-----------|---------------|---------------|-------------------" << std::endl;
    
    double t2 = run_multi_threaded_test_omp(2);
    double t4 = run_multi_threaded_test_omp(4);
    double t8 = run_multi_threaded_test_omp(8);
    double t16 = run_multi_threaded_test_omp(16);
    double t24 = run_multi_threaded_test_omp(24);
    double t32 = run_multi_threaded_test_omp(32);
    double t48 = run_multi_threaded_test_omp(48);

    // 3. Summary Report
    std::cout << "\n--- Speedup Summary ---" << std::fixed << std::setprecision(2) << std::endl;
    std::cout << "2 Threads:  " << t_baseline / t2 << "x speedup" << std::endl;
    std::cout << "4 Threads:  " << t_baseline / t4 << "x speedup" << std::endl;
    std::cout << "8 Threads:  " << t_baseline / t8 << "x speedup" << std::endl;
    std::cout << "16 Threads: " << t_baseline / t16 << "x speedup" << std::endl;
    std::cout << "24 Threads: " << t_baseline / t24 << "x speedup" << std::endl;
    std::cout << "32 Threads: " << t_baseline / t32 << "x speedup" << std::endl;
    std::cout << "48 Threads: " << t_baseline / t48 << "x speedup" << std::endl;
    
    return 0;
}
