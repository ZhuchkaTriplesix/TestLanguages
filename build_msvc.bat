@echo off
echo 🚀⚡ BLAZING FAST C++ MSVC BUILD SCRIPT ⚡🚀
echo.

REM Проверяем наличие Visual Studio
where cl.exe >nul 2>&1
if %errorlevel% neq 0 (
    echo ❌ MSVC compiler not found. Please run from Developer Command Prompt.
    echo    Or install Visual Studio Build Tools.
    pause
    exit /b 1
)

echo ✅ MSVC compiler found!
echo 🔧 Compiling with maximum optimizations...
echo.

REM BLAZING FAST MSVC flags
cl.exe blazing.cpp ^
    /std:c++17 ^
    /O2 ^
    /Oi ^
    /Ot ^
    /Oy ^
    /GL ^
    /DNDEBUG ^
    /arch:AVX2 ^
    /fp:fast ^
    /GS- ^
    /Gy ^
    /LTCG ^
    /openmp ^
    /Fe:blazing_cpp.exe ^
    /link /OPT:REF /OPT:ICF /LTCG

if %errorlevel% equ 0 (
    echo.
    echo ✅ Compilation successful!
    echo 🚀 Running BLAZING FAST C++ version...
    echo.
    
    REM Устанавливаем количество пользователей и запускаем
    set NUM_USERS=100000000
    blazing_cpp.exe
    
    echo.
    echo 💥 C++ BLAZING execution complete!
) else (
    echo.
    echo ❌ Compilation failed!
    pause
)

echo.
echo 📁 Generated files:
dir /b blazing_cpp.exe blazing_results_cpp.txt 2>nul
echo.
pause
