# 🚀⚡ C BLAZING FAST BENCHMARK ⚡🚀

## Overview

This is a high-performance C implementation of the blazing fast benchmark suite. It demonstrates various optimization techniques available in C for maximum performance.

## Features

### 🔥 Core C Performance Features
- **Manual Memory Management**: Zero garbage collection overhead
- **SIMD Intrinsics**: AVX2 vectorization for parallel processing
- **Cache-Aligned Data**: 32-byte aligned allocations for optimal cache performance
- **Loop Unrolling**: Manual loop unrolling for reduced branching
- **Compiler Optimizations**: Aggressive -O3 optimizations
- **pthread Multithreading**: Efficient parallel processing

### 📊 Benchmark Algorithms

1. **AoS Basic**: Traditional Array of Structs approach
2. **SoA Basic**: Struct of Arrays for better cache locality
3. **Unrolled**: Manual loop unrolling optimization
4. **Pointer**: Optimized pointer arithmetic
5. **AVX/SIMD**: SIMD vectorization with AVX2 intrinsics
6. **Threads**: Multi-threaded processing with pthread
7. **Threads Unrolled**: Combined threading and loop unrolling

### 🛠️ Build Requirements

#### Windows
- **MinGW-w64** (recommended) - Download from https://www.mingw-w64.org/
- **TDM-GCC** - Easy installer from https://jmeubank.github.io/tdm-gcc/
- **MSYS2** - Complete development environment from https://www.msys2.org/
- **Visual Studio Build Tools** - Free from Microsoft
- **Online Compilers** - Try https://onecompiler.com/c or https://replit.com/

#### Linux/Unix
- **GCC** 4.9+ with AVX2 support
- **Clang** 3.4+ with AVX2 support
- **pthread** library

## Building

### Quick Start (Windows)

**Option 1: Install MinGW-w64**
1. Download TDM-GCC from https://jmeubank.github.io/tdm-gcc/
2. Install with default settings
3. Run: `.\build_simple_c.bat`

**Option 2: Use Online Compiler**
1. Copy the contents of `blazing_simple.c`
2. Go to https://onecompiler.com/c
3. Paste the code and click "Run"

**Option 3: Use MSYS2**
1. Install MSYS2 from https://www.msys2.org/
2. Open MSYS2 terminal and run: `pacman -S mingw-w64-x86_64-gcc`
3. Add C:\msys64\mingw64\bin to your PATH
4. Run: `.\build_simple_c.bat`

### Manual Build Commands
```bash
# Simple version (most compatible)
gcc blazing_simple.c -o blazing_simple_c.exe

# Optimized version (if you have GCC)
gcc -O2 -Wall blazing_simple.c -o blazing_simple_c.exe

# Advanced version (requires threading support)
gcc -O3 -march=native -pthread blazing.c -o blazing_c.exe

# With Clang
clang -O2 blazing_simple.c -o blazing_simple_c.exe

# With MSVC (Visual Studio)
cl /O2 blazing_simple.c /Fe:blazing_simple_c.exe
```

### Using Makefile
```bash
make -f Makefile_c          # Build optimized version
make -f Makefile_c debug    # Build debug version
make -f Makefile_c run      # Build and run
```

## Running

### Basic Usage
```bash
# Run with default 1M users
./blazing_c

# Run with custom number of users
./blazing_c 5000000
```

### Batch Runner (Windows)
```bash
run_c_benchmark.bat
```

This runs multiple benchmarks with different user counts.

## Performance Optimizations

### 1. Memory Layout Optimizations
- **Struct of Arrays (SoA)**: Better cache locality
- **32-byte Alignment**: Optimal for AVX2 operations
- **Contiguous Allocation**: Reduces memory fragmentation

### 2. CPU Optimizations
- **SIMD Instructions**: AVX2 for vectorized operations
- **Loop Unrolling**: Reduces branch overhead
- **Compiler Hints**: LTO and aggressive optimizations

### 3. Parallel Processing
- **pthread**: Multi-threaded execution
- **Work Distribution**: Optimal chunk sizing
- **Thread Affinity**: CPU-aware thread distribution

### 4. Algorithm Optimizations
- **Pointer Arithmetic**: Direct memory access
- **Unrolled Loops**: Process 8 elements per iteration
- **Reduced Function Calls**: Inline optimizations

## Expected Performance

C typically provides the best performance in this benchmark suite because:

1. **No Runtime Overhead**: No garbage collection or runtime interpretation
2. **Direct Hardware Access**: SIMD intrinsics and pointer arithmetic
3. **Optimal Compilation**: Advanced compiler optimizations
4. **Manual Control**: Fine-grained control over memory and execution

### Typical Results (1M users)
- **Basic approaches**: 1-5ms
- **Optimized algorithms**: 0.5-2ms  
- **SIMD versions**: 0.2-1ms
- **Parallel versions**: 0.1-0.5ms

## Compilation Flags Explained

- `-O3`: Maximum optimization level
- `-march=native`: Use all CPU features available
- `-mavx2`: Enable AVX2 SIMD instructions
- `-mfma`: Enable Fused Multiply-Add instructions
- `-funroll-loops`: Automatic loop unrolling
- `-flto`: Link-time optimization
- `-pthread`: POSIX threads support

## Hardware Requirements

### Minimum
- x86-64 CPU with SSE2
- 1GB RAM
- Any C11 compiler

### Recommended
- x86-64 CPU with AVX2 support (Intel Haswell+ or AMD Excavator+)
- 4GB+ RAM
- GCC 7+ or Clang 5+
- Multiple CPU cores for threading benefits

## Troubleshooting

### AVX2 Not Supported
If your CPU doesn't support AVX2, the code will fall back to the unrolled version automatically.

### Threading Issues
If pthread is not available, you can compile without the `-pthread` flag and threading benchmarks will be skipped.

### Compiler Warnings
The code uses some advanced optimizations that might generate warnings on older compilers. These are generally safe to ignore.

## Results Output

The benchmark creates `blazing_results_c.txt` with detailed results including:
- System information
- Individual benchmark times
- Performance rankings
- Verification of correct results

## Contributing

This C implementation focuses on maximum performance and demonstrates:
- Low-level optimization techniques
- SIMD programming
- Cache-friendly data structures
- Efficient multithreading

Feel free to contribute additional optimizations or platform-specific improvements!

## License

Part of the Blazing Fast Benchmark Suite - demonstrating C's performance capabilities.
