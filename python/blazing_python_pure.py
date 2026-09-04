#!/usr/bin/env python3
"""
🐍🚀⚡ BLAZING FAST Pure Python VERSION (No Dependencies) ⚡🚀🐍

Максимальные оптимизации только с встроенными возможностями Python:
- Оптимизированные алгоритмы
- __slots__ для экономии памяти
- Разные подходы к циклам
- Multiprocessing (встроенный)
- Профилирование производительности
"""

import os
import sys
import time
import gc
import platform
import multiprocessing as mp
from typing import List, Tuple, Optional
from concurrent.futures import ProcessPoolExecutor

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
    
    def add_user(self, user_id: int, name: str, age: int) -> None:
        self.ids.append(user_id)
        self.names.append(name)
        self.ages.append(age)

# 🐍 PURE PYTHON OPTIMIZED VERSIONS 🐍

def sum_ages_python_basic(users: List[User]) -> int:
    """Базовая Python версия - AoS"""
    return sum(user.age for user in users)

def sum_ages_python_soa(user_soa: UserSoA) -> int:
    """SoA версия"""
    return sum(user_soa.ages)

def sum_ages_builtin_sum(ages: List[int]) -> int:
    """Встроенная функция sum() - самая оптимизированная"""
    return sum(ages)

def sum_ages_manual_loop(ages: List[int]) -> int:
    """Ручной цикл"""
    total = 0
    for age in ages:
        total += age
    return total

def sum_ages_while_loop(ages: List[int]) -> int:
    """While цикл с индексом"""
    total = 0
    i = 0
    n = len(ages)
    while i < n:
        total += ages[i]
        i += 1
    return total

def sum_ages_unrolled_2(ages: List[int]) -> int:
    """Loop unrolling x2"""
    total = 0
    n = len(ages)
    i = 0
    
    # Обрабатываем по 2 элемента
    while i + 2 <= n:
        total += ages[i] + ages[i+1]
        i += 2
    
    # Остальные элементы
    while i < n:
        total += ages[i]
        i += 1
    
    return total

def sum_ages_unrolled_4(ages: List[int]) -> int:
    """Loop unrolling x4"""
    total = 0
    n = len(ages)
    i = 0
    
    # Обрабатываем по 4 элемента
    while i + 4 <= n:
        total += ages[i] + ages[i+1] + ages[i+2] + ages[i+3]
        i += 4
    
    # Остальные элементы
    while i < n:
        total += ages[i]
        i += 1
    
    return total

def sum_ages_unrolled_8(ages: List[int]) -> int:
    """Loop unrolling x8"""
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

def sum_ages_chunked(ages: List[int], chunk_size: int = 1000) -> int:
    """Chunked processing для лучшей cache locality"""
    total = 0
    n = len(ages)
    
    for i in range(0, n, chunk_size):
        chunk_end = min(i + chunk_size, n)
        chunk_sum = 0
        
        for j in range(i, chunk_end):
            chunk_sum += ages[j]
        
        total += chunk_sum
    
    return total

def sum_ages_slice_sum(ages: List[int]) -> int:
    """Использование slice и sum()"""
    return sum(ages[:])

def sum_ages_reduce(ages: List[int]) -> int:
    """Функциональный подход с reduce"""
    from functools import reduce
    import operator
    return reduce(operator.add, ages, 0)

def sum_ages_map_sum(ages: List[int]) -> int:
    """Map + sum подход"""
    return sum(map(int, ages))

# 🚀 MULTIPROCESSING VERSIONS 🚀

def sum_chunk_worker(chunk: List[int]) -> int:
    """Worker функция для multiprocessing"""
    return sum(chunk)

def sum_ages_multiprocessing_simple(ages: List[int], num_processes: int = None) -> int:
    """Простая multiprocessing версия"""
    if num_processes is None:
        num_processes = min(mp.cpu_count(), 4)  # Ограничиваем для небольших данных
    
    if len(ages) < 100000:  # Для маленьких массивов multiprocessing не эффективен
        return sum(ages)
    
    # Разбиваем на чанки
    chunk_size = max(1, len(ages) // num_processes)
    chunks = [ages[i:i+chunk_size] for i in range(0, len(ages), chunk_size)]
    
    # Параллельная обработка
    with ProcessPoolExecutor(max_workers=num_processes) as executor:
        results = executor.map(sum_chunk_worker, chunks)
    
    return sum(results)

def sum_chunk_optimized(chunk: List[int]) -> int:
    """Оптимизированный worker с unrolling"""
    total = 0
    n = len(chunk)
    i = 0
    
    # Unrolling внутри worker'а
    while i + 4 <= n:
        total += chunk[i] + chunk[i+1] + chunk[i+2] + chunk[i+3]
        i += 4
    
    while i < n:
        total += chunk[i]
        i += 1
    
    return total

def sum_ages_multiprocessing_optimized(ages: List[int], num_processes: int = None) -> int:
    """Оптимизированная multiprocessing версия"""
    if num_processes is None:
        num_processes = mp.cpu_count()
    
    if len(ages) < 50000:
        return sum_ages_unrolled_8(ages)
    
    # Разбиваем на чанки с учетом cache line
    chunk_size = max(1000, len(ages) // num_processes)  # Минимум 1000 элементов
    chunks = [ages[i:i+chunk_size] for i in range(0, len(ages), chunk_size)]
    
    # Параллельная обработка с оптимизированным worker'ом
    with ProcessPoolExecutor(max_workers=num_processes) as executor:
        results = executor.map(sum_chunk_optimized, chunks)
    
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
        else:
            avg_age = result / len(args[0]) if len(args[0]) > 0 else 0
        
        return BenchmarkResult(name, result, time_ms, avg_age)
    
    except Exception as e:
        print(f"❌ Error in {name}: {e}")
        return BenchmarkResult(name, 0, float('inf'), 0)

def print_results_table(results: List[BenchmarkResult]) -> None:
    """Печать результатов в виде таблицы"""
    print("📊 PURE PYTHON PERFORMANCE COMPARISON:")
    print("=" * 70)
    print(f"{'Algorithm':<25s} {'Avg Age':<8s} {'Time (ms)':<12s} {'Speedup':<10s}")
    print("=" * 70)
    
    for result in results:
        if result.time_ms == float('inf'):
            speedup_str = "FAILED"
        else:
            speedup_str = f"{result.speedup:.1f}x" if result.speedup else "baseline"
        print(f"{result.name:<25s} {result.avg_age:<8.0f} {result.time_ms:<12.3f} {speedup_str:<10s}")
    
    print("=" * 70)

def write_results_to_file(results: List[BenchmarkResult], system_info: dict) -> None:
    """Запись результатов в файл"""
    filename = "blazing_results_python_pure.txt"
    
    with open(filename, 'w', encoding='utf-8') as f:
        f.write("🐍🚀⚡ PURE PYTHON BLAZING FAST RESULTS ⚡🚀🐍\n\n")
        
        # Системная информация
        f.write("🖥️ SYSTEM INFO:\n")
        for key, value in system_info.items():
            f.write(f"   {key}: {value}\n")
        f.write("\n")
        
        # Результаты
        f.write("📊 BENCHMARK RESULTS:\n")
        f.write("=" * 50 + "\n")
        for result in results:
            if result.time_ms != float('inf'):
                speedup_str = f" ({result.speedup:.1f}x faster)" if result.speedup else ""
                f.write(f"{result.name}: {result.avg_age:.0f} avg - {result.time_ms:.3f}ms{speedup_str}\n")
        f.write("=" * 50 + "\n")
        
        # Лучший результат
        valid_results = [r for r in results if r.time_ms != float('inf')]
        if valid_results:
            fastest = min(valid_results, key=lambda x: x.time_ms)
            f.write(f"\n🏆 FASTEST: {fastest.name} - {fastest.time_ms:.3f}ms\n")
            f.write(f"💥 MAX SPEEDUP: {fastest.speedup:.1f}x faster than baseline\n")

def get_system_info() -> dict:
    """Сбор информации о системе"""
    return {
        "Platform": platform.platform(),
        "Python Version": platform.python_version(),
        "Python Implementation": platform.python_implementation(),
        "CPU Cores": mp.cpu_count(),
        "Architecture": platform.architecture()[0],
        "Machine": platform.machine(),
        "Dependencies": "Pure Python (no external libraries)",
    }

def main() -> None:
    print("🐍🚀⚡ PURE PYTHON BLAZING FAST BENCHMARK SUITE ⚡🚀🐍\n")
    
    # Системная информация
    system_info = get_system_info()
    
    print("🖥️ SYSTEM INFO:")
    for key, value in system_info.items():
        print(f"   {key}: {value}")
    print()
    
    # Параметры тестирования
    num_users = int(os.getenv("NUM_USERS", "1000000"))  # 1M по умолчанию для Pure Python
    
    print(f"Processing {num_users:,} users")
    print(f"User object size: ~{sys.getsizeof(User(0, '', 0))} bytes")
    print(f"List overhead per element: ~{sys.getsizeof([0]) - sys.getsizeof(0)} bytes")
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
    
    print("✅ Test data created!")
    print(f"Memory usage: ~{sys.getsizeof(users) + sys.getsizeof(ages_list):,} bytes")
    print()
    
    # Запуск бенчмарков
    results: List[BenchmarkResult] = []
    
    # Все алгоритмы для тестирования
    algorithms = [
        ("AoS Basic", sum_ages_python_basic, users),
        ("SoA Basic", sum_ages_python_soa, user_soa),
        ("Builtin Sum", sum_ages_builtin_sum, ages_list),
        ("Manual Loop", sum_ages_manual_loop, ages_list),
        ("While Loop", sum_ages_while_loop, ages_list),
        ("Unrolled x2", sum_ages_unrolled_2, ages_list),
        ("Unrolled x4", sum_ages_unrolled_4, ages_list),
        ("Unrolled x8", sum_ages_unrolled_8, ages_list),
        ("Chunked", sum_ages_chunked, ages_list),
        ("Slice Sum", sum_ages_slice_sum, ages_list),
        ("Reduce", sum_ages_reduce, ages_list),
        ("Map Sum", sum_ages_map_sum, ages_list),
    ]
    
    # Multiprocessing версии только для больших данных
    if num_users >= 100000:
        algorithms.extend([
            ("MP Simple", sum_ages_multiprocessing_simple, ages_list),
            ("MP Optimized", sum_ages_multiprocessing_optimized, ages_list),
        ])
    
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
    valid_results = [r for r in results if r.time_ms != float('inf')]
    if valid_results:
        baseline_time = valid_results[0].time_ms
        for result in results:
            if result.time_ms != float('inf'):
                result.speedup = baseline_time / result.time_ms
    
    # Показываем результаты
    print_results_table(results)
    
    # Находим самый быстрый
    if valid_results:
        fastest = min(valid_results, key=lambda x: x.time_ms)
        
        print(f"\n🏆⚡ АБСОЛЮТНЫЙ ПОБЕДИТЕЛЬ PURE PYTHON: {fastest.name}")
        print(f"    Время: {fastest.time_ms:.3f}ms")
        print(f"    Ускорение: {fastest.speedup:.1f}x\n")
        
        if fastest.speedup > 5:
            print("🔥 ОТЛИЧНЫЕ РЕЗУЛЬТАТЫ для Pure Python! 🔥")
        elif fastest.speedup > 2:
            print("✅ Хорошие оптимизации Python кода!")
        
        print()
        
        # Сравнение с другими языками
        print("🔬 COMPARISON WITH OTHER LANGUAGES:")
        print("Rust PARALLEL LUDICROUS: ~2.32ms (37x faster)")
        print("C++ Expected: ~2-3ms (35-50x faster)")
        print(f"Python Pure: {fastest.time_ms:.2f}ms ({fastest.speedup:.1f}x faster)")
        print()
        
        # Записываем результаты
        write_results_to_file(results, system_info)
        print("💾 Results written to: blazing_results_python_pure.txt")
        
        print("\n🎯 PURE PYTHON OPTIMIZATION SUMMARY:")
        print("✅ Built-in sum(): Highly optimized C implementation")
        print("✅ Loop unrolling: Reduces Python bytecode overhead")
        print("✅ Multiprocessing: Bypasses GIL for CPU-bound tasks")
        print("✅ __slots__: Reduces memory overhead")
        print("✅ Algorithm choice: Different approaches for different data sizes")
        print("✅ Chunked processing: Better cache locality")
        
        print(f"\n💥 PURE PYTHON BENCHMARK COMPLETE! 💥")
        print("🐍 Даже чистый Python может быть достаточно быстрым! 🐍⚡")
        
        # Рекомендации по дальнейшей оптимизации
        print("\n🚀 For BLAZING FAST performance, install:")
        print("   • NumPy: pip install numpy (vectorization)")
        print("   • Numba: pip install numba (JIT compilation)")
        print("   • Cython: pip install cython (C extensions)")
        
        if fastest.time_ms > 100:
            print(f"\n💡 For {num_users:,} elements, consider:")
            print("   • Reducing data size for testing")
            print("   • Installing NumPy/Numba for 10-100x speedup")
            print("   • Using PyPy for automatic JIT compilation")

if __name__ == "__main__":
    # Устанавливаем multiprocessing для Windows
    if __name__ == "__main__":
        mp.set_start_method('spawn', force=True)
    
    main()
