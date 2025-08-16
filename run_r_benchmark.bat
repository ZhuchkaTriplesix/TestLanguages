@echo off
echo 📊🚀⚡ R BLAZING FAST BENCHMARK RUNNER ⚡🚀📊

REM Проверяем наличие R
where Rscript >nul 2>&1
if %errorlevel% neq 0 (
    echo ❌ R not found! Please install R from https://cran.r-project.org/
    echo    Or add R to your PATH environment variable
    pause
    exit /b 1
)

echo ✅ R found!

REM Проверяем доступность дополнительных пакетов
echo 📦 Checking R packages...
Rscript -e "cat(ifelse(requireNamespace('data.table', quietly=TRUE), '✅ data.table available\n', '❌ data.table not available (install: install.packages(\"data.table\"))\n'))" 2>nul
Rscript -e "cat(ifelse(requireNamespace('Rcpp', quietly=TRUE), '✅ Rcpp available\n', '❌ Rcpp not available (install: install.packages(\"Rcpp\"))\n'))" 2>nul
echo.

echo 🚀 Running R benchmark...
echo.

REM Запускаем бенчмарк
Rscript blazing.R

if exist blazing_results_r.txt (
    echo.
    echo 💾 Results saved to: blazing_results_r.txt
    echo 📊 First few results:
    for /f "skip=3 tokens=*" %%a in ('type blazing_results_r.txt ^| findstr /n ":" ^| head -n 5') do echo    %%a
)

echo.
echo 🎯 USAGE EXAMPLES:
echo    • Default (1M):     run_r_benchmark.bat
echo    • Custom size:      set NUM_USERS=10000000 ^&^& run_r_benchmark.bat
echo    • Direct R:         Rscript blazing.R
echo.
echo 📦 TO INSTALL R PACKAGES:
echo    • Open R console: R
echo    • Install data.table: install.packages("data.table")
echo    • Install Rcpp: install.packages("Rcpp")
echo.
echo 💥 R BENCHMARK COMPLETE! 💥
pause
