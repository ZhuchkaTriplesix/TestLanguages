@echo off
REM 🚀⚡ ULTIMATE MULTI-LANGUAGE BENCHMARK SUITE ⚡🚀

setlocal EnableDelayedExpansion

echo.
echo 🚀⚡ BLAZING FAST MULTI-LANGUAGE BENCHMARK SUITE ⚡🚀
echo.
echo 📊 Testing all languages with sizes: 1K, 10K, 100K, 1M, 10M elements
echo.

REM Create results directory
if not exist "results" mkdir results

REM Define test sizes
set SIZES=1000 10000 100000 1000000 10000000
set SIZE_NAMES=1K 10K 100K 1M 10M

echo 🔥 BUILDING ALL IMPLEMENTATIONS...
echo.

REM Build Assembly (if not exists)
if not exist "blazing_asm_final.exe" (
    echo 🔸 Building Assembly...
    gcc -O3 -msse2 -mavx2 -march=native -funroll-loops -Wall -Wextra -std=c11 blazing_asm_final.c -o blazing_asm_final.exe >nul 2>&1
    if !errorlevel! == 0 (
        echo ✅ Assembly build successful
    ) else (
        echo ❌ Assembly build failed
    )
)

REM Build C (if not exists)
if not exist "blazing_c.exe" (
    echo 🔸 Building C...
    gcc -O2 -Wall blazing.c -o blazing_c.exe >nul 2>&1
    if !errorlevel! == 0 (
        echo ✅ C build successful
    ) else (
        echo ❌ C build failed
    )
)

REM Build C++ (if not exists)
if not exist "blazing_cpp.exe" (
    echo 🔸 Building C++...
    g++ -O2 -std=c++17 blazing.cpp -o blazing_cpp.exe >nul 2>&1
    if !errorlevel! == 0 (
        echo ✅ C++ build successful
    ) else (
        echo ❌ C++ build failed
    )
)

REM Build Go (if not exists)
if not exist "blazing_go.exe" (
    echo 🔸 Building Go...
    go build -o blazing_go.exe blazing.go >nul 2>&1
    if !errorlevel! == 0 (
        echo ✅ Go build successful
    ) else (
        echo ❌ Go build failed
    )
)

REM Build Rust (if not exists)
if not exist "target\release\blazing.exe" (
    echo 🔸 Building Rust...
    cargo build --release >nul 2>&1
    if !errorlevel! == 0 (
        echo ✅ Rust build successful
    ) else (
        echo ❌ Rust build failed
    )
)

echo.
echo 🚀 RUNNING COMPREHENSIVE BENCHMARKS...
echo.

REM Initialize results file
echo 🚀⚡ BLAZING FAST MULTI-LANGUAGE BENCHMARK RESULTS ⚡🚀 > results\all_results.txt
echo. >> results\all_results.txt
echo Generated: %date% %time% >> results\all_results.txt
echo. >> results\all_results.txt

REM Test each size
for %%s in (%SIZES%) do (
    echo.
    echo 📊 TESTING WITH %%s ELEMENTS
    echo.
    
    REM Assembly Test
    if exist "blazing_asm_final.exe" (
        echo 🔥 Assembly...
        echo === ASSEMBLY %%s ELEMENTS === >> results\all_results.txt
        blazing_asm_final.exe %%s >> results\all_results.txt 2>&1
        echo. >> results\all_results.txt
    )
    
    REM C Test
    if exist "blazing_c.exe" (
        echo 🔥 C...
        echo === C %%s ELEMENTS === >> results\all_results.txt
        blazing_c.exe %%s >> results\all_results.txt 2>&1
        echo. >> results\all_results.txt
    )
    
    REM C++ Test
    if exist "blazing_cpp.exe" (
        echo 🔥 C++...
        echo === C++ %%s ELEMENTS === >> results\all_results.txt
        blazing_cpp.exe %%s >> results\all_results.txt 2>&1
        echo. >> results\all_results.txt
    )
    
    REM Rust Test
    if exist "target\release\blazing.exe" (
        echo 🔥 Rust...
        echo === RUST %%s ELEMENTS === >> results\all_results.txt
        target\release\blazing.exe %%s >> results\all_results.txt 2>&1
        echo. >> results\all_results.txt
    )
    
    REM Go Test
    if exist "blazing_go.exe" (
        echo 🔥 Go...
        echo === GO %%s ELEMENTS === >> results\all_results.txt
        blazing_go.exe %%s >> results\all_results.txt 2>&1
        echo. >> results\all_results.txt
    )
    
    REM Python Test
    if exist "blazing_python_main.py" (
        echo 🔥 Python...
        echo === PYTHON %%s ELEMENTS === >> results\all_results.txt
        python blazing_python_main.py %%s >> results\all_results.txt 2>&1
        echo. >> results\all_results.txt
    )
    
    REM Dart Test
    if exist "blazing.dart" (
        echo 🔥 Dart...
        echo === DART %%s ELEMENTS === >> results\all_results.txt
        dart blazing.dart %%s >> results\all_results.txt 2>&1
        echo. >> results\all_results.txt
    )
    
    REM R Test
    if exist "blazing.R" (
        echo 🔥 R...
        echo === R %%s ELEMENTS === >> results\all_results.txt
        Rscript blazing.R %%s >> results\all_results.txt 2>&1
        echo. >> results\all_results.txt
    )
)

echo.
echo 🎉 ALL BENCHMARKS COMPLETED!
echo.
echo 📋 Results saved to: results\all_results.txt
echo.
echo 🏆 PERFORMANCE SUMMARY:
echo.

REM Extract best times for each size (simplified)
echo Size       Assembly   C       C++     Rust    Go      Python  Dart    R
echo ----       --------   ---     ---     ----    --      ------  ----    -

echo.
echo 🚀 To see detailed results: type results\all_results.txt
echo 📊 To run individual tests: use run_[language]_benchmark.bat
echo.

pause
