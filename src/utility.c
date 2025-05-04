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
        uint32_t  x, 
        uint32_t  y)
{
    uint32_t element_num;
    Matrix* matrix_p = (Matrix*)calloc(1, sizeof(Matrix));

    matrix_p->dims[0] = x;
    matrix_p->dims[1] = y;

    matrix_p->matrix = (double*)calloc(element_num, sizeof(double));

    return (MATRIX_T)matrix_p;
}

void get_dimensions(
        MATRIX_T  matrix_h,
        uint32_t* x, 
        uint32_t* y)
{
    Matrix* matrix_p = (Matrix*)matrix_h;
    *x = matrix_p->dims[0];
    *y = matrix_p->dims[1];
}

void set_element(
        MATRIX_T matrix_h,
        double   element,
        uint32_t x, 
        uint32_t y)
{
    Matrix* matrix_p = (Matrix*)matrix_h;

    matrix_p->matrix[x + matrix_p->dims[1]*y] = element;
}

double get_element(
        MATRIX_T matrix_h,
        uint32_t x, 
        uint32_t y)
{
    Matrix* matrix_p = (Matrix*)matrix_h;

    return matrix_p->matrix[x + matrix_p->dims[1]*y];
}

void copy_matrix(
    MATRIX_T dest_h,
    MATRIX_T source_h)
{
    uint32_t x1, x2, y1, y2;
    get_dimensions(
            dest_h,
            &x1, &y1);
    get_dimensions(
            source_h,
            &x2, &y2);    
            
    if (x1 != x2 && y1 != y2)
    {
        return;
    }

    for (uint32_t i = 0; i < x1; i++)
    {
        for (uint32_t j = 0; j < y1; j++)
        {
            set_element(
                    dest_h, 
                    get_element(source_h,
                        i, j), 
                    i, j);
        }
    }
}

MATRIX_T add_matrix(
    MATRIX_T left_h,
    MATRIX_T right_h)
{
    uint32_t x1, x2, y1, y2;
    get_dimensions(
            left_h,
            &x1, &y1);
    get_dimensions(
            right_h,
            &x2, &y2);    
            
    if (x1 != x2 && y1 != y2)
    {
        return NULL;
    }

    for (uint32_t i = 0; i < x1; i++)
    {
        for (uint32_t j = 0; j < y1; j++)
        {
            set_element(
                    left_h, 
                    get_element(left_h, 
                            i, j) + get_element(right_h,
                            i, j), 
                    i, j);
        }
    }

    return left_h;
}

MATRIX_T mul_factor(
    double   factor,
    MATRIX_T matrix_h)
{
    uint32_t x, y;
    get_dimensions(
            matrix_h,
            &x, &y);

    for (uint32_t i = 0; i < x; i++)
    {
        for (uint32_t j = 0; j < y; j++)
        {
            set_element(
                    matrix_h, 
                    factor*get_element(matrix_h,
                            i, j), 
                    i, j);
        }
    }

    return matrix_h;
}

MATRIX_T mul_matrix(
    MATRIX_T  left_h,
    MATRIX_T  right_h)
{
    uint32_t x1, x2, y1, y2;
    get_dimensions(
            left_h,
            &x1, &y1);
    get_dimensions(
            right_h,
            &x2, &y2); 

    if (y1 != x2)
    {
        return NULL;
    }

    MATRIX_T result_h = create_matrix(
            x1, y2);

    for (uint32_t i = 0; i < x1; i++)
    {
        for (uint32_t j = 0; j < y2; j++)
        {
            double dot = 0.0;
            for (uint32_t k = 0; k < x2; k++)
            {        
                dot += get_element(
                        left_h, 
                        i, k)*get_element(
                                right_h, 
                                k, j);
            }         
            set_element(
                    result_h, 
                    dot, 
                    i, j);
        }
    }

    return (MATRIX_T)result_h;
}

void destroy_matrix(MATRIX_T matrix_h)
{
    free((void*)matrix_h);
}
