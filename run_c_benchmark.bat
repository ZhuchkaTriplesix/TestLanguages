@echo off
REM 🚀⚡ C BLAZING FAST BENCHMARK RUNNER ⚡🚀

setlocal EnableDelayedExpansion

echo.
echo 🚀⚡ C BLAZING FAST BENCHMARK SUITE ⚡🚀
echo.

REM Check if executable exists
if not exist "blazing_c.exe" (
    echo Building C benchmark first...
    call build_c.bat
    if !errorlevel! neq 0 (
        echo ❌ Build failed!
        exit /b 1
    )
)

echo 📊 RUNNING C BENCHMARKS...
echo.

REM Standard benchmark
echo 🔸 Standard benchmark (1M users):
blazing_c.exe
echo.

REM Small benchmark
echo 🔸 Small benchmark (100K users):
blazing_c.exe 100000
echo.

REM Large benchmark (if system has enough memory)
echo 🔸 Large benchmark (10M users):
blazing_c.exe 10000000
echo.

REM Very large benchmark (if system has lots of memory)
echo 🔸 Extreme benchmark (100M users):
blazing_c.exe 100000000
echo.

echo 🎯 C PERFORMANCE SUMMARY:
echo.
echo C provides the absolute maximum performance because:
echo   • Zero garbage collection overhead
echo   • Direct memory management
echo   • SIMD vectorization (AVX2)
echo   • Aggressive compiler optimizations
echo   • Efficient pthread multithreading
echo   • Cache-aligned data structures
echo   • Manual loop unrolling
echo.
echo 🎉 All C benchmarks complete!

pause

