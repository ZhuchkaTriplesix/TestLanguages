@echo off
REM 🚀⚡ C BLAZING FAST BUILD SCRIPT ⚡🚀

echo Building C benchmark with maximum optimizations...

REM Simple approach - try to compile with basic optimizations
echo Attempting to build with available compiler...

REM Try with GCC (MinGW)
gcc -O2 -Wall blazing.c -o blazing_c.exe >nul 2>&1
if %errorlevel% == 0 (
    echo ✅ Build successful with GCC!
    goto :run_benchmark
)

REM Try with Clang
clang -O2 -Wall blazing.c -o blazing_c.exe >nul 2>&1
if %errorlevel% == 0 (
    echo ✅ Build successful with Clang!
    goto :run_benchmark
)

REM Try with TCC (Tiny C Compiler)
tcc -O2 blazing.c -o blazing_c.exe >nul 2>&1
if %errorlevel% == 0 (
    echo ✅ Build successful with TCC!
    goto :run_benchmark
)

REM Try basic approach without advanced flags
gcc blazing.c -o blazing_c.exe >nul 2>&1
if %errorlevel% == 0 (
    echo ✅ Build successful with basic GCC!
    goto :run_benchmark
)

REM Try alternative name
g++ -x c blazing.c -o blazing_c.exe >nul 2>&1
if %errorlevel% == 0 (
    echo ✅ Build successful with G++!
    goto :run_benchmark
)

echo ❌ No suitable C compiler found!
echo.
echo Please install one of the following:
echo   • MinGW-w64 (recommended)
echo   • TDM-GCC
echo   • Clang/LLVM
echo   • Visual Studio Build Tools
echo.
echo Or try manual compilation:
echo   gcc blazing.c -o blazing_c.exe
echo.
pause
exit /b 1

:run_benchmark
echo.
echo 🚀 Running C benchmark...
echo.
blazing_c.exe
echo.
echo 🎉 C benchmark complete!
pause
