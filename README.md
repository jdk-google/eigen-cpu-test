Testing how Eigen Matrix multiplication scales with more threads 


Results on 64 hyper-thread machine:

Running Baseline (1 Thread)...
Baseline Time: 14.854s. CPU_Time 14.7776 s (result: 375053.301)

Threads    | Time          | CPU Time      | Verification
-----------|---------------|---------------|-------------------
         2 |     7.408513s |    14.745574s | Result: 375053.301
         4 |     3.719848s |    14.745823s | Result: 375053.301
         8 |     1.878615s |    14.859519s | Result: 375053.301
        16 |     0.952755s |    15.011552s | Result: 375053.301
        24 |     0.644369s |    15.185889s | Result: 375053.301
        32 |     0.510328s |    15.617727s | Result: 375053.301
        48 |     0.468160s |    21.397860s | Result: 375053.301
        64 |     0.444695s |    25.261024s | Result: 375053.301

--- Speedup Summary ---
2 Threads:  2.00x speedup
4 Threads:  3.99x speedup
8 Threads:  7.91x speedup
16 Threads: 15.59x speedup
24 Threads: 23.05x speedup
32 Threads: 29.11x speedup
48 Threads: 31.73x speedup
64 Threads: 33.40x speedup
