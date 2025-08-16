#!/usr/bin/env python3
"""
🚀⚡ Cython Setup Script for BLAZING FAST compilation ⚡🚀
"""

from setuptools import setup, Extension
from Cython.Build import cythonize
import numpy as np
import os
import platform

# Определяем compiler flags для максимальной производительности
extra_compile_args = []
extra_link_args = []

if platform.system() == "Windows":
    # MSVC flags
    extra_compile_args = [
        "/O2",          # Максимальная оптимизация
        "/Oi",          # Inline intrinsic functions
        "/Ot",          # Favor fast code
        "/Oy",          # Frame pointer omission
        "/GL",          # Whole program optimization
        "/arch:AVX2",   # AVX2 instructions
        "/fp:fast",     # Fast floating point
        "/openmp",      # OpenMP support
    ]
    extra_link_args = ["/LTCG"]  # Link-time code generation
else:
    # GCC/Clang flags
    extra_compile_args = [
        "-O3",                    # Максимальная оптимизация
        "-march=native",          # Native CPU optimization
        "-mtune=native",         # Tune for native CPU
        "-ffast-math",           # Fast math operations
        "-funroll-loops",        # Loop unrolling
        "-finline-functions",    # Function inlining
        "-fomit-frame-pointer",  # Omit frame pointer
        "-fopenmp",              # OpenMP support
        "-mavx2",                # AVX2 instructions (if supported)
        "-fno-signed-zeros",     # Optimize signed zeros
        "-fno-trapping-math",    # No math traps
        "-frename-registers",    # Register renaming
        "-fthread-jumps",        # Thread jumps optimization
    ]
    extra_link_args = ["-fopenmp"]

# Compiler directives для Cython
compiler_directives = {
    "language_level": 3,           # Python 3
    "boundscheck": False,          # Отключить проверки границ
    "wraparound": False,           # Отключить wraparound indexing
    "nonecheck": False,            # Отключить проверки None
    "cdivision": True,             # C-style division
    "profile": False,              # Отключить профилирование
    "linetrace": False,            # Отключить line tracing
    "infer_types": True,           # Автоматический вывод типов
    "optimize.use_switch": True,   # Оптимизированные switch statements
    "optimize.unpack_method_calls": True,  # Оптимизация method calls
    "warn.undeclared": False,      # Отключить предупреждения
    "warn.unreachable": False,     # Отключить предупреждения о недостижимом коде
}

# Определяем extensions
extensions = [
    Extension(
        "blazing_cython",
        ["blazing_cython.pyx"],
        include_dirs=[np.get_include()],
        extra_compile_args=extra_compile_args,
        extra_link_args=extra_link_args,
        define_macros=[
            ("NPY_NO_DEPRECATED_API", "NPY_1_7_API_VERSION"),
            ("CYTHON_TRACE", "0"),  # Отключить tracing для максимальной скорости
        ],
    )
]

# Компилируем с максимальными оптимизациями
setup(
    name="blazing_cython",
    version="1.0.0",
    description="🚀⚡ BLAZING FAST Cython optimized code ⚡🚀",
    ext_modules=cythonize(
        extensions,
        compiler_directives=compiler_directives,
        annotate=True,  # Создать HTML файл с аннотациями
        nthreads=os.cpu_count(),  # Параллельная компиляция
    ),
    zip_safe=False,
    python_requires=">=3.7",
    install_requires=[
        "numpy",
        "cython",
        "psutil",  # Для информации о памяти
    ],
)

if __name__ == "__main__":
    print("🚀⚡ BLAZING FAST Cython compilation starting... ⚡🚀")
    print(f"Platform: {platform.system()}")
    print(f"CPU cores for compilation: {os.cpu_count()}")
    print(f"Compiler flags: {' '.join(extra_compile_args)}")
    print()
    
    # Инструкции по компиляции
    print("📋 Compilation Instructions:")
    print("python setup_cython.py build_ext --inplace")
    print()
    print("🎯 Expected Performance:")
    print("• Cython should achieve 10-100x speedup over pure Python")
    print("• With OpenMP: Additional 2-8x speedup on multi-core")
    print("• Pointer arithmetic: Maximum C-like performance")
    print()
    print("🔥 Ready for LUDICROUS SPEED compilation! 🔥")
