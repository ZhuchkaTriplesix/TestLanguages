# 🚀⚡ ASSEMBLY BLAZING FAST MAKEFILE ⚡🚀

CC = gcc
CFLAGS = -O3 -march=native -msse2 -mavx2 -funroll-loops -flto -Wall -Wextra -std=c11
TARGET = blazing_asm
SOURCE = blazing_asm_inline.c

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(SOURCE)
	@echo "🚀 Building Assembly benchmark with maximum optimizations..."
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)
	@echo "✅ Assembly build complete!"

# Alternative builds for different compilers
gcc: 
	gcc -O3 -march=native -msse2 -mavx2 -funroll-loops -flto -Wall -Wextra -std=c11 blazing_asm_inline.c -o blazing_asm_gcc

clang:
	clang -O3 -march=native -msse2 -mavx2 -funroll-loops -flto -Wall -Wextra -std=c11 blazing_asm_inline.c -o blazing_asm_clang

# Debug build
debug:
	$(CC) -g -O0 -Wall -Wextra -std=c11 -DDEBUG blazing_asm_inline.c -o blazing_asm_debug

# Profile build (for profiling with gprof)
profile:
	$(CC) -O2 -pg -Wall -Wextra -std=c11 blazing_asm_inline.c -o blazing_asm_profile

# Build with pure NASM (if NASM is available)
nasm: blazing.asm
	@echo "🚀 Building pure Assembly with NASM..."
	nasm -f elf64 blazing.asm -o blazing_nasm.o
	ld blazing_nasm.o -o blazing_nasm
	@echo "✅ Pure Assembly build complete!"

# Run benchmark
run: $(TARGET)
	@echo "🚀 Running Assembly benchmark..."
	./$(TARGET)

# Run with different user counts
run-small: $(TARGET)
	./$(TARGET) 100000

run-large: $(TARGET)
	./$(TARGET) 10000000

run-extreme: $(TARGET)
	./$(TARGET) 100000000

# Performance comparison
compare: $(TARGET)
	@echo "🏆 Assembly Performance Comparison:"
	@echo "100K users:"
	./$(TARGET) 100000
	@echo ""
	@echo "1M users:"
	./$(TARGET) 1000000
	@echo ""
	@echo "10M users:"
	./$(TARGET) 10000000

# Clean build artifacts
clean:
	rm -f $(TARGET) blazing_asm_gcc blazing_asm_clang blazing_asm_debug blazing_asm_profile blazing_nasm blazing_nasm.o blazing_results_asm.txt

# Install dependencies (if needed)
install-deps:
	@echo "Assembly requires no additional dependencies beyond a compiler"

# Help
help:
	@echo "🚀⚡ ASSEMBLY BLAZING FAST BUILD HELP ⚡🚀"
	@echo ""
	@echo "Available targets:"
	@echo "  all        - Build optimized executable (default)"
	@echo "  gcc        - Build with GCC"
	@echo "  clang      - Build with Clang"
	@echo "  nasm       - Build pure Assembly with NASM"
	@echo "  debug      - Build debug version"
	@echo "  profile    - Build profile version"
	@echo "  run        - Build and run benchmark"
	@echo "  run-small  - Run with 100K users"
	@echo "  run-large  - Run with 10M users"
	@echo "  run-extreme- Run with 100M users"
	@echo "  compare    - Run performance comparison"
	@echo "  clean      - Remove build artifacts"
	@echo "  help       - Show this help"

.PHONY: all gcc clang nasm debug profile run run-small run-large run-extreme compare clean install-deps help

