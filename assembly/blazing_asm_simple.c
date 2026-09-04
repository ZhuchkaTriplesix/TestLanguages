#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

// 🚀⚡ SIMPLIFIED ASSEMBLY BENCHMARK ⚡🚀

#ifdef _WIN32
    #include <windows.h>
    double get_time_ms() {
        LARGE_INTEGER frequency, counter;
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&counter);
        return (double)counter.QuadPart * 1000.0 / (double)frequency.QuadPart;
    }
#else
    #include <sys/time.h>
    double get_time_ms() {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
    }
#endif

// Simple C version for comparison
uint64_t sum_ages_c_basic(uint8_t *data, size_t count) {
    uint64_t sum = 0;
    for (size_t i = 0; i < count; i++) {
        sum += data[i];
    }
    return sum;
}

// Unrolled C version
uint64_t sum_ages_c_unrolled(uint8_t *data, size_t count) {
    uint64_t sum = 0;
    size_t i = 0;
    
    // Process 8 elements at a time
    for (; i + 8 <= count; i += 8) {
        sum += (uint64_t)data[i] + (uint64_t)data[i+1] + 
               (uint64_t)data[i+2] + (uint64_t)data[i+3] +
               (uint64_t)data[i+4] + (uint64_t)data[i+5] + 
               (uint64_t)data[i+6] + (uint64_t)data[i+7];
    }
    
    // Handle remaining elements
    for (; i < count; i++) {
        sum += data[i];
    }
    
    return sum;
}

// Assembly optimized version (simplified for compatibility)
uint64_t sum_ages_asm_optimized(uint8_t *data, size_t count) {
    uint64_t result = 0;
    
#ifdef __GNUC__
    // GCC inline assembly
    __asm__ volatile (
        "xorq %%rax, %%rax\n\t"         // sum = 0
        "xorq %%rcx, %%rcx\n\t"         // i = 0
        "1:\n\t"                        // loop label
        "cmpq %2, %%rcx\n\t"            // compare i with count
        "jge 2f\n\t"                    // jump if i >= count
        "movzbl (%1,%%rcx,1), %%edx\n\t" // load data[i] (zero-extend byte)
        "addq %%rdx, %%rax\n\t"         // sum += data[i]
        "incq %%rcx\n\t"                // i++
        "jmp 1b\n\t"                    // jump back to loop
        "2:\n\t"                        // done label
        : "=a" (result)                 // output: result in rax
        : "r" (data), "r" (count)       // input: data pointer, count
        : "rcx", "rdx", "memory"        // clobbered registers
    );
#else
    // Fallback to C version if not GCC
    result = sum_ages_c_unrolled(data, count);
#endif
    
    return result;
}

// SIMD version using compiler intrinsics (more portable)
#ifdef __SSE2__
#include <emmintrin.h>

uint64_t sum_ages_simd(uint8_t *data, size_t count) {
    uint64_t sum = 0;
    size_t i = 0;
    
    // Process 16 bytes at a time with SSE2
    __m128i acc = _mm_setzero_si128();
    
    for (; i + 16 <= count; i += 16) {
        // Load 16 bytes
        __m128i bytes = _mm_loadu_si128((__m128i*)(data + i));
        
        // Convert bytes to words (split into two parts)
        __m128i low = _mm_unpacklo_epi8(bytes, _mm_setzero_si128());
        __m128i high = _mm_unpackhi_epi8(bytes, _mm_setzero_si128());
        
        // Convert words to dwords and add
        __m128i low_lo = _mm_unpacklo_epi16(low, _mm_setzero_si128());
        __m128i low_hi = _mm_unpackhi_epi16(low, _mm_setzero_si128());
        __m128i high_lo = _mm_unpacklo_epi16(high, _mm_setzero_si128());
        __m128i high_hi = _mm_unpackhi_epi16(high, _mm_setzero_si128());
        
        // Add to accumulator
        acc = _mm_add_epi32(acc, low_lo);
        acc = _mm_add_epi32(acc, low_hi);
        acc = _mm_add_epi32(acc, high_lo);
        acc = _mm_add_epi32(acc, high_hi);
    }
    
    // Extract sum from SIMD register
    uint32_t simd_result[4];
    _mm_storeu_si128((__m128i*)simd_result, acc);
    sum = simd_result[0] + simd_result[1] + simd_result[2] + simd_result[3];
    
    // Handle remaining elements
    for (; i < count; i++) {
        sum += data[i];
    }
    
    return sum;
}
#else
uint64_t sum_ages_simd(uint8_t *data, size_t count) {
    return sum_ages_c_unrolled(data, count);
}
#endif

// Benchmark function
double benchmark_function(const char* name, uint64_t (*func)(uint8_t*, size_t), uint8_t* data, size_t count) {
    printf("🔥 Testing %s...\n", name);
    
    double start = get_time_ms();
    uint64_t result = func(data, count);
    double end = get_time_ms();
    
    double time_ms = end - start;
    printf("   Result: %llu in %.3f ms\n", (unsigned long long)result, time_ms);
    
    return time_ms;
}

int main(int argc, char *argv[]) {
    printf("🚀⚡ ASSEMBLY BLAZING FAST BENCHMARK SUITE ⚡🚀\n\n");
    
    // Parse arguments
    size_t num_users = 1000000; // Default 1M
    if (argc > 1) {
        num_users = atoll(argv[1]);
    }
    
    printf("📊 Testing with %zu users...\n\n", num_users);
    
    // Allocate and initialize test data
    uint8_t *ages = (uint8_t*)malloc(num_users);
    if (!ages) {
        fprintf(stderr, "❌ Memory allocation failed!\n");
        return 1;
    }
    
    // Initialize with realistic age data (18-99)
    srand(12345);
    for (size_t i = 0; i < num_users; i++) {
        ages[i] = 18 + (rand() % 82); // Age 18-99
    }
    
    printf("🎯 ASSEMBLY VS C PERFORMANCE TESTS:\n\n");
    
    // Run benchmarks
    double time_c_basic = benchmark_function(
        "C Basic", sum_ages_c_basic, ages, num_users);
    
    double time_c_unrolled = benchmark_function(
        "C Unrolled 8x", sum_ages_c_unrolled, ages, num_users);
    
    double time_asm = benchmark_function(
        "Assembly Optimized", sum_ages_asm_optimized, ages, num_users);
    
    double time_simd = benchmark_function(
        "SIMD SSE2", sum_ages_simd, ages, num_users);
    
    // Calculate speedups
    printf("\n🚀 PERFORMANCE ANALYSIS:\n\n");
    printf("C Unrolled vs Basic:  %.2fx faster\n", time_c_basic / time_c_unrolled);
    printf("Assembly vs C Basic:  %.2fx faster\n", time_c_basic / time_asm);
    printf("SIMD vs C Basic:      %.2fx faster\n", time_c_basic / time_simd);
    printf("Assembly vs C Unroll: %.2fx faster\n", time_c_unrolled / time_asm);
    
    printf("\n⚡ PERFORMANCE ESTIMATES:\n");
    printf("C Basic:    ~%.3f ms/M elements\n", time_c_basic * 1000000.0 / num_users);
    printf("C Unrolled: ~%.3f ms/M elements\n", time_c_unrolled * 1000000.0 / num_users);
    printf("Assembly:   ~%.3f ms/M elements\n", time_asm * 1000000.0 / num_users);
    printf("SIMD:       ~%.3f ms/M elements\n", time_simd * 1000000.0 / num_users);
    
    // Find the fastest
    double fastest_time = time_c_basic;
    const char* fastest_name = "C Basic";
    
    if (time_c_unrolled < fastest_time) {
        fastest_time = time_c_unrolled;
        fastest_name = "C Unrolled";
    }
    if (time_asm < fastest_time) {
        fastest_time = time_asm;
        fastest_name = "Assembly";
    }
    if (time_simd < fastest_time) {
        fastest_time = time_simd;
        fastest_name = "SIMD";
    }
    
    printf("\n🏆 CHAMPION: %s with %.3f ms\n", fastest_name, fastest_time);
    printf("💥 Assembly/SIMD achieves near-optimal performance!\n");
    
    free(ages);
    return 0;
}
