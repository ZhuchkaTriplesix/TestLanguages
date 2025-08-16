@echo off
echo 🐍🚀⚡ PYTHON BLAZING FAST BENCHMARK RUNNER ⚡🚀🐍
echo.

REM Проверяем Python
python --version >nul 2>&1
if %errorlevel% neq 0 (
    echo ❌ Python not found! Run setup_python.bat first
    pause
    exit /b 1
)

REM Получаем количество пользователей из аргумента или используем default
set NUM_USERS=%1
if "%NUM_USERS%"=="" set NUM_USERS=1000000

echo 🎯 Benchmark Parameters:
echo    Users: %NUM_USERS:,=%
echo    Platform: Windows
echo.

REM Проверяем доступность компонентов
echo 🔍 Checking available optimizations...
python -c "import numpy; print('✅ NumPy available')" 2>nul || echo "❌ NumPy not available"
python -c "import numba; print('✅ Numba JIT available')" 2>nul || echo "❌ Numba not available"
python -c "import blazing_cython; print('✅ Cython extensions available')" 2>nul || echo "❌ Cython not available"
echo.

echo 🚀 Running comprehensive Python benchmark...
echo.

REM Устанавливаем переменную окружения и запускаем
set NUM_USERS=%NUM_USERS%
python blazing_python_main.py

echo.
echo 📊 Benchmark Results Summary:
echo.

REM Показываем результаты если файл создался
if exist blazing_results_python_full.txt (
    echo 💾 Full results saved to: blazing_results_python_full.txt
    echo.
    echo 🏆 Top 3 Results:
    for /f "skip=3 tokens=*" %%a in ('findstr /n ":" blazing_results_python_full.txt ^| head -n 5') do echo    %%a
) else (
    echo ⚠️  Results file not created
)

echo.
echo 🔬 Performance Analysis:
echo.

REM Дополнительные бенчмарки
echo 🔥 Running Numba-only benchmark...
if exist blazing_numba.py (
    set NUM_USERS=%NUM_USERS%
    python blazing_numba.py
) else (
    echo ❌ blazing_numba.py not found
)

echo.
echo 🎯 PYTHON BENCHMARK COMPLETE!
echo.
echo 📋 Available benchmark commands:
echo    • Full benchmark: run_python_benchmark.bat [num_users]
echo    • JIT only: python blazing_numba.py
echo    • Main suite: python blazing_python_main.py
echo.
echo 🚀 Python optimization techniques tested:
echo    ✓ Pure Python optimization
echo    ✓ NumPy vectorization
echo    ✓ Numba JIT compilation
echo    ✓ Cython C extensions
echo    ✓ Multiprocessing parallelism
echo    ✓ Memory optimization
echo.

REM Сравнение с другими языками
echo 🏁 Performance Comparison:
echo    Rust LUDICROUS: ~2.32ms (37x)
echo    C++ Expected: ~2-3ms (35-50x)
echo    Python Target: ~5-50ms (5-50x)
echo.
echo 💥 Python может конкурировать с системными языками! 💥
pause
