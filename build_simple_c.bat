@echo off
REM 🚀⚡ C SIMPLE BUILD SCRIPT ⚡🚀

echo Building simplified C benchmark...

REM Try GCC with basic optimization
gcc -O2 -Wall blazing_simple.c -o blazing_simple_c.exe 2>nul
if %errorlevel% == 0 (
    echo ✅ Built successfully with GCC!
    goto :run_simple
)

REM Try GCC without optimization
gcc blazing_simple.c -o blazing_simple_c.exe 2>nul
if %errorlevel% == 0 (
    echo ✅ Built successfully with basic GCC!
    goto :run_simple
)

REM Try G++
g++ -x c blazing_simple.c -o blazing_simple_c.exe 2>nul
if %errorlevel% == 0 (
    echo ✅ Built successfully with G++!
    goto :run_simple
)

REM Try Clang
clang blazing_simple.c -o blazing_simple_c.exe 2>nul
if %errorlevel% == 0 (
    echo ✅ Built successfully with Clang!
    goto :run_simple
)

REM Try TCC
tcc blazing_simple.c -o blazing_simple_c.exe 2>nul
if %errorlevel% == 0 (
    echo ✅ Built successfully with TCC!
    goto :run_simple
)

echo ❌ No C compiler found! Please install MinGW or similar.
echo.
echo You can try manual compilation:
echo   gcc blazing_simple.c -o blazing_simple_c.exe
echo.
pause
exit /b 1

:run_simple
echo.
echo 🚀 Running simplified C benchmark...
echo.
blazing_simple_c.exe
echo.
pause

