/*
    Parallel Systems Extra Project -- Serial and Parallel implementation of PageRank
    Copyright (C) 2025 Christoforos-Marios Mamaloukas

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <stdint.h>

#define HANDLE_TYPE(name) name##Handle
#define HANDLE_BUILDER(name) struct HANDLE_TYPE(name)* HANDLE_TYPE(name)

// --- BENCHMARKING --- //

typedef HANDLE_BUILDER(Benchmark);
#define BENCHMARK_T HANDLE_TYPE(Benchmark)

/// @brief Start a benchmark 
/// @return Returns a handle to the used benchmark structure
inline BENCHMARK_T start_benchmark();

/// @brief Stop a benchmark. Frees the resources of the passed benchmark handle.
/// ALWAYS DISCARD THE HANDLE YOU PASSED IN THIS FUNCTION AFTERWARDS!
/// @param handle The benchmark to stop
/// @return the time that passed in nsec since `start_benchmark` was called
inline uint64_t stop_benchmark(BENCHMARK_T handle);

// --- ARRAYS & MATRICES --- //

typedef HANDLE_BUILDER(Matrix);

#endif /* __UTILITY_H__ */