import 'dart:io';
import 'dart:isolate';
import 'dart:math' as math;
import 'dart:typed_data';

/// 🚀⚡ DART BLAZING FAST BENCHMARK SUITE ⚡🚀
/// 
/// Dart features:
/// - AOT compilation (fast startup)
/// - JIT compilation (development)
/// - Isolates (parallel processing)
/// - Typed data (Int8List, Uint8List)
/// - Tree shaking optimizations
/// - Sound null safety

class User {
  final int id;
  final String name;
  final int age;
  
  const User({required this.id, required this.name, required this.age});
}

/// Struct of Arrays - memory efficient
class UserSoA {
  late final List<int> ids;
  late final List<String> names;
  late final List<int> ages;
  
  UserSoA(int capacity) {
    ids = List<int>.filled(capacity, 0);
    names = List<String>.filled(capacity, '');
    ages = List<int>.filled(capacity, 0);
  }
  
  void setUser(int index, int id, String name, int age) {
    ids[index] = id;
    names[index] = name;
    ages[index] = age;
  }
  
  int get length => ages.length;
}

/// Ultra-optimized typed data version
class UserTypedData {
  late final Uint32List ids;
  late final List<String> names;
  late final Uint8List ages;  // Ages 0-255
  
  UserTypedData(int capacity) {
    ids = Uint32List(capacity);
    names = List<String>.filled(capacity, '');
    ages = Uint8List(capacity);
  }
  
  void setUser(int index, int id, String name, int age) {
    ids[index] = id;
    names[index] = name;
    ages[index] = age;
  }
  
  int get length => ages.length;
}

/// 🔥 BLAZING FAST SUM IMPLEMENTATIONS 🔥

/// Basic Dart sum
int sumAgesBasic(List<User> users) {
  var sum = 0;
  for (final user in users) {
    sum += user.age;
  }
  return sum;
}

/// Functional approach with fold
int sumAgesFunctional(List<User> users) {
  return users.fold<int>(0, (sum, user) => sum + user.age);
}

/// SoA optimized version
int sumAgesSoA(UserSoA users) {
  var sum = 0;
  final ages = users.ages;
  for (int i = 0; i < ages.length; i++) {
    sum += ages[i];
  }
  return sum;
}

/// Ultra-fast typed data version
int sumAgesTyped(UserTypedData users) {
  var sum = 0;
  final ages = users.ages;
  for (int i = 0; i < ages.length; i++) {
    sum += ages[i];
  }
  return sum;
}

/// SIMD-like optimized version (loop unrolling)
int sumAgesUnrolled(UserTypedData users) {
  var sum = 0;
  final ages = users.ages;
  final len = ages.length;
  
  // Process 8 elements at a time (manual SIMD)
  int i = 0;
  for (; i < len - 7; i += 8) {
    sum += ages[i] + ages[i+1] + ages[i+2] + ages[i+3] +
           ages[i+4] + ages[i+5] + ages[i+6] + ages[i+7];
  }
  
  // Handle remaining elements
  for (; i < len; i++) {
    sum += ages[i];
  }
  
  return sum;
}

/// Parallel processing with isolates and zero-copy views
Future<int> sumAgesParallel(UserTypedData users) async {
  final numIsolates = Platform.numberOfProcessors;
  final chunkSize = (users.length / numIsolates).ceil();
  
  final futures = <Future<int>>[];
  
  for (int i = 0; i < numIsolates; i++) {
    final start = i * chunkSize;
    final end = math.min(start + chunkSize, users.length);
    
    if (start < users.length) {
      final chunk = Uint8List.sublistView(users.ages, start, end);
      futures.add(Isolate.run(() => _sumChunk(chunk)));
    }
  }
  
  final results = await Future.wait(futures);
  return results.fold<int>(0, (sum, result) => sum + result);
}

/// Helper for isolate processing with unrolled loop
int _sumChunk(Uint8List chunk) {
  var sum = 0;
  final len = chunk.length;
  int i = 0;
  for (; i < len - 7; i += 8) {
    sum += chunk[i] + chunk[i+1] + chunk[i+2] + chunk[i+3] +
           chunk[i+4] + chunk[i+5] + chunk[i+6] + chunk[i+7];
  }
  for (; i < len; i++) {
    sum += chunk[i];
  }
  return sum;
}

/// 🚀 BENCHMARK RUNNER 🚀

class BenchmarkResult {
  final String name;
  final double timeMs;
  final int result;
  
  const BenchmarkResult(this.name, this.timeMs, this.result);
  
  @override
  String toString() => '$name: ${timeMs.toStringAsFixed(3)}ms (result: $result)';
}

Future<BenchmarkResult> benchmark(String name, Future<int> Function() fn) async {
  // Warmup
  await fn();
  
  final sw = Stopwatch()..start();
  final result = await fn();
  sw.stop();
  
  return BenchmarkResult(name, sw.elapsedMicroseconds / 1000.0, result);
}

BenchmarkResult benchmarkSync(String name, int Function() fn) {
  // Warmup
  fn();
  
  final sw = Stopwatch()..start();
  final result = fn();
  sw.stop();
  
  return BenchmarkResult(name, sw.elapsedMicroseconds / 1000.0, result);
}

/// 📊 MAIN BENCHMARK SUITE 📊

Future<void> main(List<String> args) async {
  print('🚀⚡ DART BLAZING FAST BENCHMARK SUITE ⚡🚀');
  print('');
  print('📊 SYSTEM INFO:');
  print('   Dart Version: ${Platform.version}');
  print('   Platform: ${Platform.operatingSystem}');
  print('   CPU Cores: ${Platform.numberOfProcessors}');
  print('   Executable: ${Platform.resolvedExecutable}');
  print('');
  
  // Get number of users from environment or default
  final numUsersEnv = Platform.environment['NUM_USERS'];
  final numUsers = numUsersEnv != null ? int.parse(numUsersEnv) : 1000000;
  
  print('Processing $numUsers users');
  print('Dart is optimized for both AOT and JIT compilation!');
  print('');
  
  // 🏗️ DATA CREATION
  print('🏗️ Creating test data...');
  final sw = Stopwatch()..start();
  
  // Traditional AoS
  final users = <User>[];
  for (int i = 0; i < numUsers; i++) {
    users.add(User(
      id: i,
      name: 'User $i',
      age: i % 100,
    ));
  }
  
  // Struct of Arrays
  final usersSoA = UserSoA(numUsers);
  for (int i = 0; i < numUsers; i++) {
    usersSoA.setUser(i, i, 'User $i', i % 100);
  }
  
  // Typed data version
  final usersTyped = UserTypedData(numUsers);
  for (int i = 0; i < numUsers; i++) {
    usersTyped.setUser(i, i, 'User $i', i % 100);
  }
  
  sw.stop();
  print('Data creation: ${sw.elapsedMilliseconds}ms');
  print('');
  
  // 🚀 BENCHMARKS
  print('🚀 Running benchmarks...');
  print('');
  
  final results = <BenchmarkResult>[];
  
  // Basic approaches
  results.add(benchmarkSync('Dart AoS Basic', () => sumAgesBasic(users)));
  results.add(benchmarkSync('Dart AoS Functional', () => sumAgesFunctional(users)));
  results.add(benchmarkSync('Dart SoA Basic', () => sumAgesSoA(usersSoA)));
  
  // Optimized approaches
  results.add(benchmarkSync('Dart Typed Data', () => sumAgesTyped(usersTyped)));
  results.add(benchmarkSync('Dart Unrolled', () => sumAgesUnrolled(usersTyped)));
  
  // Parallel approach (async)
  results.add(await benchmark('Dart Parallel', () => sumAgesParallel(usersTyped)));
  
  // 📊 RESULTS
  print('📊 RESULTS:');
  print('');
  
  // Sort by performance (fastest first)
  results.sort((a, b) => a.timeMs.compareTo(b.timeMs));
  
  final fastest = results.first.timeMs;
  
  for (int i = 0; i < results.length; i++) {
    final result = results[i];
    final emoji = i == 0 ? '🥇' : i == 1 ? '🥈' : i == 2 ? '🥉' : '🔸';
    final speedup = fastest / result.timeMs;
    
    print('$emoji ${result.name}: ${result.timeMs.toStringAsFixed(3)}ms (${speedup.toStringAsFixed(1)}x)');
  }
  
  print('');
  print('🎯 DART PERFORMANCE INSIGHTS:');
  print('   • AOT compilation provides consistent performance');
  print('   • Typed data (Uint8List) is faster than generic lists');
  print('   • Loop unrolling helps with predictable access patterns');
  print('   • Isolates have overhead but scale well for large datasets');
  print('   • Sound null safety enables aggressive optimizations');
  print('');
  
  // Verify all results are the same
  // Sum of ages: for each complete cycle of 100 users (ages 0-99), sum = 0+1+...+99 = 4950
  // For remaining users: sum = 0+1+...+(remainder-1)
  final completeCycles = numUsers ~/ 100;
  final remainder = numUsers % 100;
  final expectedSum = completeCycles * 4950 + (remainder * (remainder - 1) ~/ 2);
  final allMatch = results.every((r) => r.result == expectedSum);
  
  print('✅ Verification: ${allMatch ? "All results match!" : "ERROR: Results don\'t match!"}');
  print('   Expected sum: $expectedSum');
  print('   Actual results: ${results.map((r) => r.result).toSet()}');
  print('');
  
  // Performance summary
  final bestTime = results.first.timeMs;
  print('🏆 DART CHAMPION: ${results.first.name}');
  print('⚡ Best time: ${bestTime.toStringAsFixed(3)}ms');
  print('🚀 Elements per second: ${(numUsers / (bestTime / 1000)).toStringAsFixed(0)}');
  print('');
  
  // Write results to file
  try {
    final file = File('blazing_results_dart.txt');
    final buffer = StringBuffer();
    
    buffer.writeln('🚀⚡ DART BLAZING FAST BENCHMARK RESULTS ⚡🚀');
    buffer.writeln('');
    buffer.writeln('System: ${Platform.operatingSystem}');
    buffer.writeln('Dart: ${Platform.version}');
    buffer.writeln('CPU Cores: ${Platform.numberOfProcessors}');
    buffer.writeln('Elements: $numUsers');
    buffer.writeln('');
    
    for (final result in results) {
      buffer.writeln(result.toString());
    }
    
    buffer.writeln('');
    buffer.writeln('Best: ${results.first.name} - ${bestTime.toStringAsFixed(3)}ms');
    
    await file.writeAsString(buffer.toString());
    print('📝 Results saved to blazing_results_dart.txt');
  } catch (e) {
    print('❌ Failed to save results: $e');
  }
  
  print('');
  print('🎉 Dart benchmark complete!');
}
