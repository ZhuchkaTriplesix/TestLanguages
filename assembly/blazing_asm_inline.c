#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <omp.h>
#include <pthread.h>
#include <stdatomic.h>

// 🚀⚡ ASSEMBLY INLINE BLAZING FAST BENCHMARK ⚡🚀
//
// Features:
// - Inline Assembly for maximum performance
// - Direct register manipulation
// - SIMD instructions (SSE2/AVX2)
// - Zero function call overhead
// - Cache-optimized access patterns
// - Loop unrolling

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

// 🔥 BLAZING ASSEMBLY IMPLEMENTATIONS 🔥

// Ultra-optimized inline assembly - basic version
uint64_t sum_ages_asm_basic(uint8_t *data, size_t count) {
    uint64_t result;
    
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
    
    return result;
}

// Ultra-optimized inline assembly - unrolled version (8x)
uint64_t sum_ages_asm_unrolled(uint8_t *data, size_t count) {
    uint64_t result;
    
    __asm__ volatile (
        "xorq %%rax, %%rax\n\t"         // sum = 0
        "xorq %%rcx, %%rcx\n\t"         // i = 0
        "movq %2, %%rdx\n\t"            // rdx = count
        "andq $-8, %%rdx\n\t"           // rdx = count & ~7 (round down to multiple of 8)
        
        // Main unrolled loop (8 elements per iteration)
        "1:\n\t"                        // unrolled_loop label
        "cmpq %%rdx, %%rcx\n\t"         // compare i with rounded count
        "jge 3f\n\t"                    // jump to remainder handling
        
        // Load and sum 8 bytes in parallel
        "movzbl 0(%1,%%rcx,1), %%r8d\n\t"  // r8 = data[i]
        "movzbl 1(%1,%%rcx,1), %%r9d\n\t"  // r9 = data[i+1]
        "movzbl 2(%1,%%rcx,1), %%r10d\n\t" // r10 = data[i+2]
        "movzbl 3(%1,%%rcx,1), %%r11d\n\t" // r11 = data[i+3]
        
        "addq %%r8, %%rax\n\t"          // sum += data[i]
        "addq %%r9, %%rax\n\t"          // sum += data[i+1]
        "addq %%r10, %%rax\n\t"         // sum += data[i+2]
        "addq %%r11, %%rax\n\t"         // sum += data[i+3]
        
        "movzbl 4(%1,%%rcx,1), %%r8d\n\t"  // r8 = data[i+4]
        "movzbl 5(%1,%%rcx,1), %%r9d\n\t"  // r9 = data[i+5]
        "movzbl 6(%1,%%rcx,1), %%r10d\n\t" // r10 = data[i+6]
        "movzbl 7(%1,%%rcx,1), %%r11d\n\t" // r11 = data[i+7]
        
        "addq %%r8, %%rax\n\t"          // sum += data[i+4]
        "addq %%r9, %%rax\n\t"          // sum += data[i+5]
        "addq %%r10, %%rax\n\t"         // sum += data[i+6]
        "addq %%r11, %%rax\n\t"         // sum += data[i+7]
        
        "addq $8, %%rcx\n\t"            // i += 8
        "jmp 1b\n\t"                    // jump back to unrolled loop
        
        // Handle remaining elements (< 8)
        "3:\n\t"                        // remainder label
        "cmpq %2, %%rcx\n\t"            // compare i with count
        "jge 2f\n\t"                    // jump if i >= count
        "movzbl (%1,%%rcx,1), %%edx\n\t" // load data[i]
        "addq %%rdx, %%rax\n\t"         // sum += data[i]
        "incq %%rcx\n\t"                // i++
        "jmp 3b\n\t"                    // jump back to remainder
        
        "2:\n\t"                        // done label
        : "=a" (result)                 // output: result in rax
        : "r" (data), "r" (count)       // input: data pointer, count
        : "rcx", "rdx", "r8", "r9", "r10", "r11", "memory" // clobbered
    );
    
    return result;
}

// SIMD version with SSE2 (16 bytes at once)
uint64_t sum_ages_asm_sse2(uint8_t *data, size_t count) {
    uint64_t result;
    
    if (count < 16) {
        return sum_ages_asm_unrolled(data, count);
    }
    
    __asm__ volatile (
        "xorq %%rax, %%rax\n\t"         // sum = 0
        "xorq %%rcx, %%rcx\n\t"         // i = 0
        "pxor %%xmm0, %%xmm0\n\t"       // Clear accumulator
        "pxor %%xmm1, %%xmm1\n\t"       // Clear helper register
        
        "movq %2, %%rdx\n\t"            // rdx = count
        "andq $-16, %%rdx\n\t"          // rdx = count & ~15 (round down to multiple of 16)
        
        // Main SIMD loop (16 elements per iteration)
        "1:\n\t"                        // simd_loop label
        "cmpq %%rdx, %%rcx\n\t"         // compare i with rounded count
        "jge 3f\n\t"                    // jump to remainder handling
        
        // Load 16 bytes and compute sum of bytes per 8-byte group with psadbw
        "movdqu (%1,%%rcx,1), %%xmm2\n\t"   // Load 16 bytes unaligned
        "psadbw %%xmm1, %%xmm2\n\t"         // Sum each 8-byte group against zero
        "paddq %%xmm2, %%xmm0\n\t"          // 64-bit accumulation
        
        "addq $16, %%rcx\n\t"           // i += 16
        "jmp 1b\n\t"                    // jump back to SIMD loop
        
        // Extract sum from SIMD register (64-bit)
        "3:\n\t"                        // extract label
        "movdqa %%xmm0, %%xmm1\n\t"         // Copy for horizontal add
        "psrldq $8, %%xmm1\n\t"             // Shift high 64 bits to low
        "paddq %%xmm1, %%xmm0\n\t"          // Add high and low 64-bit parts
        "movq %%xmm0, %%rax\n\t"            // Extract final 64-bit sum into rax
        
        // Handle remaining elements (< 16)
        "4:\n\t"                        // remainder label
        "cmpq %2, %%rcx\n\t"            // compare i with count
        "jge 2f\n\t"                    // jump if i >= count
        "movzbl (%1,%%rcx,1), %%edx\n\t" // load data[i]
        "addq %%rdx, %%rax\n\t"         // sum += data[i]
        "incq %%rcx\n\t"                // i++
        "jmp 4b\n\t"                    // jump back to remainder
        
        "2:\n\t"                        // done label
        : "=a" (result)                 // output: result in rax
        : "r" (data), "r" (count)       // input: data pointer, count
        : "rcx", "rdx", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "memory"
    );
    
    return result;
}

// AVX2 inline assembly implementation (256-bit VPSADBW)
uint64_t sum_ages_asm_avx2(uint8_t *data, size_t count) {
    uint64_t result;
    __asm__ volatile (
        "vpxor %%ymm0, %%ymm0, %%ymm0\n\t"   // acc = 0
        "vpxor %%ymm1, %%ymm1, %%ymm1\n\t"   // zero = 0
        "xorq %%rcx, %%rcx\n\t"              // i = 0
        "movq %2, %%rdx\n\t"
        "andq $-32, %%rdx\n\t"
        
        "1:\n\t"
        "cmpq %%rdx, %%rcx\n\t"
        "jge 3f\n\t"
        
        "vmovdqu (%1,%%rcx,1), %%ymm2\n\t"
        "vpsadbw %%ymm1, %%ymm2, %%ymm2\n\t"
        "vpaddq %%ymm2, %%ymm0, %%ymm0\n\t"
        
        "addq $32, %%rcx\n\t"
        "jmp 1b\n\t"
        
        "3:\n\t"
        "vextracti128 $1, %%ymm0, %%xmm1\n\t"
        "vpaddq %%xmm1, %%xmm0, %%xmm0\n\t"
        "vmovdqa %%xmm0, %%xmm1\n\t"
        "vpsrldq $8, %%xmm1, %%xmm1\n\t"
        "vpaddq %%xmm1, %%xmm0, %%xmm0\n\t"
        "vmovq %%xmm0, %%rax\n\t"
        "vzeroupper\n\t"
        
        "4:\n\t"
        "cmpq %2, %%rcx\n\t"
        "jge 2f\n\t"
        "movzbl (%1,%%rcx,1), %%edx\n\t"
        "addq %%rdx, %%rax\n\t"
        "incq %%rcx\n\t"
        "jmp 4b\n\t"
        
        "2:\n\t"
        : "=a" (result)
        : "r" (data), "r" (count)
        : "rcx", "rdx", "ymm0", "ymm1", "ymm2", "memory"
    );
    return result;
}

#ifdef __AVX512BW__
// AVX-512 inline assembly implementation (512-bit ZMM VPSADBW, 256-byte unrolled)
uint64_t sum_ages_asm_avx512(uint8_t *data, size_t count) {
    uint64_t result;
    __asm__ volatile (
        "vpxord %%zmm0, %%zmm0, %%zmm0\n\t"   // acc0 = 0
        "vpxord %%zmm1, %%zmm1, %%zmm1\n\t"   // acc1 = 0
        "vpxord %%zmm2, %%zmm2, %%zmm2\n\t"   // acc2 = 0
        "vpxord %%zmm3, %%zmm3, %%zmm3\n\t"   // acc3 = 0
        "vpxord %%zmm8, %%zmm8, %%zmm8\n\t"   // zero = 0
        "xorq %%rcx, %%rcx\n\t"              // i = 0
        "movq %2, %%rdx\n\t"
        "andq $-256, %%rdx\n\t"
        
        "1:\n\t"
        "cmpq %%rdx, %%rcx\n\t"
        "jge 3f\n\t"
        
        "vmovdqu8 (%1,%%rcx,1), %%zmm4\n\t"
        "vmovdqu8 64(%1,%%rcx,1), %%zmm5\n\t"
        "vmovdqu8 128(%1,%%rcx,1), %%zmm6\n\t"
        "vmovdqu8 192(%1,%%rcx,1), %%zmm7\n\t"
        
        "vpsadbw %%zmm8, %%zmm4, %%zmm4\n\t"
        "vpsadbw %%zmm8, %%zmm5, %%zmm5\n\t"
        "vpsadbw %%zmm8, %%zmm6, %%zmm6\n\t"
        "vpsadbw %%zmm8, %%zmm7, %%zmm7\n\t"
        
        "vpaddq %%zmm4, %%zmm0, %%zmm0\n\t"
        "vpaddq %%zmm5, %%zmm1, %%zmm1\n\t"
        "vpaddq %%zmm6, %%zmm2, %%zmm2\n\t"
        "vpaddq %%zmm7, %%zmm3, %%zmm3\n\t"
        
        "addq $256, %%rcx\n\t"
        "jmp 1b\n\t"
        
        "3:\n\t"
        "vpaddq %%zmm1, %%zmm0, %%zmm0\n\t"
        "vpaddq %%zmm3, %%zmm2, %%zmm2\n\t"
        "vpaddq %%zmm2, %%zmm0, %%zmm0\n\t"
        
        "movq %2, %%rdx\n\t"
        "andq $-64, %%rdx\n\t"
        "5:\n\t"
        "cmpq %%rdx, %%rcx\n\t"
        "jge 6f\n\t"
        "vmovdqu8 (%1,%%rcx,1), %%zmm4\n\t"
        "vpsadbw %%zmm8, %%zmm4, %%zmm4\n\t"
        "vpaddq %%zmm4, %%zmm0, %%zmm0\n\t"
        "addq $64, %%rcx\n\t"
        "jmp 5b\n\t"
        
        "6:\n\t"
        "vextracti64x4 $1, %%zmm0, %%ymm1\n\t"
        "vpaddq %%ymm1, %%ymm0, %%ymm0\n\t"
        "vextracti128 $1, %%ymm0, %%xmm1\n\t"
        "vpaddq %%xmm1, %%xmm0, %%xmm0\n\t"
        "vmovdqa %%xmm0, %%xmm1\n\t"
        "vpsrldq $8, %%xmm1, %%xmm1\n\t"
        "vpaddq %%xmm1, %%xmm0, %%xmm0\n\t"
        "vmovq %%xmm0, %%rax\n\t"
        "vzeroupper\n\t"
        
        "4:\n\t"
        "cmpq %2, %%rcx\n\t"
        "jge 2f\n\t"
        "movzbl (%1,%%rcx,1), %%edx\n\t"
        "addq %%rdx, %%rax\n\t"
        "incq %%rcx\n\t"
        "jmp 4b\n\t"
        
        "2:\n\t"
        : "=a" (result)
        : "r" (data), "r" (count)
        : "rcx", "rdx", "zmm0", "zmm1", "zmm2", "zmm3", "zmm4", "zmm5", "zmm6", "zmm7", "zmm8", "memory"
    );
    return result;
}

// Multi-threaded Assembly with OpenMP (8 cores)
uint64_t sum_ages_asm_parallel_avx512(uint8_t *data, size_t count) {
    uint64_t total_sum = 0;
    #pragma omp parallel num_threads(8) reduction(+:total_sum)
    {
        int tid = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        size_t raw_chunk = count / num_threads;
        size_t aligned_chunk = (raw_chunk + 63) & ~63;
        size_t start = tid * aligned_chunk;
        if (start > count) start = count;
        size_t end = (tid == num_threads - 1) ? count : (start + aligned_chunk);
        if (end > count) end = count;
        if (end > start) {
            total_sum += sum_ages_asm_avx512(data + start, end - start);
        }
    }
    return total_sum;
}
// ⚡ PURE AVX-512 EXTREME - 512 BYTES / ITERATION, MEMORY-FUSED VPSADBW, 8 ZMM ACCUMULATORS ⚡
uint64_t sum_ages_asm_avx512_extreme(uint8_t *data, size_t count) {
    uint64_t result;
    __asm__ volatile (
        "vpxord %%zmm0, %%zmm0, %%zmm0\n\t"   // acc0
        "vpxord %%zmm1, %%zmm1, %%zmm1\n\t"   // acc1
        "vpxord %%zmm2, %%zmm2, %%zmm2\n\t"   // acc2
        "vpxord %%zmm3, %%zmm3, %%zmm3\n\t"   // acc3
        "vpxord %%zmm4, %%zmm4, %%zmm4\n\t"   // acc4
        "vpxord %%zmm5, %%zmm5, %%zmm5\n\t"   // acc5
        "vpxord %%zmm6, %%zmm6, %%zmm6\n\t"   // acc6
        "vpxord %%zmm7, %%zmm7, %%zmm7\n\t"   // acc7
        "vpxord %%zmm16, %%zmm16, %%zmm16\n\t" // zero register
        
        "movq %1, %%rax\n\t"                 // current ptr = data
        "movq %2, %%rdx\n\t"                 // count
        "andq $-512, %%rdx\n\t"              // rounded to 512 bytes
        "addq %1, %%rdx\n\t"                 // end ptr = data + rounded_count
        
        "cmpq %%rdx, %%rax\n\t"              // if data >= end ptr
        "jae 3f\n\t"
        
        // Main 512-byte unrolled loop (8 x 64 bytes) with prefetch and direct memory operands
        ".p2align 4\n\t"
        "1:\n\t"
        "prefetcht0 1024(%%rax)\n\t"
        
        "vpsadbw (%%rax), %%zmm16, %%zmm8\n\t"
        "vpsadbw 64(%%rax), %%zmm16, %%zmm9\n\t"
        "vpsadbw 128(%%rax), %%zmm16, %%zmm10\n\t"
        "vpsadbw 192(%%rax), %%zmm16, %%zmm11\n\t"
        "vpsadbw 256(%%rax), %%zmm16, %%zmm12\n\t"
        "vpsadbw 320(%%rax), %%zmm16, %%zmm13\n\t"
        "vpsadbw 384(%%rax), %%zmm16, %%zmm14\n\t"
        "vpsadbw 448(%%rax), %%zmm16, %%zmm15\n\t"
        
        "vpaddq %%zmm8, %%zmm0, %%zmm0\n\t"
        "vpaddq %%zmm9, %%zmm1, %%zmm1\n\t"
        "vpaddq %%zmm10, %%zmm2, %%zmm2\n\t"
        "vpaddq %%zmm11, %%zmm3, %%zmm3\n\t"
        "vpaddq %%zmm12, %%zmm4, %%zmm4\n\t"
        "vpaddq %%zmm13, %%zmm5, %%zmm5\n\t"
        "vpaddq %%zmm14, %%zmm6, %%zmm6\n\t"
        "vpaddq %%zmm15, %%zmm7, %%zmm7\n\t"
        
        "addq $512, %%rax\n\t"
        "cmpq %%rdx, %%rax\n\t"
        "jb 1b\n\t"
        
        // Sum the 8 accumulators
        "3:\n\t"
        "vpaddq %%zmm1, %%zmm0, %%zmm0\n\t"
        "vpaddq %%zmm3, %%zmm2, %%zmm2\n\t"
        "vpaddq %%zmm5, %%zmm4, %%zmm4\n\t"
        "vpaddq %%zmm7, %%zmm6, %%zmm6\n\t"
        "vpaddq %%zmm2, %%zmm0, %%zmm0\n\t"
        "vpaddq %%zmm6, %%zmm4, %%zmm4\n\t"
        "vpaddq %%zmm4, %%zmm0, %%zmm0\n\t"
        
        // Handle 64-byte chunks
        "movq %2, %%rdx\n\t"
        "andq $-64, %%rdx\n\t"
        "addq %1, %%rdx\n\t"
        "5:\n\t"
        "cmpq %%rdx, %%rax\n\t"
        "jae 6f\n\t"
        "vpsadbw (%%rax), %%zmm16, %%zmm8\n\t"
        "vpaddq %%zmm8, %%zmm0, %%zmm0\n\t"
        "addq $64, %%rax\n\t"
        "jmp 5b\n\t"
        
        // Horizontal reduction of zmm0 to scalar
        "6:\n\t"
        "vextracti64x4 $1, %%zmm0, %%ymm1\n\t"
        "vpaddq %%ymm1, %%ymm0, %%ymm0\n\t"
        "vextracti128 $1, %%ymm0, %%xmm1\n\t"
        "vpaddq %%xmm1, %%xmm0, %%xmm0\n\t"
        "vmovdqa %%xmm0, %%xmm1\n\t"
        "vpsrldq $8, %%xmm1, %%xmm1\n\t"
        "vpaddq %%xmm1, %%xmm0, %%xmm0\n\t"
        "vmovq %%xmm0, %%rcx\n\t"            // sum in rcx
        "vzeroupper\n\t"
        
        // Remaining bytes (< 64)
        "movq %1, %%rdx\n\t"
        "addq %2, %%rdx\n\t"                 // absolute end ptr
        "4:\n\t"
        "cmpq %%rdx, %%rax\n\t"
        "jae 2f\n\t"
        "movzbl (%%rax), %%r8d\n\t"
        "addq %%r8, %%rcx\n\t"
        "incq %%rax\n\t"
        "jmp 4b\n\t"
        
        "2:\n\t"
        "movq %%rcx, %0\n\t"
        : "=r" (result)
        : "r" (data), "r" (count)
        : "rax", "rdx", "rcx", "r8", "zmm0", "zmm1", "zmm2", "zmm3", "zmm4", "zmm5", "zmm6", "zmm7", "zmm8", "zmm9", "zmm10", "zmm11", "zmm12", "zmm13", "zmm14", "zmm15", "zmm16", "memory"
    );
    return result;
}

// 🚀 LOCK-FREE USERSPACE SPIN THREAD POOL FOR ASSEMBLY (SUB-MICROSECOND) 🚀
typedef struct {
    uint8_t *data;
    size_t count;
    _Atomic size_t req_epoch;
    _Atomic size_t ack_epoch;
    uint64_t result;
    char pad[64];
} __attribute__((aligned(64))) AsmWorker;

static AsmWorker asm_workers[7];
static pthread_t asm_threads[7];
static _Atomic size_t asm_pool_epoch = 0;
static int asm_pool_init = 0;

static void* asm_worker_func(void *arg) {
    AsmWorker *w = (AsmWorker*)arg;
    size_t local_epoch = 0;
    while (1) {
        while (atomic_load_explicit(&w->req_epoch, memory_order_acquire) == local_epoch) {
            __builtin_ia32_pause();
        }
        local_epoch = atomic_load_explicit(&w->req_epoch, memory_order_relaxed);
        if (local_epoch == (size_t)-1) break;
        
        w->result = sum_ages_asm_avx512_extreme(w->data, w->count);
        atomic_store_explicit(&w->ack_epoch, local_epoch, memory_order_release);
    }
    return NULL;
}

static void init_asm_spin_pool(void) {
    if (asm_pool_init) return;
    
    // Pin master thread to core 0
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    
    for (int i = 0; i < 7; i++) {
        asm_workers[i].data = NULL;
        asm_workers[i].count = 0;
        atomic_init(&asm_workers[i].req_epoch, 0);
        atomic_init(&asm_workers[i].ack_epoch, 0);
        asm_workers[i].result = 0;
        pthread_create(&asm_threads[i], NULL, asm_worker_func, &asm_workers[i]);
        
        CPU_ZERO(&cpuset);
        CPU_SET(i + 1, &cpuset);
        pthread_setaffinity_np(asm_threads[i], sizeof(cpu_set_t), &cpuset);
    }
    asm_pool_init = 1;
}

uint64_t sum_ages_asm_spin_parallel(uint8_t *data, size_t count) {
    init_asm_spin_pool();
    size_t epoch = atomic_fetch_add_explicit(&asm_pool_epoch, 1, memory_order_relaxed) + 1;
    
    int total_threads = 8;
    size_t raw_chunk = count / total_threads;
    size_t aligned_chunk = (raw_chunk + 63) & ~63;
    
    // Dispatch to 7 workers
    for (int i = 0; i < 7; i++) {
        int chunk_idx = i + 1;
        size_t start = chunk_idx * aligned_chunk;
        if (start > count) start = count;
        size_t end = (chunk_idx == total_threads - 1) ? count : (start + aligned_chunk);
        if (end > count) end = count;
        
        asm_workers[i].data = data + start;
        asm_workers[i].count = end - start;
        atomic_store_explicit(&asm_workers[i].req_epoch, epoch, memory_order_release);
    }
    
    // Master thread computes chunk 0 immediately!
    size_t master_len = aligned_chunk < count ? aligned_chunk : count;
    uint64_t total = sum_ages_asm_avx512_extreme(data, master_len);
    
    // Collect from 7 workers
    for (int i = 0; i < 7; i++) {
        while (atomic_load_explicit(&asm_workers[i].ack_epoch, memory_order_acquire) != epoch) {
            __builtin_ia32_pause();
        }
        total += asm_workers[i].result;
    }
    return total;
}
#endif

// EXTREME version - combines all optimizations
uint64_t sum_ages_asm_extreme(uint8_t *data, size_t count) {
#ifdef __AVX512BW__
    return sum_ages_asm_avx512_extreme(data, count);
#else
    return sum_ages_asm_avx2(data, count);
#endif
}

// Benchmark helper function
double benchmark_asm_function(const char* name, uint64_t (*func)(uint8_t*, size_t), uint8_t* data, size_t count) {
    printf("🔥 Testing %s...\n", name);
    
    // Warmup runs
    for (int w = 0; w < 5; w++) {
        func(data, count);
    }

    double best = 1e9;
    uint64_t result = 0;
    for (int it = 0; it < 30; it++) {
        __asm__ volatile("" : : "r"(data) : "memory");
        double start = get_time_ms();
        result = func(data, count);
        __asm__ volatile("" : "+r"(result) : : "memory");
        double end = get_time_ms();
        double dt = end - start;
        if (dt < best) best = dt;
    }
    
    printf("   Result: %llu in %.3f ms (%.1f µs)\n", (unsigned long long)result, best, best * 1000.0);
    return best;
}

int main(int argc, char *argv[]) {
    printf("🚀⚡ ASSEMBLY BLAZING FAST BENCHMARK SUITE ⚡🚀\n\n");
    
    // Parse arguments
    size_t num_users = 1000000; // Default 1M
    if (argc > 1) {
        num_users = atoll(argv[1]);
    }
    
    printf("📊 Testing with %zu users...\n\n", num_users);
    
    // Allocate and initialize test data (64-byte aligned)
    uint8_t *ages = (uint8_t*)aligned_alloc(64, num_users);
    if (!ages) {
        fprintf(stderr, "❌ Memory allocation failed!\n");
        return 1;
    }
    
    // Initialize with deterministic age data (0-99)
    for (size_t i = 0; i < num_users; i++) {
        ages[i] = (uint8_t)(i % 100);
    }
    
    printf("🎯 ASSEMBLY PERFORMANCE TESTS:\n\n");
    
    // Run benchmarks
    double time_basic = benchmark_asm_function(
        "Assembly Basic", sum_ages_asm_basic, ages, num_users);
    
    double time_unrolled = benchmark_asm_function(
        "Assembly Unrolled 8x", sum_ages_asm_unrolled, ages, num_users);
    
    double time_sse2 = benchmark_asm_function(
        "Assembly SSE2 SIMD", sum_ages_asm_sse2, ages, num_users);

    double time_avx2 = benchmark_asm_function(
        "Assembly AVX2 SIMD", sum_ages_asm_avx2, ages, num_users);

#ifdef __AVX512BW__
    double time_avx512 = benchmark_asm_function(
        "Assembly AVX-512 SIMD", sum_ages_asm_avx512, ages, num_users);
#endif
    
    double time_extreme = benchmark_asm_function(
        "Assembly EXTREME", sum_ages_asm_extreme, ages, num_users);

#ifdef __AVX512BW__
    double time_parallel = benchmark_asm_function(
        "Assembly OpenMP Parallel (8 Cores)", sum_ages_asm_parallel_avx512, ages, num_users);
    double time_spin = benchmark_asm_function(
        "Assembly SpinPool Lock-Free (8 Cores)", sum_ages_asm_spin_parallel, ages, num_users);
#endif
    
    // Calculate speedups
    printf("\n🚀 ASSEMBLY SPEEDUP ANALYSIS:\n\n");
    printf("Unrolled vs Basic:   %.2fx faster\n", time_basic / time_unrolled);
    printf("SSE2 vs Basic:       %.2fx faster\n", time_basic / time_sse2);
    printf("AVX2 vs Basic:       %.2fx faster\n", time_basic / time_avx2);
#ifdef __AVX512BW__
    printf("AVX-512 vs Basic:    %.2fx faster\n", time_basic / time_avx512);
    printf("OpenMP vs Basic:     %.2fx faster\n", time_basic / time_parallel);
    printf("SpinPool vs Basic:   %.2fx faster\n", time_basic / time_spin);
#endif
    printf("EXTREME vs Basic:    %.2fx faster\n", time_basic / time_extreme);
    
    printf("\n⚡ ESTIMATED PERFORMANCE:\n");
    printf("Basic:     ~%.3f ms/M elements\n", time_basic * 1000000.0 / num_users);
    printf("AVX2:      ~%.3f ms/M elements (%.1f µs)\n", time_avx2 * 1000000.0 / num_users, time_avx2 * 1000.0);
#ifdef __AVX512BW__
    printf("AVX-512:   ~%.3f ms/M elements (%.1f µs)\n", time_avx512 * 1000000.0 / num_users, time_avx512 * 1000.0);
#endif
    
    printf("\n💥 Assembly achieves ULTIMATE PERFORMANCE!\n");
    printf("Expected speedup vs C: 2-5x faster\n");
    printf("Expected speedup vs Python: 1000x+ faster\n");
    
    free(ages);
    return 0;
}

