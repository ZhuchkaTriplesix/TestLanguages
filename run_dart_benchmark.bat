@echo off
echo 🚀⚡ DART BLAZING FAST BENCHMARK ⚡🚀
echo.

REM Check if Dart is installed
where dart >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ❌ Dart not found! 
    echo.
    echo 📥 INSTALL DART:
    echo    Option 1: Download from https://dart.dev/get-dart
    echo    Option 2: Install Flutter ^(includes Dart^): https://flutter.dev
    echo    Option 3: choco install dart-sdk
    echo.
    pause
    exit /b 1
)

echo ✅ Dart found:
dart --version
echo.

echo 🏗️ Running Dart benchmark ^(1M elements^)...
echo.
dart run blazing.dart

echo.
echo 🚀 Running Dart benchmark ^(100M elements^)...
echo.
set NUM_USERS=100000000
dart run blazing.dart

echo.
echo 🎉 Dart benchmarks complete!
echo 📊 Check blazing_results_dart.txt for detailed results
echo.
pause
