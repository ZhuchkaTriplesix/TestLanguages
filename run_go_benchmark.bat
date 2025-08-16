@echo off
echo 🚀⚡ GO BLAZING FAST BENCHMARK ⚡🚀
echo.

REM Check if Go is installed
where go >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ❌ Go not found! 
    echo.
    echo 📥 INSTALL GO:
    echo    Option 1: Download from https://golang.org/dl/
    echo    Option 2: choco install golang
    echo    Option 3: Windows installer from official site
    echo.
    pause
    exit /b 1
)

echo ✅ Go found:
go version
echo.

echo 🏗️ Running Go benchmark ^(1M elements^)...
echo.
go run blazing.go

echo.
echo 🚀 Running Go benchmark ^(100M elements^)...
echo.
set NUM_USERS=100000000
go run blazing.go

echo.
echo 🎉 Go benchmarks complete!
echo 📊 Check blazing_results_go.txt for detailed results
echo.
pause

