@echo off
REM 🚀⚡ ASSEMBLY BLAZING FAST BUILD SCRIPT ⚡🚀

echo Building Assembly benchmark with maximum optimizations...

REM Simple approach - try to compile with available compiler
echo Attempting to build Assembly inline version...

REM Try with GCC (MinGW) - inline assembly version
gcc -O3 -march=native -msse2 -mavx2 -funroll-loops -flto -Wall -Wextra -std=c11 blazing_asm_inline.c -o blazing_asm.exe >nul 2>&1
if %errorlevel% == 0 (
    echo ✅ Assembly inline build successful with GCC!
    goto :run_benchmark
)

REM Try with basic optimizations
gcc -O2 -msse2 blazing_asm_inline.c -o blazing_asm.exe >nul 2>&1
if %errorlevel% == 0 (
    echo ✅ Assembly build successful with basic GCC!
    goto :run_benchmark
)

REM Try with Clang
clang -O3 -march=native -msse2 -mavx2 -funroll-loops -flto blazing_asm_inline.c -o blazing_asm.exe >nul 2>&1
if %errorlevel% == 0 (
    echo ✅ Assembly build successful with Clang!
    goto :run_benchmark
)

REM Try minimal build
gcc blazing_asm_inline.c -o blazing_asm.exe >nul 2>&1
if %errorlevel% == 0 (
    echo ✅ Assembly build successful with minimal GCC!
    goto :run_benchmark
)

echo ❌ No suitable compiler found for Assembly build!
echo.
echo Please install one of the following:
echo   • MinGW-w64 (recommended)
echo   • TDM-GCC
echo   • Clang/LLVM
echo   • Visual Studio Build Tools
echo.
echo Or try manual compilation:
echo   gcc -O3 -msse2 blazing_asm_inline.c -o blazing_asm.exe
echo.
pause
exit /b 1

:run_benchmark
echo.
echo 🚀 Running Assembly benchmark...
echo.
blazing_asm.exe
echo.
echo 🎉 Assembly benchmark complete!
pause

