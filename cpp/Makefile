# BLAZING FAST C++ Makefile 🚀⚡
CXX = g++
CXXFLAGS = -std=c++17 -O3 -march=native -mtune=native -flto -ffast-math \
           -funroll-loops -finline-functions -fomit-frame-pointer \
           -mavx2 -mavx512bw -mavx512dq -mfma -mbmi2 -fopenmp -DNDEBUG
           
# Дополнительные флаги для максимальной производительности
CXXFLAGS += -Wall -Wextra -Wpedantic -Wno-unused-parameter
CXXFLAGS += -ftree-vectorize -floop-parallelize-all -floop-interchange
CXXFLAGS += -floop-strip-mine -floop-block -fgraphite-identity
CXXFLAGS += -fipa-pta -fdevirtualize-at-ltrans

# Линковка с оптимизациями
LDFLAGS = -flto -fopenmp -ltbb

# Файлы
TARGET = blazing_cpp
SOURCE = blazing.cpp

# Цели
.PHONY: all clean run benchmark release

all: $(TARGET)

$(TARGET): $(SOURCE)
	@echo "🚀 Compiling BLAZING FAST C++ version with maximum optimizations..."
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET) $(LDFLAGS)
	@echo "✅ Compilation complete! Ready for LUDICROUS SPEED!"

release: CXXFLAGS += -DRELEASE_BUILD
release: $(TARGET)

run: $(TARGET)
	@echo "🔥 Running BLAZING FAST C++ version..."
	./$(TARGET)

benchmark: $(TARGET)
	@echo "📊 Running C++ benchmark with 100M elements..."
	NUM_USERS=100000000 ./$(TARGET)

# Профилирование
profile: CXXFLAGS += -pg -g
profile: $(TARGET)
	@echo "📈 Running with profiling..."
	./$(TARGET)
	gprof $(TARGET) gmon.out > profile_analysis.txt

# Анализ сборки
assembly: $(SOURCE)
	$(CXX) $(CXXFLAGS) -S $(SOURCE) -o blazing.s
	@echo "🔍 Assembly code generated in blazing.s"

# Оптимизированная версия с Intel Compiler (если доступен)
intel: CXX = icpc
intel: CXXFLAGS = -std=c++17 -O3 -xHost -ipo -no-prec-div -fp-model fast=2 \
                  -qopenmp -DNDEBUG -march=native -mtune=native
intel: $(TARGET)

# Clang версия
clang: CXX = clang++
clang: CXXFLAGS += -stdlib=libc++
clang: $(TARGET)

clean:
	@echo "🧹 Cleaning up..."
	rm -f $(TARGET) blazing.s gmon.out profile_analysis.txt blazing_results_cpp.txt

# Информация о компиляторе и флагах
info:
	@echo "🔧 Compiler: $(CXX)"
	@echo "🚀 Flags: $(CXXFLAGS)"
	@echo "🔗 Linker flags: $(LDFLAGS)"
	@$(CXX) --version

# Тест всех версий
test-all: all
	@echo "🧪 Testing all optimization levels..."
	@echo "=== O1 ===" && $(CXX) -std=c++17 -O1 $(SOURCE) -o test_o1 && ./test_o1
	@echo "=== O2 ===" && $(CXX) -std=c++17 -O2 $(SOURCE) -o test_o2 && ./test_o2  
	@echo "=== O3 ===" && $(CXX) -std=c++17 -O3 $(SOURCE) -o test_o3 && ./test_o3
	@echo "=== BLAZING ===" && ./$(TARGET)
	@rm -f test_o1 test_o2 test_o3

help:
	@echo "🚀 BLAZING FAST C++ Build System"
	@echo ""
	@echo "Targets:"
	@echo "  all        - Build with maximum optimizations"
	@echo "  release    - Release build with extra defines"
	@echo "  run        - Compile and run"
	@echo "  benchmark  - Run with 100M elements"
	@echo "  profile    - Build with profiling support"
	@echo "  assembly   - Generate assembly output"
	@echo "  intel      - Build with Intel C++ Compiler"
	@echo "  clang      - Build with Clang"
	@echo "  test-all   - Test all optimization levels"
	@echo "  clean      - Remove build artifacts"
	@echo "  info       - Show compiler information"
	@echo "  help       - Show this help"
	@echo ""
	@echo "🌟 For maximum performance, use: make benchmark"
