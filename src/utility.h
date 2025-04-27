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

// --- ERROR CODES --- //

#define SUCCESS 0
#define OUT_OF_BOUNDS_ERR 1
#define ILLEGAL_DIM_ERR 2

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

/// @brief A matrice type whose elements are real numbers (doubles)
typedef HANDLE_BUILDER(Matrix);
#define MATRIX_T HANDLE_TYPE(Matrix)

/// @brief Create a matrix
/// @param dim_num the degree of the matrix
/// @param x... the dimensions of the matrix
/// @return a handle to the created matrix, `NULL` otherwise
MATRIX_T create_matrix(
        uint32_t dim_num,
        ...);

/// @brief Set an element on a matrix
/// @param matrix_h the matrix to traverse
/// @param element the variable to write from
/// @param x the column coordinate of the element
/// @param y... the other coordinates of the element
inline void set_element(
    MATRIX_T matrix_h,
    double   element,
    ...);

/// @brief Get an element from a matrix
/// @param matrix_h the matrix to traverse
/// @param x the column coordinate of the element
/// @param y... the other coordinates of the element
inline double get_element(
        MATRIX_T matrix_h,
        ...);

/// @brief Multiply a matrix by a factor
/// @param factor the factor to multiply by
/// @param matrix_h the matrix to multiply with
inline void mul_factor(
        double   factor,
        MATRIX_T matrix_h);

/// @brief Multiply two matrices together
/// @param left_h the left matrix
/// @param right_h the right matrix
/// @return a matrix handle if the operation is legal. `NULL` otherwise
MATRIX_T mul_matrix(
        MATRIX_T left_h,
        MATRIX_T right_h);

/// @brief Destroy a matrix and free its resources
/// @param matrix_h the matrix to destroy
inline void destroy_matrix(MATRIX_T matrix_h);

#endif /* __UTILITY_H__ */