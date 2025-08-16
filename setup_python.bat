@echo off
echo 🐍🚀⚡ PYTHON BLAZING FAST SETUP SCRIPT ⚡🚀🐍
echo.

REM Проверяем Python
python --version >nul 2>&1
if %errorlevel% neq 0 (
    echo ❌ Python not found! Please install Python 3.7+
    echo    Download: https://www.python.org/downloads/
    pause
    exit /b 1
)

echo ✅ Python found!
python --version

REM Проверяем pip
pip --version >nul 2>&1
if %errorlevel% neq 0 (
    echo ❌ pip not found! Please install pip
    pause
    exit /b 1
)

echo ✅ pip found!
echo.

echo 🔧 Installing Python dependencies...
echo.

REM Обновляем pip
echo 📦 Updating pip...
python -m pip install --upgrade pip setuptools wheel

REM Устанавливаем основные зависимости
echo 📦 Installing core dependencies...
pip install numpy
if %errorlevel% neq 0 (
    echo ❌ Failed to install NumPy
    pause
    exit /b 1
)

echo 📦 Installing Numba (JIT compiler)...
pip install numba
if %errorlevel% neq 0 (
    echo ⚠️  Warning: Failed to install Numba (JIT will not be available)
)

echo 📦 Installing Cython...
pip install cython
if %errorlevel% neq 0 (
    echo ⚠️  Warning: Failed to install Cython (C extensions will not be available)
)

echo 📦 Installing additional packages...
pip install psutil

REM Пытаемся установить все из requirements
echo 📦 Installing all requirements...
pip install -r requirements_python.txt
if %errorlevel% neq 0 (
    echo ⚠️  Some packages failed to install, but core functionality should work
)

echo.
echo 🔨 Compiling Cython extensions...
python setup_cython.py build_ext --inplace
if %errorlevel% neq 0 (
    echo ⚠️  Cython compilation failed, but other optimizations will work
    echo.
) else (
    echo ✅ Cython extensions compiled successfully!
    echo.
)

echo 🧪 Testing installation...
python -c "import numpy; print('✅ NumPy:', numpy.__version__)"
python -c "import numba; print('✅ Numba:', numba.__version__)" 2>nul || echo "⚠️  Numba not available"
python -c "import blazing_cython; print('✅ Cython extensions available')" 2>nul || echo "⚠️  Cython extensions not available"

echo.
echo 🎯 INSTALLATION SUMMARY:
echo ✅ Python environment ready
echo ✅ NumPy for vectorization
echo ✅ Core dependencies installed
echo.

echo 🚀 READY TO RUN BLAZING FAST PYTHON!
echo.
echo 📋 Next steps:
echo    1. Run basic benchmark: python blazing_python_main.py
echo    2. Run Numba JIT: python blazing_numba.py
echo    3. Set users count: set NUM_USERS=1000000 ^&^& python blazing_python_main.py
echo.

echo 🎯 Expected Performance:
echo • Pure Python: baseline
echo • NumPy: 5-20x faster
echo • Numba JIT: 10-100x faster
echo • Cython: 20-200x faster
echo • Goal: Match Rust/C++ performance (~2-5ms for 100M elements)
echo.
pause
