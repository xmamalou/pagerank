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

#include "utility.h"

#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <time.h>
#define itimerspec linux_itimerspec
#include <linux/time.h>
#undef itimerspec

// --- TYPES --- //

typedef struct Benchmark {
    struct timespec start; 
    struct timespec end;
} Benchmark;

typedef struct Matrix {
    uint32_t dims[2];
    uint8_t  degree;
    double*  matrix;
} Matrix;

// --- CONSTANTS --- //

const uint64_t nsecs_in_sec = 1000000000;

// --- FUNCTION DEFINITIONS --- //

BENCHMARK_T start_benchmark() 
{
    const Benchmark* benchmark_p = (Benchmark*)calloc(1, sizeof(Benchmark));
    clock_gettime(CLOCK_REALTIME, &benchmark_p->start);

    return (BENCHMARK_T)benchmark_p;
}

uint64_t stop_benchmark(BENCHMARK_T handle)
{
    const Benchmark* benchmark_p = (Benchmark*)handle;
    clock_gettime(CLOCK_REALTIME, &benchmark_p->end);

    const uint64_t time =
            benchmark_p->end.tv_sec*nsecs_in_sec + benchmark_p->end.tv_nsec
            - benchmark_p->start.tv_sec*nsecs_in_sec - benchmark_p->start.tv_nsec;
    free((void*)benchmark_p);

    return time;
}

MATRIX_T create_matrix(
    uint32_t dim_num,
    ...)
{
    assert(dim_num < 3 && dim_num > 0);

    uint32_t element_num;
    Matrix* matrix_p = (Matrix*)calloc(1, sizeof(Matrix));

    matrix_p->degree = dim_num;

    va_list args;
    va_start(args, dim_num);
    for (uint32_t i = 0; i < dim_num; i++)
    {
        uint32_t dim = (uint32_t)va_arg(args, uint32_t);
        matrix_p->dims[i] = dim; 
        element_num *= dim;
    }
    va_end(args);

    matrix_p->matrix = (double*)calloc(element_num, sizeof(double));

    return (MATRIX_T)matrix_p;
}

void set_element(
    MATRIX_T matrix_h,
    double   element,
    ...)
{
    Matrix* matrix_p = (Matrix*)matrix_h;
    va_list args;

    va_start(args, element);
    uint32_t x = (uint32_t)va_arg(args, uint32_t);
    uint32_t y = (uint32_t)va_arg(args, uint32_t);
    va_end(args);

    matrix_p->matrix[x + matrix_p->dims[1]*y] = element;
}

double get_element(
    MATRIX_T matrix_h,
    ...)
{
    Matrix* matrix_p = (Matrix*)matrix_h;
    va_list args;

    va_start(args, matrix_h);
    uint32_t x = (uint32_t)va_arg(args, uint32_t);
    uint32_t y = (uint32_t)va_arg(args, uint32_t);
    va_end(args);

    return matrix_p->matrix[x + matrix_p->dims[1]*y];
}

void mul_factor(
    double   factor,
    MATRIX_T matrix_h)
{
    Matrix* matrix_p = (Matrix*)matrix_h;

    for (uint32_t i = 0; i < matrix_p->dims[0]; i++)
    {
        for (uint32_t j = 0; j < matrix_p->dims[1]; j++)
        {
            set_element(matrix_h, factor, i, j);
        }
    }
}

MATRIX_T mul_matrix(
    MATRIX_T  left_h,
    MATRIX_T  right_h)
{
    Matrix* left_p = (Matrix*)left_h;
    Matrix* right_p = (Matrix*)right_h;

    if (left_p->dims[1] == right_p->dims[0])
    {
        return NULL;
    }

    MATRIX_T result_h = create_matrix(
            2,
            left_p->dims[0], right_p->dims[1]);

    for (uint32_t i = 0; i < left_p->dims[0]; i++)
    {
        for (uint32_t j = 0; j < right_p->dims[1]; j++)
        {
            double dot = 0;
            for (uint32_t k = 0; k < right_p->dims[0]; k++)
            {        
                dot += get_element(left_h, i, k)*get_element(right_h, k, j);
            }         
            set_element(result_h, dot, i, j);
        }
    }
}

void destroy_matrix(MATRIX_T matrix_h)
{
    free((void*)matrix_h);
}
