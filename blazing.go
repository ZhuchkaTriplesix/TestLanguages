package main

import (
	"fmt"
	"os"
	"runtime"
	"sort"
	"strconv"
	"sync"
	"time"
	"unsafe"
)

// 🚀⚡ GO BLAZING FAST BENCHMARK SUITE ⚡🚀
//
// Go features:
// - Goroutines (lightweight threads)
// - Channels (CSP communication)
// - Garbage collector (optimized)
// - Static compilation
// - Excellent standard library
// - Built-in concurrency primitives

// User represents a user with traditional struct layout
type User struct {
	ID   uint32
	Name string
	Age  uint8
}

// UserSoA represents users in Struct of Arrays layout for cache efficiency
type UserSoA struct {
	IDs   []uint32
	Names []string
	Ages  []uint8
}

// NewUserSoA creates a new SoA structure with given capacity
func NewUserSoA(capacity int) *UserSoA {
	return &UserSoA{
		IDs:   make([]uint32, capacity),
		Names: make([]string, capacity),
		Ages:  make([]uint8, capacity),
	}
}

// SetUser sets user data at given index
func (u *UserSoA) SetUser(index int, id uint32, name string, age uint8) {
	u.IDs[index] = id
	u.Names[index] = name
	u.Ages[index] = age
}

// Len returns the number of users
func (u *UserSoA) Len() int {
	return len(u.Ages)
}

// 🔥 BLAZING FAST SUM IMPLEMENTATIONS 🔥

// SumAgesBasic - basic array of structs approach
func SumAgesBasic(users []User) uint64 {
	var sum uint64
	for _, user := range users {
		sum += uint64(user.Age)
	}
	return sum
}

// SumAgesSoA - optimized Struct of Arrays approach
func SumAgesSoA(users *UserSoA) uint64 {
	var sum uint64
	ages := users.Ages
	for i := 0; i < len(ages); i++ {
		sum += uint64(ages[i])
	}
	return sum
}

// SumAgesUnrolled - manual loop unrolling for better performance
func SumAgesUnrolled(users *UserSoA) uint64 {
	var sum uint64
	ages := users.Ages
	length := len(ages)
	
	// Process 8 elements at a time (manual SIMD-like optimization)
	i := 0
	for i <= length-8 {
		sum += uint64(ages[i]) + uint64(ages[i+1]) + uint64(ages[i+2]) + uint64(ages[i+3]) +
			   uint64(ages[i+4]) + uint64(ages[i+5]) + uint64(ages[i+6]) + uint64(ages[i+7])
		i += 8
	}
	
	// Handle remaining elements
	for i < length {
		sum += uint64(ages[i])
		i++
	}
	
	return sum
}

// SumAgesUnsafe - unsafe pointer arithmetic for maximum speed
func SumAgesUnsafe(users *UserSoA) uint64 {
	if len(users.Ages) == 0 {
		return 0
	}
	
	var sum uint64
	ages := users.Ages
	length := len(ages)
	
	// Get pointer to the first element
	ptr := unsafe.Pointer(&ages[0])
	
	// Process elements using unsafe pointer arithmetic
	for i := 0; i < length; i++ {
		// Calculate pointer offset
		elementPtr := (*uint8)(unsafe.Pointer(uintptr(ptr) + uintptr(i)))
		sum += uint64(*elementPtr)
	}
	
	return sum
}

// SumAgesGoroutines - parallel processing with goroutines
func SumAgesGoroutines(users *UserSoA) uint64 {
	numWorkers := runtime.NumCPU()
	ages := users.Ages
	length := len(ages)
	
	if length == 0 {
		return 0
	}
	
	// Calculate chunk size for each goroutine
	chunkSize := length / numWorkers
	if chunkSize == 0 {
		chunkSize = 1
		numWorkers = length
	}
	
	// Channel to collect results
	results := make(chan uint64, numWorkers)
	var wg sync.WaitGroup
	
	// Launch goroutines
	for i := 0; i < numWorkers; i++ {
		wg.Add(1)
		go func(start int) {
			defer wg.Done()
			
			end := start + chunkSize
			if end > length {
				end = length
			}
			
			var localSum uint64
			for j := start; j < end; j++ {
				localSum += uint64(ages[j])
			}
			
			results <- localSum
		}(i * chunkSize)
	}
	
	// Close results channel when all goroutines are done
	go func() {
		wg.Wait()
		close(results)
	}()
	
	// Collect results
	var totalSum uint64
	for result := range results {
		totalSum += result
	}
	
	return totalSum
}

// SumAgesGoroutinesUnrolled - parallel processing with loop unrolling
func SumAgesGoroutinesUnrolled(users *UserSoA) uint64 {
	numWorkers := runtime.NumCPU()
	ages := users.Ages
	length := len(ages)
	
	if length == 0 {
		return 0
	}
	
	// Calculate chunk size for each goroutine
	chunkSize := length / numWorkers
	if chunkSize == 0 {
		chunkSize = 1
		numWorkers = length
	}
	
	// Channel to collect results
	results := make(chan uint64, numWorkers)
	var wg sync.WaitGroup
	
	// Launch goroutines
	for i := 0; i < numWorkers; i++ {
		wg.Add(1)
		go func(start int) {
			defer wg.Done()
			
			end := start + chunkSize
			if end > length {
				end = length
			}
			
			var localSum uint64
			
			// Loop unrolling within each goroutine
			j := start
			for j <= end-8 {
				localSum += uint64(ages[j]) + uint64(ages[j+1]) + uint64(ages[j+2]) + uint64(ages[j+3]) +
						   uint64(ages[j+4]) + uint64(ages[j+5]) + uint64(ages[j+6]) + uint64(ages[j+7])
				j += 8
			}
			
			// Handle remaining elements
			for j < end {
				localSum += uint64(ages[j])
				j++
			}
			
			results <- localSum
		}(i * chunkSize)
	}
	
	// Close results channel when all goroutines are done
	go func() {
		wg.Wait()
		close(results)
	}()
	
	// Collect results
	var totalSum uint64
	for result := range results {
		totalSum += result
	}
	
	return totalSum
}

// SumAgesChannels - CSP-style communication with channels
func SumAgesChannels(users *UserSoA) uint64 {
	numWorkers := runtime.NumCPU()
	ages := users.Ages
	length := len(ages)
	
	if length == 0 {
		return 0
	}
	
	// Input channel for work distribution
	work := make(chan []uint8, numWorkers)
	results := make(chan uint64, numWorkers)
	
	// Start workers
	for i := 0; i < numWorkers; i++ {
		go func() {
			var localSum uint64
			for chunk := range work {
				for _, age := range chunk {
					localSum += uint64(age)
				}
			}
			results <- localSum
		}()
	}
	
	// Distribute work
	chunkSize := length / numWorkers
	if chunkSize == 0 {
		chunkSize = 1
	}
	
	for i := 0; i < length; i += chunkSize {
		end := i + chunkSize
		if end > length {
			end = length
		}
		work <- ages[i:end]
	}
	close(work)
	
	// Collect results
	var totalSum uint64
	for i := 0; i < numWorkers; i++ {
		totalSum += <-results
	}
	
	return totalSum
}

// 📊 BENCHMARK FRAMEWORK 📊

type BenchmarkResult struct {
	Name   string
	TimeMs float64
	Result uint64
}

func (br BenchmarkResult) String() string {
	return fmt.Sprintf("%s: %.3fms (result: %d)", br.Name, br.TimeMs, br.Result)
}

// Benchmark runs a function and measures its performance
func Benchmark(name string, fn func() uint64) BenchmarkResult {
	// Warmup
	fn()
	
	// Measure
	start := time.Now()
	result := fn()
	elapsed := time.Since(start)
	
	return BenchmarkResult{
		Name:   name,
		TimeMs: float64(elapsed.Nanoseconds()) / 1e6,
		Result: result,
	}
}

// 📊 MAIN BENCHMARK SUITE 📊

func main() {
	fmt.Println("🚀⚡ GO BLAZING FAST BENCHMARK SUITE ⚡🚀")
	fmt.Println()
	fmt.Println("📊 SYSTEM INFO:")
	fmt.Printf("   Go Version: %s\n", runtime.Version())
	fmt.Printf("   Platform: %s/%s\n", runtime.GOOS, runtime.GOARCH)
	fmt.Printf("   CPU Cores: %d\n", runtime.NumCPU())
	fmt.Printf("   Goroutines: %d\n", runtime.NumGoroutine())
	fmt.Println()
	
	// Get number of users from environment or default
	numUsersStr := os.Getenv("NUM_USERS")
	numUsers := 1000000 // default
	if numUsersStr != "" {
		if parsed, err := strconv.Atoi(numUsersStr); err == nil {
			numUsers = parsed
		}
	}
	
	fmt.Printf("Processing %d users\n", numUsers)
	fmt.Println("Go is optimized for concurrency and simplicity!")
	fmt.Println()
	
	// 🏗️ DATA CREATION
	fmt.Println("🏗️ Creating test data...")
	start := time.Now()
	
	// Traditional Array of Structs
	users := make([]User, numUsers)
	for i := 0; i < numUsers; i++ {
		users[i] = User{
			ID:   uint32(i),
			Name: fmt.Sprintf("User %d", i),
			Age:  uint8(i % 100),
		}
	}
	
	// Struct of Arrays
	usersSoA := NewUserSoA(numUsers)
	for i := 0; i < numUsers; i++ {
		usersSoA.SetUser(i, uint32(i), fmt.Sprintf("User %d", i), uint8(i%100))
	}
	
	elapsed := time.Since(start)
	fmt.Printf("Data creation: %dms\n", elapsed.Milliseconds())
	fmt.Println()
	
	// 🚀 BENCHMARKS
	fmt.Println("🚀 Running benchmarks...")
	fmt.Println()
	
	var results []BenchmarkResult
	
	// Basic approaches
	results = append(results, Benchmark("Go AoS Basic", func() uint64 {
		return SumAgesBasic(users)
	}))
	
	results = append(results, Benchmark("Go SoA Basic", func() uint64 {
		return SumAgesSoA(usersSoA)
	}))
	
	// Optimized approaches
	results = append(results, Benchmark("Go Unrolled", func() uint64 {
		return SumAgesUnrolled(usersSoA)
	}))
	
	results = append(results, Benchmark("Go Unsafe", func() uint64 {
		return SumAgesUnsafe(usersSoA)
	}))
	
	// Concurrent approaches
	results = append(results, Benchmark("Go Goroutines", func() uint64 {
		return SumAgesGoroutines(usersSoA)
	}))
	
	results = append(results, Benchmark("Go Goroutines Unrolled", func() uint64 {
		return SumAgesGoroutinesUnrolled(usersSoA)
	}))
	
	results = append(results, Benchmark("Go Channels", func() uint64 {
		return SumAgesChannels(usersSoA)
	}))
	
	// 📊 RESULTS
	fmt.Println("📊 RESULTS:")
	fmt.Println()
	
	// Sort by performance (fastest first)
	sort.Slice(results, func(i, j int) bool {
		return results[i].TimeMs < results[j].TimeMs
	})
	
	fastest := results[0].TimeMs
	
	for i, result := range results {
		var emoji string
		switch i {
		case 0:
			emoji = "🥇"
		case 1:
			emoji = "🥈"
		case 2:
			emoji = "🥉"
		default:
			emoji = "🔸"
		}
		
		speedup := fastest / result.TimeMs
		fmt.Printf("%s %s: %.3fms (%.1fx)\n", emoji, result.Name, result.TimeMs, speedup)
	}
	
	fmt.Println()
	fmt.Println("🎯 GO PERFORMANCE INSIGHTS:")
	fmt.Println("   • Goroutines provide excellent concurrency with low overhead")
	fmt.Println("   • Loop unrolling improves performance on predictable data")
	fmt.Println("   • Unsafe operations can provide C-like performance")
	fmt.Println("   • Channels enable elegant CSP-style communication")
	fmt.Println("   • Static compilation ensures consistent performance")
	fmt.Println("   • Garbage collector is optimized for low latency")
	fmt.Println()
	
	// Verify all results are the same
	// Sum of ages: for each complete cycle of 100 users (ages 0-99), sum = 0+1+...+99 = 4950
	// For remaining users: sum = 0+1+...+(remainder-1)
	completeCycles := numUsers / 100
	remainder := numUsers % 100
	expectedSum := uint64(completeCycles*4950 + (remainder*(remainder-1))/2)
	
	allMatch := true
	for _, result := range results {
		if result.Result != expectedSum {
			allMatch = false
			break
		}
	}
	
	if allMatch {
		fmt.Println("✅ Verification: All results match!")
	} else {
		fmt.Println("❌ Verification: ERROR: Results don't match!")
	}
	fmt.Printf("   Expected sum: %d\n", expectedSum)
	fmt.Print("   Actual results: {")
	uniqueResults := make(map[uint64]bool)
	for _, result := range results {
		uniqueResults[result.Result] = true
	}
	first := true
	for result := range uniqueResults {
		if !first {
			fmt.Print(", ")
		}
		fmt.Print(result)
		first = false
	}
	fmt.Println("}")
	fmt.Println()
	
	// Performance summary
	bestTime := results[0].TimeMs
	fmt.Printf("🏆 GO CHAMPION: %s\n", results[0].Name)
	fmt.Printf("⚡ Best time: %.3fms\n", bestTime)
	fmt.Printf("🚀 Elements per second: %.0f\n", float64(numUsers)/(bestTime/1000))
	fmt.Println()
	
	// Write results to file
	if file, err := os.Create("blazing_results_go.txt"); err == nil {
		defer file.Close()
		
		fmt.Fprintln(file, "🚀⚡ GO BLAZING FAST BENCHMARK RESULTS ⚡🚀")
		fmt.Fprintln(file)
		fmt.Fprintf(file, "System: %s/%s\n", runtime.GOOS, runtime.GOARCH)
		fmt.Fprintf(file, "Go: %s\n", runtime.Version())
		fmt.Fprintf(file, "CPU Cores: %d\n", runtime.NumCPU())
		fmt.Fprintf(file, "Elements: %d\n", numUsers)
		fmt.Fprintln(file)
		
		for _, result := range results {
			fmt.Fprintln(file, result.String())
		}
		
		fmt.Fprintln(file)
		fmt.Fprintf(file, "Best: %s - %.3fms\n", results[0].Name, bestTime)
		
		fmt.Println("📝 Results saved to blazing_results_go.txt")
	} else {
		fmt.Printf("❌ Failed to save results: %v\n", err)
	}
	
	fmt.Println()
	fmt.Println("🎉 Go benchmark complete!")
}

