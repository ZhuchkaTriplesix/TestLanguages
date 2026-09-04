#!/usr/bin/env python3
"""
🚀⚡ BLAZING FAST Python MAIN BENCHMARK SUITE ⚡🚀

Сравнение всех оптимизаций Python:
- Pure Python (baseline)
- NumPy vectorization  
- Numba JIT compilation
- Cython C extensions
- Multiprocessing
- PyPy compatibility
"""

import os
import sys
import time
import gc
import platform
import multiprocessing as mp
from typing import List, Tuple, Dict, Any, Optional

# Проверяем доступность библиотек
NUMBA_AVAILABLE = False
CYTHON_AVAILABLE = False
NUMPY_AVAILABLE = False

try:
    import numpy as np
    NUMPY_AVAILABLE = True
    print("✅ NumPy available")
except ImportError:
    print("❌ NumPy not available")

try:
    import numba
    from blazing_numba import *
    NUMBA_AVAILABLE = True
    print("✅ Numba JIT available")
except ImportError:
    print("❌ Numba not available (install: pip install numba)")

try:
    import blazing_cython
    CYTHON_AVAILABLE = True
    print("✅ Cython extensions available")
except ImportError:\
    print("❌ Cython extensions not compiled (run: python setup_cython.py build_ext --inplace)")

print()

class User:
    """Оптимизированный User class с __slots__"""
    __slots__ = ['id', 'name', 'age']
    
    def __init__(self, id: int, name: str, age: int):
        self.id = id
        self.name = name
        self.age = age

class UserSoA:
    """Struct of Arrays версия"""
    __slots__ = ['ids', 'names', 'ages']
    
    def __init__(self):
        self.ids: List[int] = []
        self.names: List[str] = []
        self.ages: List[int] = []
    
    @classmethod
    def new_with_capacity(cls, capacity: int):
        instance = cls()
        return instance
    
    def add_user(self, user_id: int, name: str, age: int) -> None:
        self.ids.append(user_id)
        self.names.append(name)
        self.ages.append(age)

# 🐍 PURE PYTHON OPTIMIZED VERSIONS 🐍

def sum_ages_python_basic(users: List[User]) -> int:
    """Базовая Python версия"""
    return sum(user.age for user in users)

def sum_ages_python_soa(user_soa: UserSoA) -> int:
    """SoA версия"""
    return sum(user_soa.ages)

def sum_ages_python_optimized(ages: List[int]) -> int:
    """Оптимизированная Python версия"""
    total = 0
    for age in ages:
        total += age
    return total

def sum_ages_python_unrolled(ages: List[int]) -> int:
    """Python с manual loop unrolling"""
    total = 0
    n = len(ages)
    i = 0
    
    # Обрабатываем по 8 элементов
    while i + 8 <= n:
        total += (ages[i] + ages[i+1] + ages[i+2] + ages[i+3] + 
                 ages[i+4] + ages[i+5] + ages[i+6] + ages[i+7])
        i += 8
    
    # Остальные элементы
    while i < n:
        total += ages[i]
        i += 1
    
    return total

def sum_ages_builtin_sum(ages: List[int]) -> int:
    """Встроенная функция sum()"""
    return sum(ages)

# 📊 NUMPY VERSIONS (если доступна) 📊

def sum_ages_numpy_basic(ages_array) -> int:
    """Базовая NumPy версия"""
    if not NUMPY_AVAILABLE:
        return 0
    return int(np.sum(ages_array))

def sum_ages_numpy_dtype_optimized(ages_list: List[int]) -> int:
    """NumPy с оптимизированным типом данных"""
    if not NUMPY_AVAILABLE:
        return 0
    ages_array = np.array(ages_list, dtype=np.uint8)
    return int(np.sum(ages_array, dtype=np.int64))

# 🚀 MULTIPROCESSING VERSIONS 🚀

def sum_chunk_mp(chunk: List[int]) -> int:
    """Worker функция для multiprocessing"""
    return sum(chunk)

def sum_ages_multiprocessing(ages: List[int], num_processes: int = None) -> int:
    """Multiprocessing версия"""
    if num_processes is None:
        num_processes = mp.cpu_count()
    
    if len(ages) < 10000:  # Для маленьких массивов multiprocessing не эффективен
        return sum(ages)
    
    # Разбиваем на чанки
    chunk_size = max(1, len(ages) // num_processes)
    chunks = [ages[i:i+chunk_size] for i in range(0, len(ages), chunk_size)]
    
    # Параллельная обработка
    with mp.Pool(processes=num_processes) as pool:
        results = pool.map(sum_chunk_mp, chunks)
    
    return sum(results)

# 💾 BENCHMARKING INFRASTRUCTURE 💾

class BenchmarkResult:
    """Результат бенчмарка"""
    def __init__(self, name: str, result: int, time_ms: float, avg_age: float):
        self.name = name
        self.result = result
        self.time_ms = time_ms
        self.avg_age = avg_age
        self.speedup: Optional[float] = None

def run_benchmark(name: str, func, *args, warmup_runs: int = 1) -> BenchmarkResult:
    """Запуск бенчмарка с warmup"""
    # Warmup runs
    for _ in range(warmup_runs):
        try:
            func(*args)
        except:
            pass
    
    # Главный замер
    gc.collect()  # Очистка garbage collector
    
    start_time = time.perf_counter()
    try:
        result = func(*args)
        elapsed_time = time.perf_counter() - start_time
        time_ms = elapsed_time * 1000
        
        # Вычисляем средний возраст
        if isinstance(args[0], list) and len(args[0]) > 0:
            if hasattr(args[0][0], 'age'):  # Список User объектов
                avg_age = result / len(args[0])
            else:  # Список возрастов
                avg_age = result / len(args[0])
        elif hasattr(args[0], 'ages'):  # UserSoA объект
            avg_age = result / len(args[0].ages)
        else:  # NumPy array или другое
            avg_age = result / len(args[0])
        
        return BenchmarkResult(name, result, time_ms, avg_age)
    
    except Exception as e:
        print(f"❌ Error in {name}: {e}")
        return BenchmarkResult(name, 0, float('inf'), 0)

def print_results_table(results: List[BenchmarkResult]) -> None:
    """Печать результатов в виде таблицы"""
    print("📊 PERFORMANCE COMPARISON:")
    print("=" * 70)
    print(f"{'Algorithm':<25s} {'Avg Age':<8s} {'Time (ms)':<12s} {'Speedup':<10s}")
    print("=" * 70)
    
    for result in results:
        speedup_str = f"{result.speedup:.1f}x" if result.speedup else "baseline"
        print(f"{result.name:<25s} {result.avg_age:<8.0f} {result.time_ms:<12.3f} {speedup_str:<10s}")
    
    print("=" * 70)

def write_results_to_file(results: List[BenchmarkResult], system_info: Dict[str, Any]) -> None:
    """Запись результатов в файл"""
    filename = "blazing_results_python_full.txt"
    
    with open(filename, 'w', encoding='utf-8') as f:
        f.write("🐍🚀⚡ PYTHON BLAZING FAST COMPLETE RESULTS ⚡🚀🐍\n\n")
        
        # Системная информация
        f.write("🖥️ SYSTEM INFO:\n")
        for key, value in system_info.items():
            f.write(f"   {key}: {value}\n")
        f.write("\n")
        
        # Результаты
        f.write("📊 BENCHMARK RESULTS:\n")
        f.write("=" * 50 + "\n")
        for result in results:
            speedup_str = f" ({result.speedup:.1f}x faster)" if result.speedup else ""
            f.write(f"{result.name}: {result.avg_age:.0f} avg - {result.time_ms:.3f}ms{speedup_str}\n")
        f.write("=" * 50 + "\n")
        
        # Лучший результат
        fastest = min(results, key=lambda x: x.time_ms)
        f.write(f"\n🏆 FASTEST: {fastest.name} - {fastest.time_ms:.3f}ms\n")
        f.write(f"💥 MAX SPEEDUP: {fastest.speedup:.1f}x faster than baseline\n")

def get_system_info() -> Dict[str, Any]:
    """Сбор информации о системе"""
    return {
        "Platform": platform.platform(),
        "Python Version": platform.python_version(),
        "Python Implementation": platform.python_implementation(),
        "CPU Cores": mp.cpu_count(),
        "Architecture": platform.architecture()[0],
        "Machine": platform.machine(),
        "NumPy Available": NUMPY_AVAILABLE,
        "Numba Available": NUMBA_AVAILABLE,
        "Cython Available": CYTHON_AVAILABLE,
    }

def main() -> None:
    print("🐍🚀⚡ PYTHON BLAZING FAST COMPLETE BENCHMARK SUITE ⚡🚀🐍\n")
    
    # Системная информация
    system_info = get_system_info()
    
    print("🖥️ SYSTEM INFO:")
    for key, value in system_info.items():
        print(f"   {key}: {value}")
    print()
    
    # Параметры тестирования
    num_users = int(os.getenv("NUM_USERS", "10000000"))  # По умолчанию 10M для Python
    
    print(f"Processing {num_users:,} users")
    print(f"User object size: ~{sys.getsizeof(User(0, '', 0))} bytes")
    print()
    
    # Создаем данные
    print("📊 Creating test data...")
    
    users: List[User] = []
    user_soa = UserSoA()
    ages_list: List[int] = []
    
    for i in range(num_users):
        uid = i
        name = f"User {i}"
        age = i % 100
        
        users.append(User(uid, name, age))
        user_soa.add_user(uid, name, age)
        ages_list.append(age)
    
    # NumPy array если доступна
    ages_numpy = None
    if NUMPY_AVAILABLE:
        ages_numpy = np.array(ages_list, dtype=np.uint8)
    
    print("✅ Test data created!\n")
    
    # Запуск бенчмарков
    results: List[BenchmarkResult] = []
    
    # Pure Python версии
    algorithms = [
        ("Python AoS", sum_ages_python_basic, users),
        ("Python SoA", sum_ages_python_soa, user_soa),
        ("Python Optimized", sum_ages_python_optimized, ages_list),
        ("Python Unrolled", sum_ages_python_unrolled, ages_list),
        ("Python Builtin", sum_ages_builtin_sum, ages_list),
    ]
    
    # NumPy версии
    if NUMPY_AVAILABLE:
        algorithms.extend([
            ("NumPy Basic", sum_ages_numpy_basic, ages_numpy),
            ("NumPy Optimized", sum_ages_numpy_dtype_optimized, ages_list),
        ])
    
    # Multiprocessing версия
    if num_users >= 100000:  # Только для больших данных
        algorithms.append(("Multiprocessing", sum_ages_multiprocessing, ages_list))
    
    # Numba версии (если доступна)
    if NUMBA_AVAILABLE:
        print("🔥 Warming up Numba JIT functions...")
        try:
            # Импортируем функции из blazing_numba
            from blazing_numba import (
                sum_ages_jit_basic, sum_ages_jit_unrolled, 
                sum_ages_jit_parallel, sum_ages_jit_godlike,
                sum_ages_numpy_basic as numba_numpy_basic
            )
            
            # Warmup
            small_test = ages_numpy[:1000] if NUMPY_AVAILABLE else np.array(ages_list[:1000], dtype=np.uint8)
            sum_ages_jit_basic(small_test)
            sum_ages_jit_unrolled(small_test)
            sum_ages_jit_parallel(small_test)
            sum_ages_jit_godlike(small_test)
            
            # Добавляем Numba алгоритмы
            algorithms.extend([
                ("Numba JIT Basic", sum_ages_jit_basic, ages_numpy),
                ("Numba JIT Unrolled", sum_ages_jit_unrolled, ages_numpy),
                ("Numba JIT Parallel", sum_ages_jit_parallel, ages_numpy),
                ("Numba JIT GODLIKE", sum_ages_jit_godlike, ages_numpy),
            ])
            print("✅ Numba warmup complete!")
        except Exception as e:
            print(f"❌ Numba warmup failed: {e}")
        print()
    
    # Cython версии (если доступна)
    if CYTHON_AVAILABLE:
        try:
            algorithms.extend([
                ("Cython Basic", blazing_cython.sum_ages_cy_basic, ages_numpy),
                ("Cython Unrolled", blazing_cython.sum_ages_cy_unrolled, ages_numpy),
                ("Cython Parallel", blazing_cython.sum_ages_cy_parallel, ages_numpy),
                ("Cython GODLIKE", blazing_cython.sum_ages_cy_godlike, ages_numpy),
                ("Cython Pointer", blazing_cython.sum_ages_cy_pointer, ages_numpy),
            ])
        except Exception as e:
            print(f"❌ Cython algorithms failed to load: {e}")
    
    # Запускаем все бенчмарки
    print("🚀 Running benchmarks...\n")
    
    for name, func, data in algorithms:
        print(f"🔥 Testing {name}...")
        result = run_benchmark(name, func, data)
        results.append(result)
        
        if result.time_ms != float('inf'):
            print(f"   Result: {result.avg_age:.0f} avg age")
            print(f"   Time: {result.time_ms:.3f}ms")
        print()
    
    # Вычисляем speedup относительно baseline
    if results:
        baseline_time = results[0].time_ms
        for result in results:
            if result.time_ms != float('inf'):
                result.speedup = baseline_time / result.time_ms
    
    # Показываем результаты
    print_results_table(results)
    
    # Находим самый быстрый
    valid_results = [r for r in results if r.time_ms != float('inf')]
    if valid_results:
        fastest = min(valid_results, key=lambda x: x.time_ms)
        
        print(f"\n🏆⚡ АБСОЛЮТНЫЙ ПОБЕДИТЕЛЬ PYTHON: {fastest.name}")
        print(f"    Время: {fastest.time_ms:.3f}ms")
        print(f"    Ускорение: {fastest.speedup:.1f}x\n")
        
        if fastest.speedup > 100:
            print("🌌⚡ PYTHON ДОСТИГ LUDICROUS SPEED! ⚡🌌")
        elif fastest.speedup > 50:
            print("🚀 PYTHON РАБОТАЕТ С BLAZING SPEED! 🚀")
        elif fastest.speedup > 10:
            print("🔥 PYTHON ПОКАЗЫВАЕТ ОТЛИЧНУЮ ПРОИЗВОДИТЕЛЬНОСТЬ! 🔥")
        
        print()
        
        # Сравнение с другими языками
        print("🔬 COMPARISON WITH OTHER LANGUAGES:")
        print("Rust PARALLEL LUDICROUS: ~2.32ms (37x faster)")
        print("C++ Expected: ~2-3ms (35-50x faster)")
        print(f"Python Best: {fastest.time_ms:.2f}ms ({fastest.speedup:.1f}x faster)")
        print()
        
        # Записываем результаты
        write_results_to_file(results, system_info)
        print("💾 Results written to: blazing_results_python_full.txt")
        
        print("\n🎯 PYTHON OPTIMIZATION SUMMARY:")
        if NUMBA_AVAILABLE:
            print("✅ Numba JIT: Near C-speed performance")
        if CYTHON_AVAILABLE:
            print("✅ Cython: C extensions with Python syntax")
        if NUMPY_AVAILABLE:
            print("✅ NumPy: Vectorized operations in C")
        print("✅ Multiprocessing: True parallelism (GIL bypass)")
        print("✅ Algorithm optimization: Loop unrolling, chunking")
        print("✅ Memory optimization: __slots__, typed data")
        
        print(f"\n💥 PYTHON COMPLETE BENCHMARK FINISHED! 💥")
        print("🐍 Python может быть ОЧЕНЬ быстрым с правильными инструментами! 🐍⚡")

if __name__ == "__main__":
    # Проверяем поддержку multiprocessing
    if __name__ == "__main__":
        mp.set_start_method('spawn', force=True)  # Для Windows совместимости
    
    main()
