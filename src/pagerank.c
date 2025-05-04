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

#include "pagerank.h"

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>

#include "utility.h"

// --- ERROR CODES --- //

#define SUCCESS 0
#define FILE_NOT_FOUND_ERR 1
#define FILE_EMPTY_ERR 2

// --- TYPES --- //

typedef struct Options {
    bool     do_serial;
    char     path[PATH_MAX];    
    bool     is_path_local;
    double   dump_factor;
    MATRIX_T trans_matrix;
    uint32_t iterations;
} Options;

// --- CONSTANTS --- //

const char* exercise_type[] = {
    "serial",
};

const uint64_t nsec_to_sec_factor  = 1000000000;
const uint64_t nsec_to_msec_factor = 1000000;

const uint64_t max_size = 256;

// --- FUNCTION DECLARATIONS --- //

void read_flags_ex1(
        const char** flags, const uint32_t flag_count,
        Options* options_p);

/// @brief Parse lines into a valid PageRank transition matrix
/// @param lines the lines to parse
/// @param line_count the amount of lines to parse
/// @param dim the dimensions of the matrix (since it's square, `x_dim = y_dim`) 
/// @return `NULL` if the matrix is invalid (i.e. it's not square or the sum of column elements
/// is not zero) otherwise a pointer to the matrix
MATRIX_T parse_transmatrix(
        const FILE* data, const uint64_t line_count);

inline void pagerank_serial(const Options* const options_p);

// --- FUNCTION DEFINITIONS --- //

uint32_t pagerank(
        const char** flags, const uint32_t flag_count) 
{
    Options options = {
        .do_serial     = true,
        .is_path_local = false,
        .dump_factor   = 0,
        .trans_matrix  = NULL,
    };
    read_flags_ex1(
            flags, flag_count,
            &options);

    // open data file
    char full_path[PATH_MAX];
    if (options.is_path_local)
    {
        getcwd(full_path, PATH_MAX);
        strcat(full_path, "/");
    }
    strncat(full_path, options.path, 
            PATH_MAX - strlen(full_path));

    FILE* data = fopen(
            full_path,
            "r");
    if (data == NULL)
    {
        fprintf(stderr,
                "\x1b[31mERROR! File couldn't be found! Does it exist?\n\x1b[0m");
        return FILE_NOT_FOUND_ERR;
    }

    // parse data file
    if (fgets(
            full_path, 
            max_size, 
            data) != NULL)
    {
        options.dump_factor = atof(full_path);
    } else {
        fprintf(stderr,
                "\x1b[31mERROR! File is empty!\n\x1b[0m");
        return FILE_EMPTY_ERR;
    }
    uint64_t line_count = 0;
    // we do a first run-through so we can be sure of how much space we need
    // to allocate for the matrix. Remember that the matrix is square
    while (fgets(
            full_path, 
            max_size, 
            data) != NULL)
    {
        line_count++;
    }
    // we then reset the pointer to the first matrix line
    fseek(
            data,
            1, SEEK_SET);
    options.trans_matrix = parse_transmatrix(
            (const FILE*)data, line_count); 

    printf("\x1b[33mParallel Systems Postgrad Course -- Extra Project -- Christoforos-Marios Mamaloukas\n\n"
        "----           PAGERANK          ----\n"
        "* You selected %s execution\n"
        "* Path to read PageRank data from: %s\n"
        "* Dumping factor: d = %.5f\n"
        "* Matrix dimensions: %lux%lu\n"
        "-------------------------------------\n\n\x1b[0m", 
        exercise_type[0],
        options.path,
        options.dump_factor,
        line_count, line_count);

    for(uint32_t i = 0; i < line_count; i++)
    {
        for(uint32_t j = 0; j < line_count; j++)
        {
            printf("%f; ", get_element(options.trans_matrix, i, j));
        }
        printf("\n");
    }

    printf("\n");

    if (options.do_serial)
    {
        pagerank_serial((const Options* const)&options);
    } 

    destroy_matrix(options.trans_matrix);
    fclose(data);

    return SUCCESS;
}

void read_flags_ex1(
    const char** flags, const uint32_t flag_count,
    Options* options_p)
{
    for (uint32_t i = 0; i < flag_count; i++)
    {
        if (strcmp(flags[i], "-fs") == 0 || strcmp(flags[i], "-fserial") == 0)
        {
            // if (options_p->do_omp)
            // {
            //     printf("\x1b[31mHey! You requested serial execution, even though"
            //         " you already want parallel! IGNORING!\n\x1b[0m");
            //     continue;
            // }

            options_p->do_serial = true;
            continue;
        }

        if (strstr(flags[i], "-fread=") != NULL)
        {
            const char* equal_char_p = strchr(flags[i], '=');
            strncpy(options_p->path, &(equal_char_p[1]), PATH_MAX);

            options_p->is_path_local = (options_p->path[0] == '~' || options_p->path[0] == '/')
                    ? false 
                    : true;

            continue;
        }

        // if (strstr(flags[i], "-fjobs=") != NULL || strstr(flags[i], "-fj=") != NULL)
        // {
        //     if(options_p->do_serial)
        //     {
        //         printf("\x1b[31mHey! You requested a custom amount of jobs, even though"
        //                 " you want serial execution! IGNORING!\n\x1b[0m");
        //         continue;
        //     }

        //     char* equal_char_p = strchr(flags[i], '=');
        //     options_p->job_count = atoi(&(equal_char_p[1])); // same as (equal_char_p + sizeof(char)), allows us to get the number next to the `=` sign
        // }

        if (strstr(flags[i], "-ftries=") != NULL || strstr(flags[i], "-fn=") != NULL)
        {
            char* equal_char_p    = strchr(flags[i], '=');
            options_p->iterations = atoi(&(equal_char_p[1])); // same as (equal_char_p + sizeof(char)), allows us to get the number next to the `=` sign
        }
    }
}

MATRIX_T parse_transmatrix(
    const FILE* data, const uint64_t line_count) 
{
    MATRIX_T matrix = create_matrix(line_count, line_count);

    uint32_t curr_column  = 0;
    char*    curr_line = NULL;
    size_t   len       = 0;
    getline(&curr_line, &len, data); // Getting rid of the first line
    while (getline(&curr_line, &len, data) != -1)
    {
        uint32_t curr_row = 0;
        char*    substr      = strtok(curr_line, "; ");
        while (substr != NULL)
        {
            set_element(
                matrix,
                atof(substr),
                curr_column, curr_row);
            curr_row++;
            substr = strtok(0, "; ");
        }
        curr_column++;
    }
    return matrix;
}

void pagerank_serial(const Options* const options_p) 
{
    uint32_t x; 
    get_dimensions(options_p->trans_matrix, &x, &x); // Matrix is square

    MATRIX_T multiplied_trans_matrix = mul_factor(
            options_p->dump_factor,
            options_p->trans_matrix);
    MATRIX_T added_vector       = create_matrix(x, 1);
    MATRIX_T initial_vector     = create_matrix(x, 1);
    for (uint32_t i = 0; i < x; i++)
    {
        set_element(
                added_vector, 
                1.0 - options_p->dump_factor, 
                i, 0);
        set_element(
                initial_vector, 
                0.0, 
                i, 0);
    }

    for (uint32_t i = 0; i < options_p->iterations; i++)
    {
        // new handle
        /*
        * J_1 = d*W*x
        */
        MATRIX_T intrm_matrix_h = mul_matrix(
                multiplied_trans_matrix,
                initial_vector); 

        // not a new handle
        /*
        * J_2 = d*W*x + (1-d)*I
        */
        add_matrix(
                intrm_matrix_h,
                added_vector); // we ignore the result, because it's just intrm_matrix_h

        copy_matrix(initial_vector, intrm_matrix_h);

        // the firstfunction that returned intrm_matrix_h 
        // created a new handle, so we need to destroy it
        destroy_matrix(intrm_matrix_h);
    }

    for(uint32_t i = 0; i < x; i++)
    {
        printf("%f; ", get_element(initial_vector, i, 0));
        printf("\n");
    }

    destroy_matrix(initial_vector);
    destroy_matrix(added_vector);

    return;
}