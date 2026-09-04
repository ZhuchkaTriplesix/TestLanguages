#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

// 🚀⚡ C BLAZING FAST BENCHMARK SUITE ⚡🚀
// Simplified version for maximum compatibility

// User represents a user with traditional struct layout
typedef struct {
    uint32_t id;
    char name[32];
    uint8_t age;
} User;

// UserSoA represents users in Struct of Arrays layout
typedef struct {
    uint32_t *ids;
    char (*names)[32];
    uint8_t *ages;
    size_t count;
} UserSoA;

// Benchmark result structure
typedef struct {
    char name[64];
    double time_ms;
    uint64_t result;
} BenchmarkResult;

// Global configuration
static size_t NUM_USERS = 1000000;

// 🛠️ UTILITY FUNCTIONS 🛠️

// Get current time in milliseconds (simplified)
double get_time_ms() {
    return (double)clock() * 1000.0 / CLOCKS_PER_SEC;
}

// Create UserSoA with given capacity
UserSoA* create_user_soa(size_t capacity) {
    UserSoA *soa = malloc(sizeof(UserSoA));
    if (!soa) return NULL;
    
    soa->ids = malloc(capacity * sizeof(uint32_t));
    soa->names = malloc(capacity * sizeof(char[32]));
    soa->ages = malloc(capacity * sizeof(uint8_t));
    
    if (!soa->ids || !soa->names || !soa->ages) {
        free(soa->ids);
        free(soa->names);
        free(soa->ages);
        free(soa);
        return NULL;
    }
    
    soa->count = capacity;
    return soa;
}

// Free UserSoA
void free_user_soa(UserSoA *soa) {
    if (soa) {
        free(soa->ids);
        free(soa->names);
        free(soa->ages);
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
    
    // Process 8 elements at a time
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

// SumAgesOptimized - combined optimizations
uint64_t sum_ages_optimized(UserSoA *users) {
    uint64_t sum = 0;
    uint8_t *ages = users->ages;
    size_t count = users->count;
    
    // Process 16 elements at a time for maximum unrolling
    size_t i = 0;
    for (; i + 16 <= count; i += 16) {
        sum += (uint64_t)ages[i] + (uint64_t)ages[i+1] + (uint64_t)ages[i+2] + (uint64_t)ages[i+3] +
               (uint64_t)ages[i+4] + (uint64_t)ages[i+5] + (uint64_t)ages[i+6] + (uint64_t)ages[i+7] +
               (uint64_t)ages[i+8] + (uint64_t)ages[i+9] + (uint64_t)ages[i+10] + (uint64_t)ages[i+11] +
               (uint64_t)ages[i+12] + (uint64_t)ages[i+13] + (uint64_t)ages[i+14] + (uint64_t)ages[i+15];
    }
    
    // Process remaining 8-element chunks
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

// 📊 BENCHMARK FRAMEWORK 📊

// Benchmark a function and measure its performance
BenchmarkResult benchmark(const char *name, uint64_t (*func)(UserSoA*), UserSoA *data) {
    BenchmarkResult result;
    strncpy(result.name, name, sizeof(result.name) - 1);
    result.name[sizeof(result.name) - 1] = '\0';
    
    // Warmup
    func(data);
    
    // Measure multiple times for accuracy
    double start = get_time_ms();
    result.result = func(data);
    result.result = func(data);
    result.result = func(data);
    double end = get_time_ms();
    
    result.time_ms = (end - start) / 3.0;
    return result;
}

// Benchmark basic AoS function
BenchmarkResult benchmark_aos(const char *name, uint64_t (*func)(User*, size_t), User *data, size_t count) {
    BenchmarkResult result;
    strncpy(result.name, name, sizeof(result.name) - 1);
    result.name[sizeof(result.name) - 1] = '\0';
    
    // Warmup
    func(data, count);
    
    // Measure multiple times for accuracy
    double start = get_time_ms();
    result.result = func(data, count);
    result.result = func(data, count);
    result.result = func(data, count);
    double end = get_time_ms();
    
    result.time_ms = (end - start) / 3.0;
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
    printf("   Compiler: %s\n", 
#ifdef __GNUC__
           "GCC"
#elif defined(__clang__)
           "Clang"
#elif defined(_MSC_VER)
           "MSVC"
#else
           "Unknown"
#endif
    );
    printf("   Platform: %s\n", 
#ifdef _WIN32
           "Windows"
#elif defined(__linux__)
           "Linux"
#elif defined(__APPLE__)
           "macOS"
#else
           "Unknown"
#endif
    );
    printf("\n");
    
    // Parse command line arguments
    if (argc > 1) {
        NUM_USERS = strtoul(argv[1], NULL, 10);
        if (NUM_USERS == 0) NUM_USERS = 1000000;
    }
    
    printf("Processing %zu users\n", NUM_USERS);
    printf("C provides maximum performance with manual optimization!\n");
    printf("\n");
    
    // 🏗️ DATA CREATION
    printf("🏗️ Creating test data...\n");
    double start_time = get_time_ms();
    
    // Traditional Array of Structs
    User *users = malloc(NUM_USERS * sizeof(User));
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
    
    BenchmarkResult results[6];
    int result_count = 0;
    
    // Basic approaches
    results[result_count++] = benchmark_aos("C AoS Basic", sum_ages_basic, users, NUM_USERS);
    results[result_count++] = benchmark("C SoA Basic", sum_ages_soa, users_soa);
    
    // Optimized approaches
    results[result_count++] = benchmark("C Unrolled 8x", sum_ages_unrolled, users_soa);
    results[result_count++] = benchmark("C Pointer", sum_ages_pointer, users_soa);
    results[result_count++] = benchmark("C Optimized 16x", sum_ages_optimized, users_soa);
    
    // 📊 RESULTS
    printf("📊 RESULTS:\n");
    printf("\n");
    
    // Sort by performance (fastest first)
    qsort(results, result_count, sizeof(BenchmarkResult), compare_results);
    
    double fastest = results[0].time_ms;
    
    const char *emojis[] = {"🥇", "🥈", "🥉", "🔸"};
    
    for (int i = 0; i < result_count; i++) {
        const char *emoji = i < 3 ? emojis[i] : emojis[3];
        double speedup = fastest > 0 ? fastest / results[i].time_ms : 1.0;
        printf("%s %s: %.3fms (%.1fx)\n", emoji, results[i].name, results[i].time_ms, speedup);
    }
    
    printf("\n");
    printf("🎯 C PERFORMANCE INSIGHTS:\n");
    printf("   • Manual memory management eliminates GC overhead\n");
    printf("   • Loop unrolling reduces branching overhead\n");
    printf("   • Struct of Arrays improves cache locality\n");
    printf("   • Pointer arithmetic can be faster than array indexing\n");
    printf("   • Compiler optimizations (-O2, -O3) are crucial\n");
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
    printf("   Expected sum: %llu\n", (unsigned long long)expected_sum);
    printf("   Actual results: %llu\n", (unsigned long long)results[0].result);
    printf("\n");
    
    // Performance summary
    double best_time = results[0].time_ms;
    printf("🏆 C CHAMPION: %s\n", results[0].name);
    printf("⚡ Best time: %.3fms\n", best_time);
    if (best_time > 0) {
        printf("🚀 Elements per second: %.0f\n", (double)NUM_USERS / (best_time / 1000.0));
    }
    printf("\n");
    
    // Write results to file
    FILE *file = fopen("blazing_results_c_simple.txt", "w");
    if (file) {
        fprintf(file, "🚀⚡ C BLAZING FAST BENCHMARK RESULTS (Simple) ⚡🚀\n");
        fprintf(file, "\n");
        fprintf(file, "Elements: %zu\n", NUM_USERS);
        fprintf(file, "\n");
        
        for (int i = 0; i < result_count; i++) {
            fprintf(file, "%s: %.3fms (result: %llu)\n", 
                    results[i].name, results[i].time_ms, (unsigned long long)results[i].result);
        }
        
        fprintf(file, "\n");
        fprintf(file, "Best: %s - %.3fms\n", results[0].name, best_time);
        
        fclose(file);
        printf("📝 Results saved to blazing_results_c_simple.txt\n");
    } else {
        printf("❌ Failed to save results\n");
    }
    
    // Cleanup
    free(users);
    free_user_soa(users_soa);
    
    printf("\n");
    printf("🎉 C benchmark complete!\n");
    
    return 0;
}

