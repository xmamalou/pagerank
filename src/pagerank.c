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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>

#include "utility.h"

// -- TYPES --- //

typedef struct PROptions {
    bool     do_serial;
    char     path[PATH_MAX];    
    bool     is_path_local; 
} PROptions;

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
        PROptions* options_p);

/// @brief Parse lines into a valid PageRank transition matrix
/// @param lines the lines to parse
/// @param line_count the amount of lines to parse
/// @param dim the dimensions of the matrix (since it's square, `x_dim = y_dim`) 
/// @return `NULL` if the matrix is invalid (i.e. it's not square or the sum of column elements
/// is not zero) otherwise a pointer to the matrix
double** parse_transmatrix(
        const FILE* data, const uint64_t line_count);

inline void pagerank_serial(const PROptions* const options_p);

// --- FUNCTION DEFINITIONS --- //

uint32_t pagerank(
        const char** flags, const uint32_t flag_count) 
{
    PROptions options = {
        .do_serial     = true,
        .is_path_local = false,
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
        printf("\x1b[31mERROR! File couldn't be found! Does it exist?\n\x1b[0m");
        return 1;
    }

    // parse data file
    double dump_factor = 0;
    if (fgets(
            full_path, 
            max_size, 
            data) != NULL)
    {
        dump_factor = atof(full_path);
    } else {
        printf("\x1b[31mERROR! File is empty!\n\x1b[0m");
        return 2;
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
    double** matrix = parse_transmatrix(
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
        dump_factor,
        line_count, line_count);

    if (options.do_serial)
    {
        pagerank_serial((const PROptions* const)&options);
    } 

    fclose(data);

    return 0;
}

void read_flags_ex1(
    const char** flags, const uint32_t flag_count,
    PROptions* options_p)
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

        // if (strstr(flags[i], "-fthrows=") != NULL || strstr(flags[i], "-fn=") != NULL)
        // {
        //     char* equal_char_p     = strchr(flags[i], '=');
        //     options_p->throw_count = atoll(&(equal_char_p[1])); // same as (equal_char_p + sizeof(char)), allows us to get the number next to the `=` sign
        // }
    }
}

double** parse_transmatrix(
    const FILE* data, const uint64_t line_count) 
{

}

void pagerank_serial(const PROptions* const options_p) 
{
    

    

    return;
}