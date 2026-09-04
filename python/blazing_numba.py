#!/usr/bin/env python3
"""
🚀⚡ BLAZING FAST Python JIT VERSION with Numba ⚡🚀

Максимальные оптимизации Python:
- Numba JIT (LLVM backend)
- NumPy vectorization
- Multiprocessing
- Memory views
- Type annotations
"""

import os
import time
import numpy as np
from typing import List, Tuple
from numba import jit, njit, prange, types
from numba.typed import List as TypedList
import multiprocessing as mp
from concurrent.futures import ProcessPoolExecutor
import gc

# Установка переменных окружения для максимальной производительности
os.environ['NUMBA_CACHE'] = '1'  # Кэширование скомпилированного кода
os.environ['NUMBA_NUM_THREADS'] = str(mp.cpu_count())  # Все ядра для параллельности

class User:
    """Обычный User class для AoS"""
    __slots__ = ['id', 'name', 'age']  # Оптимизация памяти
    
    def __init__(self, id: int, name: str, age: int):
        self.id = id
        self.name = name
        self.age = age

class UserSoA:
    """Struct of Arrays для лучшей cache locality"""
    __slots__ = ['ids', 'names', 'ages']
    
    def __init__(self):
        self.ids: List[int] = []
        self.names: List[str] = []
        self.ages: List[int] = []
    
    @classmethod
    def new_with_capacity(cls, capacity: int):
        instance = cls()
        # Pre-allocate lists for better performance
        instance.ids = [0] * capacity
        instance.names = [''] * capacity  
        instance.ages = [0] * capacity
        return instance
    
    def add_user(self, user_id: int, name: str, age: int) -> None:
        self.ids.append(user_id)
        self.names.append(name)
        self.ages.append(age)

# 🔥 NUMBA JIT OPTIMIZED VERSIONS 🔥

@njit(cache=True, fastmath=True, nogil=True)
def sum_ages_jit_basic(ages: np.ndarray) -> int:
    """Базовая JIT версия с Numba"""
    total = 0
    for age in ages:
        total += age
    return total

@njit(cache=True, fastmath=True, nogil=True)
def sum_ages_jit_unrolled(ages: np.ndarray) -> int:
    """JIT с loop unrolling для лучшей производительности"""
    total = 0
    n = len(ages)
    i = 0
    
    # Обрабатываем по 8 элементов за раз (unrolling)
    while i + 8 <= n:
        total += (ages[i] + ages[i+1] + ages[i+2] + ages[i+3] + 
                 ages[i+4] + ages[i+5] + ages[i+6] + ages[i+7])
        i += 8
    
    # Остальные элементы
    while i < n:
        total += ages[i]
        i += 1
    
    return total

@njit(cache=True, fastmath=True, nogil=True, parallel=True)
def sum_ages_jit_parallel(ages: np.ndarray) -> int:
    """Параллельная JIT версия с prange"""
    n = len(ages)
    total = 0
    
    # Параллельная обработка с Numba
    for i in prange(n):
        total += ages[i]
    
    return total

@njit(cache=True, fastmath=True, nogil=True)
def sum_ages_jit_simd_like(ages: np.ndarray) -> int:
    """SIMD-подобная обработка в JIT"""
    total = 0
    n = len(ages)
    i = 0
    
    # Обрабатываем по 16 элементов (имитация SIMD)
    while i + 16 <= n:
        chunk_sum = 0
        for j in range(16):
            chunk_sum += ages[i + j]
        total += chunk_sum
        i += 16
    
    # Остальные элементы
    while i < n:
        total += ages[i]
        i += 1
        
    return total

@njit(cache=True, fastmath=True, nogil=True, parallel=True)
def sum_ages_jit_godlike(ages: np.ndarray) -> int:
    """GODLIKE JIT версия с максимальными оптимизациями"""
    n = len(ages)
    
    # Параллельная обработка чанков
    chunk_size = max(1, n // (4 * mp.cpu_count()))  # Оптимальный размер чанка
    num_chunks = (n + chunk_size - 1) // chunk_size
    
    total = 0
    for chunk_idx in prange(num_chunks):
        start = chunk_idx * chunk_size
        end = min(start + chunk_size, n)
        chunk_sum = 0
        
        # Unrolled loop внутри каждого чанка
        i = start
        while i + 8 <= end:
            chunk_sum += (ages[i] + ages[i+1] + ages[i+2] + ages[i+3] + 
                         ages[i+4] + ages[i+5] + ages[i+6] + ages[i+7])
            i += 8
        
        while i < end:
            chunk_sum += ages[i]
            i += 1
            
        total += chunk_sum
    
    return total

# ⚡ NUMPY VECTORIZED VERSIONS ⚡

def sum_ages_numpy_basic(ages: np.ndarray) -> int:
    """Базовая NumPy векторизация"""
    return np.sum(ages, dtype=np.int64)

def sum_ages_numpy_optimized(ages: np.ndarray) -> int:
    """Оптимизированная NumPy версия"""
    # Используем наиболее эффективный тип данных
    if ages.dtype != np.uint8:
        ages = ages.astype(np.uint8)
    return np.sum(ages, dtype=np.int64)

def sum_ages_numpy_chunked(ages: np.ndarray, chunk_size: int = 1024*1024) -> int:
    """NumPy с chunked processing для больших массивов"""
    total = 0
    n = len(ages)
    
    for i in range(0, n, chunk_size):
        chunk = ages[i:i+chunk_size]
        total += np.sum(chunk, dtype=np.int64)
    
    return total

# 🌟 MULTIPROCESSING VERSIONS 🌟

def sum_chunk_worker(chunk: np.ndarray) -> int:
    """Worker function для multiprocessing"""
    return np.sum(chunk, dtype=np.int64)

def sum_ages_multiprocessing(ages: np.ndarray, num_processes: int = None) -> int:
    """Multiprocessing версия с NumPy"""
    if num_processes is None:
        num_processes = mp.cpu_count()
    
    n = len(ages)
    chunk_size = max(1, n // num_processes)
    
    # Разбиваем на чанки
    chunks = [ages[i:i+chunk_size] for i in range(0, n, chunk_size)]
    
    # Параллельная обработка
    with ProcessPoolExecutor(max_workers=num_processes) as executor:
        results = executor.map(sum_chunk_worker, chunks)
    
    return sum(results)

@njit(cache=True, fastmath=True, nogil=True)
def sum_chunk_jit_worker(chunk: np.ndarray) -> int:
    """JIT worker для комбинированной версии"""
    return sum_ages_jit_unrolled(chunk)

def sum_ages_jit_multiprocessing(ages: np.ndarray, num_processes: int = None) -> int:
    """Комбинация JIT + Multiprocessing"""
    if num_processes is None:
        num_processes = mp.cpu_count()
    
    n = len(ages)
    chunk_size = max(1, n // num_processes)
    
    # Разбиваем на чанки
    chunks = [ages[i:i+chunk_size] for i in range(0, n, chunk_size)]
    
    # Параллельная обработка с JIT
    with ProcessPoolExecutor(max_workers=num_processes) as executor:
        results = executor.map(sum_chunk_jit_worker, chunks)
    
    return sum(results)

# 🚀 MEMORY OPTIMIZED VERSIONS 🚀

def sum_ages_memoryview(ages_list: List[int]) -> int:
    """Использование memory views для эффективной работы с памятью"""
    # Создаем numpy array из списка
    ages_array = np.array(ages_list, dtype=np.uint8)
    
    # Используем memory view для прямого доступа
    mv = memoryview(ages_array)
    total = 0
    
    # Прямой доступ через memory view
    for i in range(len(mv)):
        total += mv[i]
    
    return total

@njit(cache=True, fastmath=True, nogil=True)
def sum_ages_jit_from_list(ages_list):
    """JIT версия работающая напрямую с typed list"""
    total = 0
    for age in ages_list:
        total += age
    return total

# 💾 BLAZING FAST I/O FUNCTIONS 💾

def print_results_blazing(name: str, avg_age: int, elapsed_ms: float, speedup: float = None) -> None:
    """Оптимизированный вывод результатов"""
    if speedup:
        print(f"{name:20s}: {avg_age:3d} - {elapsed_ms:8.3f}ms ({speedup:5.1f}x faster)")
    else:
        print(f"{name:20s}: {avg_age:3d} - {elapsed_ms:8.3f}ms")

def write_results_to_file(results: List[Tuple[str, int, float]], filename: str = "blazing_results_python.txt") -> None:
    """Быстрая запись результатов в файл"""
    with open(filename, 'w', buffering=8192) as f:
        f.write("🚀⚡ PYTHON BLAZING FAST RESULTS ⚡🚀\n\n")
        for name, avg_age, elapsed_ms in results:
            f.write(f"{name}: {avg_age} avg age - {elapsed_ms:.3f}ms\n")

def main() -> None:
    print("🐍🚀⚡ PYTHON BLAZING FAST JIT VERSION ⚡🚀🐍\n")
    
    # Читаем количество пользователей
    num_users = int(os.getenv("NUM_USERS", "100000000"))
    num_processes = mp.cpu_count()
    
    print(f"User size: {User.__sizeof__(User())} bytes")
    print(f"Processing {num_users:,} users")
    print(f"CPU cores: {num_processes}")
    print(f"Numba cache enabled: {os.getenv('NUMBA_CACHE', 'False')}")
    print()
    
    # Создаем данные
    print("📊 Creating data...")
    users: List[User] = []
    user_soa = UserSoA()
    
    # Заполняем данные
    for i in range(num_users):
        uid = i
        name = f"User {i}"
        age = i % 100
        users.append(User(uid, name, age))
        user_soa.add_user(uid, name, age)
    
    # Создаем NumPy массив для JIT версий
    ages_numpy = np.array(user_soa.ages, dtype=np.uint8)
    
    # Создаем typed list для некоторых Numba функций
    ages_typed = TypedList()
    for age in user_soa.ages:
        ages_typed.append(age)
    
    print("✅ Data created!\n")
    
    results = []
    
    # Тестируем разные версии
    algorithms = [
        ("AoS Basic", lambda: sum(u.age for u in users)),
        ("SoA Basic", lambda: sum(user_soa.ages)),
        ("NumPy Basic", lambda: sum_ages_numpy_basic(ages_numpy)),
        ("NumPy Optimized", lambda: sum_ages_numpy_optimized(ages_numpy)),
        ("NumPy Chunked", lambda: sum_ages_numpy_chunked(ages_numpy)),
        ("Memory View", lambda: sum_ages_memoryview(user_soa.ages)),
        ("JIT Basic", lambda: sum_ages_jit_basic(ages_numpy)),
        ("JIT Unrolled", lambda: sum_ages_jit_unrolled(ages_numpy)),
        ("JIT SIMD-like", lambda: sum_ages_jit_simd_like(ages_numpy)),
        ("JIT Parallel", lambda: sum_ages_jit_parallel(ages_numpy)),
        ("JIT GODLIKE", lambda: sum_ages_jit_godlike(ages_numpy)),
        ("Multiprocessing", lambda: sum_ages_multiprocessing(ages_numpy)),
        ("JIT + MP", lambda: sum_ages_jit_multiprocessing(ages_numpy)),
    ]
    
    # Прогреваем JIT функции
    print("🔥 Warming up JIT functions...")
    small_test = ages_numpy[:1000]
    sum_ages_jit_basic(small_test)
    sum_ages_jit_unrolled(small_test)
    sum_ages_jit_simd_like(small_test)
    sum_ages_jit_parallel(small_test)
    sum_ages_jit_godlike(small_test)
    print("✅ JIT warmup complete!\n")
    
    baseline_time = None
    
    for name, func in algorithms:
        print(f"🔥 {name.upper()} VERSION:")
        
        # Garbage collection для честного сравнения
        gc.collect()
        
        # Измеряем время
        start_time = time.perf_counter()
        total_age = func()
        elapsed_time = time.perf_counter() - start_time
        
        avg_age = total_age // num_users
        elapsed_ms = elapsed_time * 1000
        
        if baseline_time is None:
            baseline_time = elapsed_ms
            print_results_blazing(name, avg_age, elapsed_ms)
        else:
            speedup = baseline_time / elapsed_ms
            print_results_blazing(name, avg_age, elapsed_ms, speedup)
        
        results.append((name, avg_age, elapsed_ms))
        print()
    
    # Находим самый быстрый алгоритм
    fastest = min(results, key=lambda x: x[2])
    max_speedup = baseline_time / fastest[2]
    
    print("📊 PERFORMANCE SUMMARY:")
    print("=" * 60)
    for name, avg_age, elapsed_ms in results:
        speedup = baseline_time / elapsed_ms
        print(f"{name:20s}: {elapsed_ms:8.3f}ms ({speedup:5.1f}x)")
    print("=" * 60)
    
    print(f"\n🏆⚡ АБСОЛЮТНЫЙ ПОБЕДИТЕЛЬ PYTHON: {fastest[0]}")
    print(f"    Время: {fastest[2]:.3f}ms")
    print(f"    Ускорение: {max_speedup:.1f}x\n")
    
    if max_speedup > 50:
        print("🌌⚡ PYTHON ДОСТИГ LUDICROUS SPEED! ⚡🌌\n")
    elif max_speedup > 20:
        print("🚀 PYTHON РАБОТАЕТ С BLAZING SPEED! 🚀\n")
    
    # Сравнение с другими языками
    print("🔬 COMPARISON WITH OTHER LANGUAGES:")
    print("Rust PARALLEL LUDICROUS: ~2.32ms (37x faster)")
    print("C++ Expected: ~2-3ms (35-50x faster)")
    print(f"Python JIT Best: {fastest[2]:.2f}ms ({max_speedup:.1f}x faster)")
    print()
    
    # Записываем результаты в файл
    write_results_to_file(results)
    print("💾 Results written to: blazing_results_python.txt")
    
    # Технические детали
    print("\n🎯 PYTHON OPTIMIZATION SUMMARY:")
    print("• Numba JIT: LLVM-based native code compilation")
    print("• NumPy: Vectorized operations in C")
    print("• Multiprocessing: True parallelism bypassing GIL")
    print("• Memory views: Zero-copy data access")
    print("• Type annotations: Better JIT optimization")
    print("• Caching: Compiled code reuse")
    print("• Loop unrolling: Reduced iteration overhead")
    print("• Chunked processing: Better memory locality")
    
    print(f"\n💥 PYTHON JIT VERSION COMPLETE! 💥")
    print("🐍 Even Python can be BLAZING FAST with the right tools! 🐍⚡")

if __name__ == "__main__":
    main()
