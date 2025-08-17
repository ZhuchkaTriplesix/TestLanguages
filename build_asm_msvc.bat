@echo off
REM 🚀⚡ ASSEMBLY BLAZING FAST BUILD SCRIPT (MSVC) ⚡🚀

echo Building Assembly benchmark with MSVC optimizations...

REM Try with Visual Studio compiler
cl /O2 /arch:SSE2 blazing_asm_inline.c /Fe:blazing_asm.exe >nul 2>&1
if %errorlevel% == 0 (
    echo ✅ Assembly build successful with MSVC!
    goto :run_benchmark
)

REM Try with basic MSVC
cl blazing_asm_inline.c /Fe:blazing_asm.exe >nul 2>&1
if %errorlevel% == 0 (
    echo ✅ Assembly build successful with basic MSVC!
    goto :run_benchmark
)

echo ❌ MSVC compiler not found or not configured!
echo.
echo Please run this from a Visual Studio Developer Command Prompt
echo or install Visual Studio Build Tools.
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

