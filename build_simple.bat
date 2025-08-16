@echo off
echo 🚀⚡ SIMPLE C++ BLAZING FAST BUILD ⚡🚀
echo.

REM Попробуем разные компиляторы
echo 🔍 Searching for available compilers...

REM Попробуем cl.exe (MSVC)
where cl.exe >nul 2>&1
if %errorlevel% equ 0 (
    echo ✅ Found MSVC compiler!
    echo 🔧 Compiling with MSVC...
    cl.exe blazing_simple.cpp /std:c++17 /O2 /DNDEBUG /Fe:blazing_simple.exe /link
    if %errorlevel% equ 0 goto :run_program
)

REM Попробуем g++
where g++.exe >nul 2>&1
if %errorlevel% equ 0 (
    echo ✅ Found GCC compiler!
    echo 🔧 Compiling with GCC...
    g++ -std=c++17 -O3 -DNDEBUG blazing_simple.cpp -o blazing_simple.exe
    if %errorlevel% equ 0 goto :run_program
)

REM Попробуем clang++
where clang++.exe >nul 2>&1
if %errorlevel% equ 0 (
    echo ✅ Found Clang compiler!
    echo 🔧 Compiling with Clang...
    clang++ -std=c++17 -O3 -DNDEBUG blazing_simple.cpp -o blazing_simple.exe
    if %errorlevel% equ 0 goto :run_program
)

REM Попробуем MinGW
where mingw32-g++.exe >nul 2>&1
if %errorlevel% equ 0 (
    echo ✅ Found MinGW compiler!
    echo 🔧 Compiling with MinGW...
    mingw32-g++ -std=c++17 -O3 -DNDEBUG blazing_simple.cpp -o blazing_simple.exe
    if %errorlevel% equ 0 goto :run_program
)

echo ❌ No C++ compiler found!
echo.
echo 💡 To compile C++, install one of:
echo    - Visual Studio Community (includes MSVC)
echo    - MinGW-w64
echo    - LLVM/Clang
echo    - Or run from VS Developer Command Prompt
echo.
echo 🔗 Download links:
echo    Visual Studio: https://visualstudio.microsoft.com/downloads/
echo    MinGW-w64: https://www.mingw-w64.org/downloads/
echo.
pause
exit /b 1

:run_program
echo.
echo ✅ Compilation successful!
echo 🚀 Running BLAZING FAST C++ version...
echo.

REM Устанавливаем количество пользователей и запускаем
set NUM_USERS=100000000
blazing_simple.exe

echo.
echo 💥 C++ BLAZING execution complete!
echo.
echo 📁 Generated files:
if exist blazing_simple.exe echo   blazing_simple.exe
if exist blazing_results_cpp.txt echo   blazing_results_cpp.txt
echo.
pause
