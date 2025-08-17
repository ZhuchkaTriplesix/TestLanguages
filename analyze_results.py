#!/usr/bin/env python3
"""
🚀⚡ BLAZING FAST BENCHMARK RESULTS ANALYZER ⚡🚀

Analyzes benchmark results from all languages and generates comprehensive reports.
"""

import re
import json
import sys
from pathlib import Path
from typing import Dict, List, Tuple, Optional
from datetime import datetime

class BenchmarkAnalyzer:
    def __init__(self):
        self.results = {}
        self.languages = ['Assembly', 'C', 'C++', 'Rust', 'Go', 'Python', 'Dart', 'R']
        self.sizes = [1000, 10000, 100000, 1000000, 10000000]
        self.size_names = ['1K', '10K', '100K', '1M', '10M']
    
    def parse_results_file(self, filename: str) -> Dict:
        """Parse the benchmark results file."""
        results = {}
        current_lang = None
        current_size = None
        
        try:
            with open(filename, 'r', encoding='utf-8') as f:
                content = f.read()
            
            # Split by language sections
            sections = re.split(r'=== (\w+) (\d+) ELEMENTS ===', content)
            
            for i in range(1, len(sections), 3):
                if i + 2 < len(sections):
                    lang = sections[i].strip()
                    size = int(sections[i+1])
                    output = sections[i+2].strip()
                    
                    if lang not in results:
                        results[lang] = {}
                    
                    # Extract timing information
                    times = self.extract_times(output)
                    results[lang][size] = times
            
            return results
        except Exception as e:
            print(f"Error parsing results: {e}")
            return {}
    
    def extract_times(self, output: str) -> Dict:
        """Extract timing information from benchmark output."""
        times = {}
        
        # Look for specific patterns
        lines = output.split('\n')
        
        for line in lines:
            line = line.strip()
            
            # Assembly patterns
            if 'CHAMPION:' in line and 'with' in line and 'ms' in line:
                match = re.search(r'CHAMPION:\s*(.+?)\s+with\s+(\d+\.?\d*)\s*ms', line)
                if match:
                    name, time_str = match.groups()
                    try:
                        times['best'] = float(time_str)
                        times[name.strip()] = float(time_str)
                    except ValueError:
                        pass
            
            # C patterns - look for best time
            elif 'Best time:' in line:
                match = re.search(r'Best time:\s*(\d+\.?\d*)\s*ms', line)
                if match:
                    try:
                        times['best'] = float(match.group(1))
                    except ValueError:
                        pass
            
            # Rust patterns
            elif 'fastest:' in line.lower() and 'ms' in line:
                match = re.search(r'(\d+\.?\d*)\s*ms', line)
                if match:
                    try:
                        times['best'] = float(match.group(1))
                    except ValueError:
                        pass
            
            # Go patterns
            elif 'Fastest:' in line and 'ms' in line:
                match = re.search(r'(\d+\.?\d*)\s*ms', line)
                if match:
                    try:
                        times['best'] = float(match.group(1))
                    except ValueError:
                        pass
            
            # General patterns - Result: ... in X.XXX ms
            elif 'Result:' in line and 'in' in line and 'ms' in line:
                match = re.search(r'Result:.*?in\s+(\d+\.?\d*)\s*ms', line)
                if match:
                    try:
                        time_val = float(match.group(1))
                        if 'best' not in times or time_val < times['best']:
                            times['best'] = time_val
                    except ValueError:
                        pass
            
            # Individual technique timings
            elif re.match(r'.*?(\d+\.?\d*)\s*ms', line) and ('Testing' in line or ':' in line):
                match = re.search(r'(.+?).*?(\d+\.?\d*)\s*ms', line)
                if match:
                    name, time_str = match.groups()
                    try:
                        time_val = float(time_str)
                        clean_name = re.sub(r'[🔥⚡✅🎯💥🏆]', '', name).strip()
                        clean_name = clean_name.replace('Testing', '').strip()
                        if clean_name:
                            times[clean_name] = time_val
                            if 'best' not in times or time_val < times['best']:
                                times['best'] = time_val
                    except ValueError:
                        pass
        
        # If no times found, try simpler pattern
        if not times:
            simple_matches = re.findall(r'(\d+\.?\d*)\s*ms', output)
            if simple_matches:
                try:
                    times['best'] = min(float(t) for t in simple_matches if float(t) > 0)
                except ValueError:
                    pass
        
        return times
    
    def generate_summary_table(self, results: Dict) -> str:
        """Generate a summary table of all results."""
        table = []
        table.append("🏆 PERFORMANCE SUMMARY TABLE\n")
        table.append("=" * 80)
        
        # Header
        header = "Size".ljust(8)
        for lang in self.languages:
            if any(lang.upper() in results or lang in results for _ in [0]):
                header += f"{lang}".ljust(12)
        table.append(header)
        table.append("-" * len(header))
        
        # Data rows
        for i, size in enumerate(self.sizes):
            row = f"{self.size_names[i]}".ljust(8)
            
            for lang in self.languages:
                lang_key = None
                for key in results.keys():
                    if key.upper() == lang.upper() or key == lang:
                        lang_key = key
                        break
                
                if lang_key and size in results[lang_key]:
                    times = results[lang_key][size]
                    if times:
                        best_time = min(times.values()) if times else 0
                        if best_time > 0:
                            if best_time < 1:
                                row += f"{best_time:.3f}ms".ljust(12)
                            else:
                                row += f"{best_time:.1f}ms".ljust(12)
                        else:
                            row += "N/A".ljust(12)
                    else:
                        row += "N/A".ljust(12)
                else:
                    row += "N/A".ljust(12)
            
            table.append(row)
        
        return "\n".join(table)
    
    def find_champions(self, results: Dict) -> Dict:
        """Find the fastest implementation for each size."""
        champions = {}
        
        for size in self.sizes:
            best_time = float('inf')
            best_lang = None
            best_technique = None
            
            for lang, lang_results in results.items():
                if size in lang_results and lang_results[size]:
                    times = lang_results[size]
                    min_time = min(times.values())
                    min_technique = min(times.items(), key=lambda x: x[1])
                    
                    if min_time < best_time:
                        best_time = min_time
                        best_lang = lang
                        best_technique = min_technique[0]
            
            champions[size] = {
                'language': best_lang,
                'technique': best_technique,
                'time': best_time
            }
        
        return champions
    
    def calculate_speedups(self, results: Dict) -> Dict:
        """Calculate speedups relative to the fastest implementation."""
        speedups = {}
        champions = self.find_champions(results)
        
        for size in self.sizes:
            if size in champions and champions[size]['time'] < float('inf') and champions[size]['time'] > 0:
                fastest_time = champions[size]['time']
                speedups[size] = {}
                
                for lang, lang_results in results.items():
                    if size in lang_results and lang_results[size]:
                        times = lang_results[size]
                        min_time = min(times.values())
                        if min_time > 0 and fastest_time > 0:
                            speedup = min_time / fastest_time
                            speedups[size][lang] = speedup
        
        return speedups
    
    def generate_analysis_report(self, results: Dict) -> str:
        """Generate comprehensive analysis report."""
        report = []
        report.append("🚀⚡ COMPREHENSIVE PERFORMANCE ANALYSIS ⚡🚀\n")
        report.append("=" * 60)
        
        # Summary table
        report.append(self.generate_summary_table(results))
        report.append("")
        
        # Champions analysis
        champions = self.find_champions(results)
        report.append("🏆 PERFORMANCE CHAMPIONS:")
        report.append("")
        
        for size in self.sizes:
            size_name = self.size_names[self.sizes.index(size)]
            if size in champions and champions[size]['language']:
                champ = champions[size]
                report.append(f"{size_name}: {champ['language']} ({champ['technique']}) - {champ['time']:.3f}ms")
            else:
                report.append(f"{size_name}: No data")
        
        report.append("")
        
        # Speedups
        speedups = self.calculate_speedups(results)
        report.append("⚡ SPEEDUP ANALYSIS (relative to fastest):")
        report.append("")
        
        for size in self.sizes:
            size_name = self.size_names[self.sizes.index(size)]
            report.append(f"{size_name}:")
            
            if size in speedups:
                sorted_speedups = sorted(speedups[size].items(), key=lambda x: x[1])
                for lang, speedup in sorted_speedups:
                    if speedup == 1.0:
                        report.append(f"  🥇 {lang}: {speedup:.2f}x (CHAMPION)")
                    elif speedup <= 2.0:
                        report.append(f"  🥈 {lang}: {speedup:.2f}x")
                    elif speedup <= 5.0:
                        report.append(f"  🥉 {lang}: {speedup:.2f}x")
                    else:
                        report.append(f"  🔸 {lang}: {speedup:.2f}x")
            report.append("")
        
        # Scaling analysis
        report.append("📈 SCALING ANALYSIS:")
        report.append("")
        
        for lang in results.keys():
            report.append(f"{lang}:")
            lang_results = results[lang]
            
            # Check 1K -> 10K scaling
            if 1000 in lang_results and 10000 in lang_results:
                time_1k = min(lang_results[1000].values()) if lang_results[1000] else 0
                time_10k = min(lang_results[10000].values()) if lang_results[10000] else 0
                if time_1k > 0 and time_10k > 0:
                    scaling = time_10k / time_1k
                    report.append(f"  1K→10K: {scaling:.2f}x scaling")
            
            # Check 100K -> 1M scaling
            if 100000 in lang_results and 1000000 in lang_results:
                time_100k = min(lang_results[100000].values()) if lang_results[100000] else 0
                time_1m = min(lang_results[1000000].values()) if lang_results[1000000] else 0
                if time_100k > 0 and time_1m > 0:
                    scaling = time_1m / time_100k
                    report.append(f"  100K→1M: {scaling:.2f}x scaling")
            
            # Check 1M -> 10M scaling
            if 1000000 in lang_results and 10000000 in lang_results:
                time_1m = min(lang_results[1000000].values()) if lang_results[1000000] else 0
                time_10m = min(lang_results[10000000].values()) if lang_results[10000000] else 0
                if time_1m > 0 and time_10m > 0:
                    scaling = time_10m / time_1m
                    report.append(f"  1M→10M: {scaling:.2f}x scaling")
            report.append("")
        
        return "\n".join(report)
    
    def save_json_results(self, results: Dict, filename: str):
        """Save results in JSON format for further analysis."""
        json_data = {
            'timestamp': datetime.now().isoformat(),
            'results': results,
            'metadata': {
                'languages': self.languages,
                'sizes': self.sizes,
                'size_names': self.size_names
            }
        }
        
        with open(filename, 'w') as f:
            json.dump(json_data, f, indent=2)

def main():
    analyzer = BenchmarkAnalyzer()
    
    # Check if results file exists
    results_file = "results/all_results.txt"
    if not Path(results_file).exists():
        print(f"❌ Results file not found: {results_file}")
        print("Please run 'run_all_benchmarks.bat' first.")
        return
    
    print("🔍 Analyzing benchmark results...")
    
    # Parse results
    results = analyzer.parse_results_file(results_file)
    
    if not results:
        print("❌ No valid results found in the file.")
        return
    
    print(f"✅ Found results for {len(results)} languages")
    
    # Generate analysis
    analysis = analyzer.generate_analysis_report(results)
    
    # Save analysis
    analysis_file = "results/performance_analysis.txt"
    with open(analysis_file, 'w', encoding='utf-8') as f:
        f.write(analysis)
    
    # Save JSON data
    json_file = "results/benchmark_data.json"
    analyzer.save_json_results(results, json_file)
    
    print(f"📊 Analysis saved to: {analysis_file}")
    print(f"📋 JSON data saved to: {json_file}")
    print("")
    print("🏆 QUICK SUMMARY:")
    print("")
    print(analyzer.generate_summary_table(results))

if __name__ == "__main__":
    main()
