#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

// Platform-specific includes
#ifdef _WIN32
    #include <windows.h>
    #include <process.h>
    #define HAVE_WINDOWS_THREADS 1
#else
    #include <unistd.h>
    #include <pthread.h>
    #include <sys/time.h>
    #define HAVE_PTHREAD 1
#endif

// SIMD includes (if supported)
#if defined(__AVX512BW__) || defined(__AVX2__)
    #include <immintrin.h>
#elif defined(__SSE2__)
    #include <emmintrin.h>
#endif

#ifdef _OPENMP
    #include <omp.h>
#endif

// 🚀⚡ C BLAZING FAST BENCHMARK SUITE ⚡🚀
//
// C features:
// - Manual memory management (zero GC overhead)
// - Direct hardware access
// - Compiler optimizations (-O3, -march=native)
// - SIMD intrinsics
// - Low-level pointer arithmetic
// - pthread multithreading
// - Cache-friendly data structures

// User represents a user with traditional struct layout
typedef struct {
    uint32_t id;
    char name[32];  // Fixed size for better cache performance
    uint8_t age;
} User;

// UserSoA represents users in Struct of Arrays layout for cache efficiency
typedef struct {
    uint32_t *ids;
    char (*names)[32];  // Array of fixed-size strings
    uint8_t *ages;
    size_t count;
    size_t capacity;
} UserSoA;

// Benchmark result structure
typedef struct {
    char name[64];
    double time_ms;
    uint64_t result;
} BenchmarkResult;

// Thread data for parallel processing
typedef struct {
    uint8_t *ages;
    size_t start;
    size_t end;
    uint64_t result;
} ThreadData;

// Thread handle type (platform-specific)
#ifdef HAVE_WINDOWS_THREADS
    typedef HANDLE thread_t;
    typedef DWORD thread_return_t;
    #define THREAD_CALL WINAPI
#else
    typedef pthread_t thread_t;
    typedef void* thread_return_t;
    #define THREAD_CALL
#endif

// Global variables for configuration
static size_t NUM_USERS = 1000000;
static int NUM_THREADS = 4;

// 🛠️ UTILITY FUNCTIONS 🛠️

// Get current time in milliseconds
double get_time_ms() {
#ifdef _WIN32
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart * 1000.0 / (double)frequency.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
#endif
}

// Get number of CPU cores
int get_cpu_cores() {
#ifdef _WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#else
    return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

// Create UserSoA with given capacity
UserSoA* create_user_soa(size_t capacity) {
    UserSoA *soa = malloc(sizeof(UserSoA));
    if (!soa) return NULL;
    
    // Use platform-specific aligned allocation
#ifdef _WIN32
    soa->ids = _aligned_malloc(capacity * sizeof(uint32_t), 64);
    soa->names = _aligned_malloc(capacity * sizeof(char[32]), 64);
    soa->ages = _aligned_malloc(capacity * sizeof(uint8_t), 64);
#else
    soa->ids = aligned_alloc(64, capacity * sizeof(uint32_t));  // 64-byte aligned for AVX-512 and cache lines
    soa->names = aligned_alloc(64, capacity * sizeof(char[32]));
    soa->ages = aligned_alloc(64, capacity * sizeof(uint8_t));
#endif
    
    if (!soa->ids || !soa->names || !soa->ages) {
        free(soa->ids);
        free(soa->names);
        free(soa->ages);
        free(soa);
        return NULL;
    }
    
    soa->count = capacity;
    soa->capacity = capacity;
    return soa;
}

// Free UserSoA
void free_user_soa(UserSoA *soa) {
    if (soa) {
#ifdef _WIN32
        _aligned_free(soa->ids);
        _aligned_free(soa->names);
        _aligned_free(soa->ages);
#else
        free(soa->ids);
        free(soa->names);
        free(soa->ages);
#endif
        free(soa);
    }
}

// 🔥 BLAZING FAST SUM IMPLEMENTATIONS 🔥

// SumAgesBasic - basic array of structs approach
uint64_t sum_ages_basic(User *users, size_t count) {
    uint64_t sum = 0;
    for (size_t i = 0; i < count; i++) {
        sum += users[i].age;
    }
    return sum;
}

// SumAgesSoA - optimized Struct of Arrays approach
uint64_t sum_ages_soa(UserSoA *users) {
    uint64_t sum = 0;
    uint8_t *ages = users->ages;
    size_t count = users->count;
    
    for (size_t i = 0; i < count; i++) {
        sum += ages[i];
    }
    return sum;
}

// SumAgesUnrolled - manual loop unrolling for better performance
uint64_t sum_ages_unrolled(UserSoA *users) {
    uint64_t sum = 0;
    uint8_t *ages = users->ages;
    size_t count = users->count;
    
    // Process 8 elements at a time (manual SIMD-like optimization)
    size_t i = 0;
    for (; i + 8 <= count; i += 8) {
        sum += (uint64_t)ages[i] + (uint64_t)ages[i+1] + (uint64_t)ages[i+2] + (uint64_t)ages[i+3] +
               (uint64_t)ages[i+4] + (uint64_t)ages[i+5] + (uint64_t)ages[i+6] + (uint64_t)ages[i+7];
    }
    
    // Handle remaining elements
    for (; i < count; i++) {
        sum += ages[i];
    }
    
    return sum;
}

// SumAgesPointer - pointer arithmetic optimization
uint64_t sum_ages_pointer(UserSoA *users) {
    uint64_t sum = 0;
    uint8_t *ptr = users->ages;
    uint8_t *end = ptr + users->count;
    
    while (ptr < end) {
        sum += *ptr++;
    }
    
    return sum;
}

// Fast AVX2 helper using PSADBW (8 bytes summed in 1 cycle)
static inline uint64_t sum_bytes_avx2(const uint8_t *ages, size_t count) {
    uint64_t sum = 0;
#ifdef __AVX2__
    const __m256i zero = _mm256_setzero_si256();
    __m256i acc0 = _mm256_setzero_si256();
    __m256i acc1 = _mm256_setzero_si256();
    __m256i acc2 = _mm256_setzero_si256();
    __m256i acc3 = _mm256_setzero_si256();

    size_t chunks_128 = count / 128;
    for (size_t i = 0; i < chunks_128; i++) {
        size_t offset = i * 128;
        __m256i b0 = _mm256_loadu_si256((const __m256i*)(ages + offset));
        __m256i b1 = _mm256_loadu_si256((const __m256i*)(ages + offset + 32));
        __m256i b2 = _mm256_loadu_si256((const __m256i*)(ages + offset + 64));
        __m256i b3 = _mm256_loadu_si256((const __m256i*)(ages + offset + 96));

        acc0 = _mm256_add_epi64(acc0, _mm256_sad_epu8(b0, zero));
        acc1 = _mm256_add_epi64(acc1, _mm256_sad_epu8(b1, zero));
        acc2 = _mm256_add_epi64(acc2, _mm256_sad_epu8(b2, zero));
        acc3 = _mm256_add_epi64(acc3, _mm256_sad_epu8(b3, zero));
    }

    __m256i acc = _mm256_add_epi64(
        _mm256_add_epi64(acc0, acc1),
        _mm256_add_epi64(acc2, acc3)
    );

    size_t processed = chunks_128 * 128;
    size_t chunks_32 = (count - processed) / 32;
    for (size_t i = 0; i < chunks_32; i++) {
        __m256i b = _mm256_loadu_si256((const __m256i*)(ages + processed + i * 32));
        acc = _mm256_add_epi64(acc, _mm256_sad_epu8(b, zero));
    }

    uint64_t result[4];
    _mm256_storeu_si256((__m256i*)result, acc);
    sum = result[0] + result[1] + result[2] + result[3];

    for (size_t i = processed + chunks_32 * 32; i < count; i++) {
        sum += ages[i];
    }
#else
    for (size_t i = 0; i < count; i++) {
        sum += ages[i];
    }
#endif
    return sum;
}

// SumAgesAVX - SIMD optimization with AVX2
uint64_t sum_ages_avx(UserSoA *users) {
    return sum_bytes_avx2(users->ages, users->count);
}

#ifdef __AVX512BW__
// Fast AVX-512 helper using PSADBW (64 bytes summed per instruction, 256 bytes per unrolled loop)
static inline uint64_t sum_bytes_avx512(const uint8_t *ages, size_t count) {
    uint64_t sum = 0;
    const __m512i zero = _mm512_setzero_si512();
    __m512i acc0 = _mm512_setzero_si512();
    __m512i acc1 = _mm512_setzero_si512();
    __m512i acc2 = _mm512_setzero_si512();
    __m512i acc3 = _mm512_setzero_si512();

    size_t chunks_256 = count / 256;
    for (size_t i = 0; i < chunks_256; i++) {
        size_t offset = i * 256;
        __m512i b0 = _mm512_loadu_si512((const void*)(ages + offset));
        __m512i b1 = _mm512_loadu_si512((const void*)(ages + offset + 64));
        __m512i b2 = _mm512_loadu_si512((const void*)(ages + offset + 128));
        __m512i b3 = _mm512_loadu_si512((const void*)(ages + offset + 192));

        acc0 = _mm512_add_epi64(acc0, _mm512_sad_epu8(b0, zero));
        acc1 = _mm512_add_epi64(acc1, _mm512_sad_epu8(b1, zero));
        acc2 = _mm512_add_epi64(acc2, _mm512_sad_epu8(b2, zero));
        acc3 = _mm512_add_epi64(acc3, _mm512_sad_epu8(b3, zero));
    }

    __m512i acc = _mm512_add_epi64(
        _mm512_add_epi64(acc0, acc1),
        _mm512_add_epi64(acc2, acc3)
    );

    size_t processed = chunks_256 * 256;
    size_t chunks_64 = (count - processed) / 64;
    for (size_t i = 0; i < chunks_64; i++) {
        __m512i b = _mm512_loadu_si512((const void*)(ages + processed + i * 64));
        acc = _mm512_add_epi64(acc, _mm512_sad_epu8(b, zero));
    }

    sum = _mm512_reduce_add_epi64(acc);

    for (size_t i = processed + chunks_64 * 64; i < count; i++) {
        sum += ages[i];
    }
    return sum;
}

uint64_t sum_ages_avx512(UserSoA *users) {
    return sum_bytes_avx512(users->ages, users->count);
}
#endif

// Thread function for parallel processing
thread_return_t THREAD_CALL thread_sum_ages(void *arg) {
    ThreadData *data = (ThreadData*)arg;
    data->result = sum_bytes_avx2(data->ages + data->start, data->end - data->start);
    
#ifdef HAVE_WINDOWS_THREADS
    return 0;
#else
    return NULL;
#endif
}

// SumAgesThreads - parallel processing with persistent OpenMP threads or fallback to pthreads
uint64_t sum_ages_threads(UserSoA *users) {
    if (users->count == 0) return 0;

#ifdef _OPENMP
    uint64_t total_sum = 0;
    int num_threads = NUM_THREADS;
    #pragma omp parallel num_threads(num_threads) reduction(+:total_sum)
    {
        int tid = omp_get_thread_num();
        int nth = omp_get_num_threads();
        size_t chunk_size = users->count / nth;
        size_t start = tid * chunk_size;
        size_t end = (tid == nth - 1) ? users->count : start + chunk_size;
#ifdef __AVX512BW__
        total_sum += sum_bytes_avx512(users->ages + start, end - start);
#else
        total_sum += sum_bytes_avx2(users->ages + start, end - start);
#endif
    }
    return total_sum;
#else
    int num_threads = NUM_THREADS;
    if (num_threads > (int)users->count) {
        num_threads = users->count;
    }
    
    thread_t threads[16];  // Fixed size array for compatibility
    ThreadData thread_data[16];
    
    if (num_threads > 16) num_threads = 16;  // Limit to array size
    
    size_t chunk_size = users->count / num_threads;
    size_t remaining = users->count % num_threads;
    
    // Create threads
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].ages = users->ages;
        thread_data[i].start = i * chunk_size;
        thread_data[i].end = (i + 1) * chunk_size;
        
        // Distribute remaining elements to last thread
        if (i == num_threads - 1) {
            thread_data[i].end += remaining;
        }
        
#ifdef HAVE_WINDOWS_THREADS
        threads[i] = CreateThread(NULL, 0, thread_sum_ages, &thread_data[i], 0, NULL);
#else
        pthread_create(&threads[i], NULL, thread_sum_ages, &thread_data[i]);
#endif
    }
    
    // Wait for threads and collect results
    uint64_t total_sum = 0;
    for (int i = 0; i < num_threads; i++) {
#ifdef HAVE_WINDOWS_THREADS
        WaitForSingleObject(threads[i], INFINITE);
        CloseHandle(threads[i]);
#else
        pthread_join(threads[i], NULL);
#endif
        total_sum += thread_data[i].result;
    }
    
    return total_sum;
#endif
}

// Thread function for parallel processing with unrolling
thread_return_t THREAD_CALL thread_sum_ages_unrolled(void *arg) {
    ThreadData *data = (ThreadData*)arg;
    uint64_t local_sum = 0;
    size_t i = data->start;
    
    // Unrolled loop within thread
    for (; i + 8 <= data->end; i += 8) {
        local_sum += (uint64_t)data->ages[i] + (uint64_t)data->ages[i+1] + 
                     (uint64_t)data->ages[i+2] + (uint64_t)data->ages[i+3] +
                     (uint64_t)data->ages[i+4] + (uint64_t)data->ages[i+5] + 
                     (uint64_t)data->ages[i+6] + (uint64_t)data->ages[i+7];
    }
    
    // Handle remaining elements
    for (; i < data->end; i++) {
        local_sum += data->ages[i];
    }
    
    data->result = local_sum;
#ifdef HAVE_WINDOWS_THREADS
    return 0;
#else
    return NULL;
#endif
}

// SumAgesThreadsUnrolled - parallel processing with loop unrolling
uint64_t sum_ages_threads_unrolled(UserSoA *users) {
    if (users->count == 0) return 0;
    
    int num_threads = NUM_THREADS;
    if (num_threads > (int)users->count) {
        num_threads = users->count;
    }
    
    thread_t threads[16];  // Fixed size array for compatibility
    ThreadData thread_data[16];
    
    if (num_threads > 16) num_threads = 16;  // Limit to array size
    
    size_t chunk_size = users->count / num_threads;
    size_t remaining = users->count % num_threads;
    
    // Create threads
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].ages = users->ages;
        thread_data[i].start = i * chunk_size;
        thread_data[i].end = (i + 1) * chunk_size;
        
        // Distribute remaining elements to last thread
        if (i == num_threads - 1) {
            thread_data[i].end += remaining;
        }
        
#ifdef HAVE_WINDOWS_THREADS
        threads[i] = CreateThread(NULL, 0, thread_sum_ages_unrolled, &thread_data[i], 0, NULL);
#else
        pthread_create(&threads[i], NULL, thread_sum_ages_unrolled, &thread_data[i]);
#endif
    }
    
    // Wait for threads and collect results
    uint64_t total_sum = 0;
    for (int i = 0; i < num_threads; i++) {
#ifdef HAVE_WINDOWS_THREADS
        WaitForSingleObject(threads[i], INFINITE);
        CloseHandle(threads[i]);
#else
        pthread_join(threads[i], NULL);
#endif
        total_sum += thread_data[i].result;
    }
    
    return total_sum;
}

// 📊 BENCHMARK FRAMEWORK 📊

// Benchmark a function and measure its performance
BenchmarkResult benchmark(const char *name, uint64_t (*func)(UserSoA*), UserSoA *data) {
    BenchmarkResult result;
    strncpy(result.name, name, sizeof(result.name) - 1);
    result.name[sizeof(result.name) - 1] = '\0';
    
    // Warmup
    func(data);
    
    // Measure best of multiple iterations for stable micro-benchmarking
    double best_time = 1e9;
    const int iters = 10;
    for (int i = 0; i < iters; i++) {
        __asm__ volatile("" : : "r"(data->ages) : "memory");
        double start = get_time_ms();
        uint64_t val = func(data);
        __asm__ volatile("" : "+r"(val) : : "memory");
        double end = get_time_ms();
        result.result = val;
        double dt = end - start;
        if (dt < best_time) best_time = dt;
    }
    
    result.time_ms = best_time;
    return result;
}

// Benchmark basic AoS function
BenchmarkResult benchmark_aos(const char *name, uint64_t (*func)(User*, size_t), User *data, size_t count) {
    BenchmarkResult result;
    strncpy(result.name, name, sizeof(result.name) - 1);
    result.name[sizeof(result.name) - 1] = '\0';
    
    // Warmup
    func(data, count);
    
    // Measure best of multiple iterations
    double best_time = 1e9;
    const int iters = 10;
    for (int i = 0; i < iters; i++) {
        __asm__ volatile("" : : "r"(data) : "memory");
        double start = get_time_ms();
        uint64_t val = func(data, count);
        __asm__ volatile("" : "+r"(val) : : "memory");
        double end = get_time_ms();
        result.result = val;
        double dt = end - start;
        if (dt < best_time) best_time = dt;
    }
    
    result.time_ms = best_time;
    return result;
}

// Compare function for qsort
int compare_results(const void *a, const void *b) {
    const BenchmarkResult *ra = (const BenchmarkResult*)a;
    const BenchmarkResult *rb = (const BenchmarkResult*)b;
    
    if (ra->time_ms < rb->time_ms) return -1;
    if (ra->time_ms > rb->time_ms) return 1;
    return 0;
}

// 📊 MAIN BENCHMARK SUITE 📊

int main(int argc, char *argv[]) {
    printf("🚀⚡ C BLAZING FAST BENCHMARK SUITE ⚡🚀\n");
    printf("\n");
    printf("📊 SYSTEM INFO:\n");
    printf("   Compiler: %s %s\n", 
#ifdef __GNUC__
           "GCC", __VERSION__
#elif defined(__clang__)
           "Clang", __VERSION__
#elif defined(_MSC_VER)
           "MSVC", "Unknown"
#else
           "Unknown", "Unknown"
#endif
    );
    printf("   CPU Cores: %d\n", get_cpu_cores());
    printf("   Threads: %d\n", NUM_THREADS);
    printf("   AVX2 Support: %s\n", 
#ifdef __AVX2__
           "Yes"
#else
           "No"
#endif
    );
    printf("\n");
    
    // Parse command line arguments
    if (argc > 1) {
        NUM_USERS = strtoul(argv[1], NULL, 10);
    }
    
    int cores = get_cpu_cores();
    NUM_THREADS = (cores > 8) ? 8 : cores;
    
    printf("Processing %zu users\n", NUM_USERS);
    printf("C provides maximum performance with manual optimization!\n");
    printf("\n");
    
    // 🏗️ DATA CREATION
    printf("🏗️ Creating test data...\n");
    double start_time = get_time_ms();
    
    // Traditional Array of Structs
#ifdef _WIN32
    User *users = _aligned_malloc(NUM_USERS * sizeof(User), 64);
#else
    User *users = aligned_alloc(64, NUM_USERS * sizeof(User));
#endif
    if (!users) {
        fprintf(stderr, "Failed to allocate memory for users\n");
        return 1;
    }
    
    for (size_t i = 0; i < NUM_USERS; i++) {
        users[i].id = (uint32_t)i;
        snprintf(users[i].name, sizeof(users[i].name), "User %zu", i);
        users[i].age = (uint8_t)(i % 100);
    }
    
    // Struct of Arrays
    UserSoA *users_soa = create_user_soa(NUM_USERS);
    if (!users_soa) {
        fprintf(stderr, "Failed to allocate memory for SoA\n");
        free(users);
        return 1;
    }
    
    for (size_t i = 0; i < NUM_USERS; i++) {
        users_soa->ids[i] = (uint32_t)i;
        snprintf(users_soa->names[i], 32, "User %zu", i);
        users_soa->ages[i] = (uint8_t)(i % 100);
    }
    
    double end_time = get_time_ms();
    printf("Data creation: %.0fms\n", end_time - start_time);
    printf("\n");
    
    // 🚀 BENCHMARKS
    printf("🚀 Running benchmarks...\n");
    printf("\n");
    
    BenchmarkResult results[16];
    int result_count = 0;
    
    // Basic approaches
    results[result_count++] = benchmark_aos("C AoS Basic", sum_ages_basic, users, NUM_USERS);
    results[result_count++] = benchmark("C SoA Basic", sum_ages_soa, users_soa);
    
    // Optimized approaches
    results[result_count++] = benchmark("C Unrolled", sum_ages_unrolled, users_soa);
    results[result_count++] = benchmark("C Pointer", sum_ages_pointer, users_soa);
    results[result_count++] = benchmark("C AVX2", sum_ages_avx, users_soa);
#ifdef __AVX512BW__
    results[result_count++] = benchmark("C AVX-512", sum_ages_avx512, users_soa);
#endif
    
    // Parallel approaches
    results[result_count++] = benchmark("C Threads", sum_ages_threads, users_soa);
    results[result_count++] = benchmark("C Threads Unrolled", sum_ages_threads_unrolled, users_soa);
    
    // 📊 RESULTS
    printf("📊 RESULTS:\n");
    printf("\n");
    
    // Sort by performance (fastest first)
    qsort(results, result_count, sizeof(BenchmarkResult), compare_results);
    
    double fastest = results[0].time_ms;
    
    const char *emojis[] = {"🥇", "🥈", "🥉", "🔸"};
    
    for (int i = 0; i < result_count; i++) {
        const char *emoji = i < 3 ? emojis[i] : emojis[3];
        double speedup = fastest / results[i].time_ms;
        printf("%s %s: %.3fms (%.1fx)\n", emoji, results[i].name, results[i].time_ms, speedup);
    }
    
    printf("\n");
    printf("🎯 C PERFORMANCE INSIGHTS:\n");
    printf("   • Manual memory management eliminates GC overhead\n");
    printf("   • SIMD intrinsics provide vectorized operations\n");
    printf("   • Loop unrolling reduces branching overhead\n");
    printf("   • pthread provides efficient multithreading\n");
    printf("   • Cache-aligned data improves memory access\n");
    printf("   • Compiler optimizations (-O3) are crucial\n");
    printf("\n");
    
    // Verify all results are the same
    uint64_t expected_sum = 0;
    size_t complete_cycles = NUM_USERS / 100;
    size_t remainder = NUM_USERS % 100;
    expected_sum = complete_cycles * 4950 + (remainder * (remainder - 1)) / 2;
    
    int all_match = 1;
    for (int i = 0; i < result_count; i++) {
        if (results[i].result != expected_sum) {
            all_match = 0;
            break;
        }
    }
    
    if (all_match) {
        printf("✅ Verification: All results match!\n");
    } else {
        printf("❌ Verification: ERROR: Results don't match!\n");
    }
    printf("   Expected sum: %lu\n", expected_sum);
    printf("   Actual results: %lu\n", results[0].result);
    printf("\n");
    
    // Performance summary
    double best_time = results[0].time_ms;
    printf("🏆 C CHAMPION: %s\n", results[0].name);
    printf("⚡ Best time: %.3fms\n", best_time);
    printf("🚀 Elements per second: %.0f\n", (double)NUM_USERS / (best_time / 1000.0));
    printf("\n");
    
    // Write results to file
    FILE *file = fopen("blazing_results_c.txt", "w");
    if (file) {
        fprintf(file, "🚀⚡ C BLAZING FAST BENCHMARK RESULTS ⚡🚀\n");
        fprintf(file, "\n");
        fprintf(file, "Compiler: %s\n", 
#ifdef __GNUC__
                "GCC " __VERSION__
#elif defined(__clang__)
                "Clang " __VERSION__
#else
                "Unknown"
#endif
        );
        fprintf(file, "CPU Cores: %d\n", get_cpu_cores());
        fprintf(file, "Elements: %zu\n", NUM_USERS);
        fprintf(file, "\n");
        
        for (int i = 0; i < result_count; i++) {
            fprintf(file, "%s: %.3fms (result: %lu)\n", 
                    results[i].name, results[i].time_ms, results[i].result);
        }
        
        fprintf(file, "\n");
        fprintf(file, "Best: %s - %.3fms\n", results[0].name, best_time);
        
        fclose(file);
        printf("📝 Results saved to blazing_results_c.txt\n");
    } else {
        printf("❌ Failed to save results\n");
    }
    
    // Cleanup
#ifdef _WIN32
    _aligned_free(users);
#else
    free(users);
#endif
    free_user_soa(users_soa);
    
    printf("\n");
    printf("🎉 C benchmark complete!\n");
    
    return 0;
}
