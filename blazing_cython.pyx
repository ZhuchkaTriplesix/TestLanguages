# cython: language_level=3
# cython: boundscheck=False
# cython: wraparound=False
# cython: nonecheck=False
# cython: cdivision=True
# cython: profile=False
# cython: linetrace=False
# cython: infer_types=True
# cython: optimize.use_switch=True

"""
🚀⚡ BLAZING FAST Python Cython VERSION ⚡🚀

Максимальные оптимизации Cython:
- Статическая типизация
- Отключенные проверки границ
- C-скорость циклов
- Inline функции
- Memory views
- OpenMP параллелизм
"""

import numpy as np
cimport numpy as cnp
from libc.stdlib cimport malloc, free
from libc.stdint cimport uint8_t, uint64_t, int64_t
cimport cython
from cython.parallel import prange, parallel
import time
import os

# Определяем типы для лучшей производительности
ctypedef cnp.uint8_t uint8_t
ctypedef cnp.uint64_t uint64_t
ctypedef cnp.int64_t int64_t

# C структура для максимальной производительности
cdef struct CUser:
    int64_t id
    uint8_t age

cdef class FastUserSoA:
    """Cython оптимизированная версия SoA"""
    cdef:
        uint8_t* ages
        int64_t* ids
        size_t capacity
        size_t size
    
    def __cinit__(self, size_t capacity):
        self.capacity = capacity
        self.size = 0
        self.ages = <uint8_t*>malloc(capacity * sizeof(uint8_t))
        self.ids = <int64_t*>malloc(capacity * sizeof(int64_t))
        
        if not self.ages or not self.ids:
            raise MemoryError("Failed to allocate memory")
    
    def __dealloc__(self):
        if self.ages:
            free(self.ages)
        if self.ids:
            free(self.ids)
    
    @cython.boundscheck(False)
    @cython.wraparound(False)
    cdef void add_user_fast(self, int64_t user_id, uint8_t age) nogil:
        """Быстрое добавление пользователя без проверок"""
        if self.size < self.capacity:
            self.ages[self.size] = age
            self.ids[self.size] = user_id
            self.size += 1
    
    def add_user(self, int64_t user_id, str name, uint8_t age):
        """Python интерфейс для добавления пользователя"""
        with nogil:
            self.add_user_fast(user_id, age)
    
    @cython.boundscheck(False)
    @cython.wraparound(False)
    def get_ages_array(self):
        """Возвращает NumPy массив возрастов"""
        cdef cnp.ndarray[uint8_t, ndim=1] result = np.empty(self.size, dtype=np.uint8)
        cdef size_t i
        
        for i in range(self.size):
            result[i] = self.ages[i]
        
        return result

# 🔥 CYTHON OPTIMIZED FUNCTIONS 🔥

@cython.boundscheck(False)
@cython.wraparound(False)
@cython.cdivision(True)
cdef uint64_t sum_ages_cython_basic(uint8_t[::1] ages) nogil:
    """Базовая Cython версия с memory view"""
    cdef uint64_t total = 0
    cdef size_t i, n = ages.shape[0]
    
    for i in range(n):
        total += ages[i]
    
    return total

@cython.boundscheck(False)
@cython.wraparound(False)
@cython.cdivision(True)
cdef uint64_t sum_ages_cython_unrolled(uint8_t[::1] ages) nogil:
    """Cython с loop unrolling"""
    cdef uint64_t total = 0
    cdef size_t i = 0, n = ages.shape[0]
    
    # Unroll по 8 элементов
    while i + 8 <= n:
        total += (ages[i] + ages[i+1] + ages[i+2] + ages[i+3] + 
                 ages[i+4] + ages[i+5] + ages[i+6] + ages[i+7])
        i += 8
    
    # Остальные элементы
    while i < n:
        total += ages[i]
        i += 1
    
    return total

@cython.boundscheck(False)
@cython.wraparound(False)
@cython.cdivision(True)
cdef uint64_t sum_ages_cython_simd_like(uint8_t[::1] ages) nogil:
    """SIMD-подобная обработка в Cython"""
    cdef uint64_t total = 0
    cdef size_t i = 0, j, n = ages.shape[0]
    cdef uint64_t chunk_sum
    
    # Обрабатываем по 16 элементов
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

@cython.boundscheck(False)
@cython.wraparound(False)
@cython.cdivision(True)
cdef uint64_t sum_ages_cython_parallel(uint8_t[::1] ages) nogil:
    """Параллельная Cython версия с OpenMP"""
    cdef uint64_t total = 0
    cdef size_t i, n = ages.shape[0]
    
    # Параллельное суммирование
    for i in prange(n, schedule='static'):
        total += ages[i]
    
    return total

@cython.boundscheck(False)
@cython.wraparound(False)
@cython.cdivision(True)
cdef uint64_t sum_ages_cython_godlike(uint8_t[::1] ages) nogil:
    """GODLIKE Cython версия с максимальными оптимизациями"""
    cdef uint64_t total = 0
    cdef size_t i, j, n = ages.shape[0]
    cdef size_t chunk_size = 1024  # Оптимальный размер чанка для кэша
    cdef size_t num_chunks = (n + chunk_size - 1) // chunk_size
    cdef size_t start, end
    cdef uint64_t chunk_sum
    
    # Параллельная обработка чанков
    for i in prange(num_chunks, schedule='static'):
        start = i * chunk_size
        end = start + chunk_size
        if end > n:
            end = n
        
        chunk_sum = 0
        
        # Unrolled loop внутри чанка
        j = start
        while j + 8 <= end:
            chunk_sum += (ages[j] + ages[j+1] + ages[j+2] + ages[j+3] + 
                         ages[j+4] + ages[j+5] + ages[j+6] + ages[j+7])
            j += 8
        
        while j < end:
            chunk_sum += ages[j]
            j += 1
        
        total += chunk_sum
    
    return total

@cython.boundscheck(False)
@cython.wraparound(False)
cdef uint64_t sum_ages_pointer_arithmetic(uint8_t* ages, size_t n) nogil:
    """Версия с pointer arithmetic для максимальной скорости"""
    cdef uint64_t total = 0
    cdef uint8_t* ptr = ages
    cdef uint8_t* end_ptr = ages + n
    cdef uint8_t* chunk_end
    
    # Обрабатываем по 8 элементов
    chunk_end = end_ptr - (n % 8)
    while ptr < chunk_end:
        total += (ptr[0] + ptr[1] + ptr[2] + ptr[3] + 
                 ptr[4] + ptr[5] + ptr[6] + ptr[7])
        ptr += 8
    
    # Остальные элементы
    while ptr < end_ptr:
        total += ptr[0]
        ptr += 1
    
    return total

# Python обертки для C функций

def sum_ages_cy_basic(cnp.ndarray[uint8_t, ndim=1] ages):
    """Python обертка для базовой Cython версии"""
    return sum_ages_cython_basic(ages)

def sum_ages_cy_unrolled(cnp.ndarray[uint8_t, ndim=1] ages):
    """Python обертка для unrolled версии"""
    return sum_ages_cython_unrolled(ages)

def sum_ages_cy_simd_like(cnp.ndarray[uint8_t, ndim=1] ages):
    """Python обертка для SIMD-like версии"""
    return sum_ages_cython_simd_like(ages)

def sum_ages_cy_parallel(cnp.ndarray[uint8_t, ndim=1] ages):
    """Python обертка для параллельной версии"""
    return sum_ages_cython_parallel(ages)

def sum_ages_cy_godlike(cnp.ndarray[uint8_t, ndim=1] ages):
    """Python обертка для GODLIKE версии"""
    return sum_ages_cython_godlike(ages)

def sum_ages_cy_pointer(cnp.ndarray[uint8_t, ndim=1] ages):
    """Python обертка для pointer arithmetic версии"""
    cdef uint8_t* ptr = &ages[0]
    cdef size_t n = ages.shape[0]
    return sum_ages_pointer_arithmetic(ptr, n)

# Дополнительные оптимизированные функции

@cython.boundscheck(False)
@cython.wraparound(False)
def create_test_data_fast(size_t n):
    """Быстрое создание тестовых данных"""
    cdef cnp.ndarray[uint8_t, ndim=1] ages = np.empty(n, dtype=np.uint8)
    cdef size_t i
    
    with nogil:
        for i in range(n):
            ages[i] = i % 100
    
    return ages

@cython.boundscheck(False)
@cython.wraparound(False) 
def benchmark_cython_versions(cnp.ndarray[uint8_t, ndim=1] ages):
    """Бенчмарк всех Cython версий"""
    cdef dict results = {}
    cdef double start_time, elapsed_time
    cdef uint64_t result
    
    # Список всех функций для тестирования
    functions = [
        ("Cython Basic", sum_ages_cy_basic),
        ("Cython Unrolled", sum_ages_cy_unrolled), 
        ("Cython SIMD-like", sum_ages_cy_simd_like),
        ("Cython Parallel", sum_ages_cy_parallel),
        ("Cython GODLIKE", sum_ages_cy_godlike),
        ("Cython Pointer", sum_ages_cy_pointer),
    ]
    
    for name, func in functions:
        start_time = time.perf_counter()
        result = func(ages)
        elapsed_time = time.perf_counter() - start_time
        results[name] = {
            'result': result,
            'time_ms': elapsed_time * 1000,
            'avg_age': result // len(ages)
        }
    
    return results

# Utility функции

@cython.boundscheck(False)
@cython.wraparound(False)
def memory_usage_info():
    """Информация об использовании памяти"""
    import psutil
    process = psutil.Process()
    memory_info = process.memory_info()
    return {
        'rss_mb': memory_info.rss / 1024 / 1024,
        'vms_mb': memory_info.vms / 1024 / 1024,
        'percent': process.memory_percent()
    }

def print_cython_info():
    """Информация о Cython компиляции"""
    print("🔧 Cython Compilation Info:")
    print(f"   • Bounds checking: {'Disabled' if not cython.boundscheck else 'Enabled'}")
    print(f"   • Wraparound: {'Disabled' if not cython.wraparound else 'Enabled'}")
    print(f"   • Division checking: {'Disabled' if cython.cdivision else 'Enabled'}")
    print(f"   • OpenMP support: Available")
    print(f"   • Memory views: Enabled")
    print(f"   • C speed loops: Enabled")
    print()

# Экспортируемые классы и функции
__all__ = [
    'FastUserSoA',
    'sum_ages_cy_basic',
    'sum_ages_cy_unrolled', 
    'sum_ages_cy_simd_like',
    'sum_ages_cy_parallel',
    'sum_ages_cy_godlike',
    'sum_ages_cy_pointer',
    'create_test_data_fast',
    'benchmark_cython_versions',
    'memory_usage_info',
    'print_cython_info'
]
