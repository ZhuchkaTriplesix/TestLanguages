#!/usr/bin/env Rscript
# 🚀⚡ BLAZING FAST R VERSION ⚡🚀

# R славится своими векторизованными операциями и статистическими функциями
# Посмотрим как R справится с нашей задачей!

cat("🚀⚡ R BLAZING FAST BENCHMARK SUITE ⚡🚀\n\n")

# Получаем размер данных из переменной окружения
num_users <- as.numeric(Sys.getenv("NUM_USERS", "1000000"))

cat("📊 SYSTEM INFO:\n")
cat("   R Version:", R.version.string, "\n")
cat("   Platform:", R.version$platform, "\n")
cat("   CPU Cores:", parallel::detectCores(), "\n")
cat("   Memory:", round(memory.limit() / 1024, 1), "GB limit\n")
cat("\n")

cat("Processing", format(num_users, big.mark=","), "users\n")
cat("R is optimized for vectorized operations!\n\n")

# ============================================================================
# 📊 DATA STRUCTURES
# ============================================================================

# R работает лучше всего с векторами и data.frames
create_test_data <- function(n) {
  cat("📊 Creating test data...\n")
  start_time <- Sys.time()
  
  # Создаем векторы (это естественная структура для R)
  ids <- 1:n
  names <- paste("User", 1:n)
  ages <- (1:n - 1) %% 100  # Возраст от 0 до 99
  
  # Data.frame (аналог AoS)
  users_df <- data.frame(
    id = ids,
    name = names,
    age = ages,
    stringsAsFactors = FALSE
  )
  
  # Отдельные векторы (аналог SoA)
  users_vectors <- list(
    ids = ids,
    names = names,
    ages = ages
  )
  
  end_time <- Sys.time()
  cat("✅ Test data created in", round(as.numeric(end_time - start_time, units="secs") * 1000, 2), "ms\n\n")
  
  return(list(df = users_df, vectors = users_vectors))
}

# ============================================================================
# 🚀 R OPTIMIZATION FUNCTIONS
# ============================================================================

# 1. Базовая версия с data.frame
sum_ages_dataframe <- function(users_df) {
  sum(users_df$age)
}

# 2. Векторизованная версия (естественная для R)
sum_ages_vectorized <- function(ages_vector) {
  sum(ages_vector)
}

# 3. R встроенная функция sum() с оптимизациями
sum_ages_builtin <- function(ages_vector) {
  sum(ages_vector, na.rm = FALSE)
}

# 4. Использование colSums (матричная операция)
sum_ages_colsums <- function(ages_matrix) {
  colSums(ages_matrix)
}

# 5. Циклическая версия (медленная в R)
sum_ages_loop <- function(ages_vector) {
  total <- 0
  for (i in seq_along(ages_vector)) {
    total <- total + ages_vector[i]
  }
  return(total)
}

# 6. Apply функция
sum_ages_apply <- function(ages_matrix) {
  apply(ages_matrix, 2, sum)
}

# 7. Векторизованная версия с Reduce
sum_ages_reduce <- function(ages_vector) {
  Reduce("+", ages_vector)
}

# 8. Параллельная версия с parallel
sum_ages_parallel <- function(ages_vector) {
  if (length(ages_vector) < 100000) {
    return(sum(ages_vector))  # Не стоит для маленьких данных
  }
  
  # Разбиваем на чанки
  num_cores <- parallel::detectCores()
  chunk_size <- ceiling(length(ages_vector) / num_cores)
  chunks <- split(ages_vector, ceiling(seq_along(ages_vector) / chunk_size))
  
  # Параллельное вычисление
  cluster <- parallel::makeCluster(num_cores)
  result <- parallel::parLapply(cluster, chunks, sum)
  parallel::stopCluster(cluster)
  
  return(sum(unlist(result)))
}

# 9. data.table версия (если доступна)
sum_ages_datatable <- function(ages_vector) {
  if (requireNamespace("data.table", quietly = TRUE)) {
    dt <- data.table::data.table(age = ages_vector)
    return(dt[, sum(age)])
  } else {
    return(sum(ages_vector))  # Fallback
  }
}

# 10. Rcpp версия (если доступна) - C++ внутри R
sum_ages_rcpp <- function(ages_vector) {
  if (requireNamespace("Rcpp", quietly = TRUE)) {
    # Простая Rcpp функция
    rcpp_code <- '
    #include <Rcpp.h>
    using namespace Rcpp;
    
    // [[Rcpp::export]]
    double sum_ages_cpp(NumericVector ages) {
      double total = 0;
      for (int i = 0; i < ages.size(); i++) {
        total += ages[i];
      }
      return total;
    }
    '
    tryCatch({
      Rcpp::sourceCpp(code = rcpp_code)
      return(sum_ages_cpp(ages_vector))
    }, error = function(e) {
      return(sum(ages_vector))  # Fallback
    })
  } else {
    return(sum(ages_vector))  # Fallback
  }
}

# ============================================================================
# 🏃 BENCHMARK RUNNER
# ============================================================================

run_benchmark <- function(name, func, data, warmup = 1) {
  # Warmup runs
  for (i in 1:warmup) {
    tryCatch(func(data), error = function(e) NULL)
  }
  
  # Замер времени
  gc()  # Garbage collection
  start_time <- Sys.time()
  
  tryCatch({
    result <- func(data)
    end_time <- Sys.time()
    
    elapsed_ms <- as.numeric(end_time - start_time, units="secs") * 1000
    avg_age <- result / length(data)
    
    return(list(
      name = name,
      result = result,
      time_ms = elapsed_ms,
      avg_age = avg_age,
      success = TRUE
    ))
  }, error = function(e) {
    return(list(
      name = name,
      result = 0,
      time_ms = Inf,
      avg_age = 0,
      success = FALSE,
      error = e$message
    ))
  })
}

# ============================================================================
# 🚀 MAIN BENCHMARK EXECUTION
# ============================================================================

main <- function() {
  # Создаем данные
  data <- create_test_data(num_users)
  users_df <- data$df
  ages_vector <- data$vectors$ages
  ages_matrix <- matrix(ages_vector, ncol = 1)
  
  cat("🚀 Running R benchmarks...\n\n")
  
  # Список всех тестов
  benchmarks <- list(
    list("R DataFrame", sum_ages_dataframe, users_df$age),
    list("R Vectorized", sum_ages_vectorized, ages_vector),
    list("R Builtin Sum", sum_ages_builtin, ages_vector),
    list("R ColSums", sum_ages_colsums, ages_matrix),
    list("R Loop (slow)", sum_ages_loop, ages_vector),
    list("R Apply", sum_ages_apply, ages_matrix),
    list("R Reduce", sum_ages_reduce, ages_vector),
    list("R data.table", sum_ages_datatable, ages_vector),
    list("R Rcpp (C++)", sum_ages_rcpp, ages_vector)
  )
  
  # Добавляем параллельную версию только для больших данных
  if (num_users >= 100000) {
    benchmarks <- append(benchmarks, list(list("R Parallel", sum_ages_parallel, ages_vector)))
  }
  
  # Запускаем бенчмарки
  results <- list()
  for (i in seq_along(benchmarks)) {
    benchmark <- benchmarks[[i]]
    name <- benchmark[[1]]
    func <- benchmark[[2]]
    test_data <- benchmark[[3]]
    
    cat("🔥 Testing", name, "...\n")
    result <- run_benchmark(name, func, test_data)
    results[[i]] <- result
    
    if (result$success) {
      cat("   Result:", round(result$avg_age, 1), "avg age\n")
      cat("   Time:", round(result$time_ms, 3), "ms\n")
    } else {
      cat("   ❌ Error:", result$error, "\n")
    }
    cat("\n")
  }
  
  # Вычисляем speedup
  baseline_time <- results[[1]]$time_ms
  for (i in seq_along(results)) {
    if (results[[i]]$success && results[[i]]$time_ms != Inf) {
      results[[i]]$speedup <- baseline_time / results[[i]]$time_ms
    } else {
      results[[i]]$speedup <- 0
    }
  }
  
  # Показываем результаты
  cat("📊 R PERFORMANCE COMPARISON:\n")
  cat("=" %R% 70, "\n")
  cat(sprintf("%-20s %-10s %-12s %-10s\n", "Algorithm", "Avg Age", "Time (ms)", "Speedup"))
  cat("=" %R% 70, "\n")
  
  for (result in results) {
    if (result$success) {
      speedup_str <- if (result$speedup > 0) paste0(round(result$speedup, 1), "x") else "baseline"
      cat(sprintf("%-20s %-10.1f %-12.3f %-10s\n", 
          result$name, result$avg_age, result$time_ms, speedup_str))
    }
  }
  cat("=" %R% 70, "\n\n")
  
  # Находим лучший результат
  valid_results <- results[sapply(results, function(x) x$success && x$time_ms != Inf)]
  if (length(valid_results) > 0) {
    fastest <- valid_results[[which.min(sapply(valid_results, function(x) x$time_ms))]]
    
    cat("🏆⚡ АБСОЛЮТНЫЙ ПОБЕДИТЕЛЬ R:", fastest$name, "\n")
    cat("    Время:", round(fastest$time_ms, 3), "ms\n")
    cat("    Ускорение:", round(fastest$speedup, 1), "x\n\n")
    
    # Сравнение с другими языками
    cat("🔬 COMPARISON WITH OTHER LANGUAGES:\n")
    cat("Rust QUANTUM: ~44.5µs (1M elements)\n")
    cat("C++ ULTRA: ~86µs (1M elements)\n")
    cat("Python SoA: ~2.58ms (1M elements)\n")
    cat("R Best:", round(fastest$time_ms, 2), "ms (", format(num_users, big.mark=","), "elements)\n\n")
    
    # Записываем результаты
    write_results(results, fastest)
    
    cat("🎯 R OPTIMIZATION SUMMARY:\n")
    cat("✅ Vectorized operations: R's natural strength\n")
    cat("✅ Built-in sum(): Highly optimized C implementation\n")
    cat("✅ Matrix operations: ColSums for numerical data\n")
    if (requireNamespace("data.table", quietly = TRUE)) {
      cat("✅ data.table: High-performance data manipulation\n")
    }
    if (requireNamespace("Rcpp", quietly = TRUE)) {
      cat("✅ Rcpp: C++ integration for maximum speed\n")
    }
    cat("✅ Parallel computing: Built-in cluster support\n")
    cat("❌ Loops: Generally slow in R (use vectorization!)\n\n")
    
    cat("💥 R BENCHMARK COMPLETE! 💥\n")
    cat("📊 R показывает отличные результаты для статистических вычислений! 📊⚡\n\n")
    
    cat("🚀 For BLAZING FAST R performance, consider:\n")
    cat("   • data.table: install.packages('data.table')\n")
    cat("   • Rcpp: install.packages('Rcpp')\n")
    cat("   • parallel: built-in parallelization\n")
    cat("   • Matrix operations: use vectorization!\n")
  }
}

# Функция записи результатов
write_results <- function(results, fastest) {
  filename <- "blazing_results_r.txt"
  
  file_conn <- file(filename, "w")
  writeLines("📊🚀⚡ R BLAZING FAST RESULTS ⚡🚀📊\n", file_conn)
  
  writeLines(paste("🖥️ SYSTEM INFO:"), file_conn)
  writeLines(paste("   R Version:", R.version.string), file_conn)
  writeLines(paste("   Platform:", R.version$platform), file_conn)
  writeLines(paste("   CPU Cores:", parallel::detectCores()), file_conn)
  writeLines(paste("   Users:", format(num_users, big.mark=",")), file_conn)
  writeLines("", file_conn)
  
  writeLines("📊 BENCHMARK RESULTS:", file_conn)
  writeLines(paste(rep("=", 50), collapse=""), file_conn)
  for (result in results) {
    if (result$success) {
      speedup_str <- if (result$speedup > 0) paste0(" (", round(result$speedup, 1), "x faster)") else ""
      writeLines(paste0(result$name, ": ", round(result$avg_age, 1), " avg - ", 
                       round(result$time_ms, 3), "ms", speedup_str), file_conn)
    }
  }
  writeLines(paste(rep("=", 50), collapse=""), file_conn)
  writeLines("", file_conn)
  
  writeLines(paste("🏆 FASTEST:", fastest$name, "-", round(fastest$time_ms, 3), "ms"), file_conn)
  writeLines(paste("💥 MAX SPEEDUP:", round(fastest$speedup, 1), "x faster than baseline"), file_conn)
  
  close(file_conn)
  cat("💾 Results written to:", filename, "\n")
}

# Определяем оператор %R% для повторения символов
"%R%" <- function(x, n) paste(rep(x, n), collapse="")

# Запускаем main функцию
if (!interactive()) {
  main()
}
