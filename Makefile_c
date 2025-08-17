# 🚀⚡ C BLAZING FAST MAKEFILE ⚡🚀

CC = gcc
CFLAGS = -O3 -march=native -mavx2 -mfma -funroll-loops -flto -Wall -Wextra -std=c11
LDFLAGS = -pthread
TARGET = blazing_c
SOURCE = blazing.c

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(SOURCE)
	@echo "🚀 Building C benchmark with maximum optimizations..."
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SOURCE)
	@echo "✅ Build complete!"

# Alternative builds for different compilers
gcc: 
	gcc -O3 -march=native -mavx2 -mfma -funroll-loops -flto -Wall -Wextra -std=c11 -pthread blazing.c -o blazing_c_gcc

clang:
	clang -O3 -march=native -mavx2 -mfma -funroll-loops -flto -Wall -Wextra -std=c11 -pthread blazing.c -o blazing_c_clang

# Debug build
debug:
	$(CC) -g -O0 -Wall -Wextra -std=c11 -pthread -DDEBUG blazing.c -o blazing_c_debug

# Profile build (for profiling with gprof)
profile:
	$(CC) -O2 -pg -Wall -Wextra -std=c11 -pthread blazing.c -o blazing_c_profile

# Run benchmark
run: $(TARGET)
	@echo "🚀 Running C benchmark..."
	./$(TARGET)

# Run with different user counts
run-small: $(TARGET)
	./$(TARGET) 100000

run-large: $(TARGET)
	./$(TARGET) 10000000

# Clean build artifacts
clean:
	rm -f $(TARGET) blazing_c_gcc blazing_c_clang blazing_c_debug blazing_c_profile blazing_results_c.txt

# Install dependencies (if needed)
install-deps:
	@echo "C requires no additional dependencies beyond a compiler and pthread"

# Help
help:
	@echo "🚀⚡ C BLAZING FAST BUILD HELP ⚡🚀"
	@echo ""
	@echo "Available targets:"
	@echo "  all        - Build optimized executable (default)"
	@echo "  gcc        - Build with GCC"
	@echo "  clang      - Build with Clang"
	@echo "  debug      - Build debug version"
	@echo "  profile    - Build profile version"
	@echo "  run        - Build and run benchmark"
	@echo "  run-small  - Run with 100K users"
	@echo "  run-large  - Run with 10M users"
	@echo "  clean      - Remove build artifacts"
	@echo "  help       - Show this help"

.PHONY: all gcc clang debug profile run run-small run-large clean install-deps help

