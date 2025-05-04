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

#include <omp.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "pagerank.h"

// --- TYPES --- //

typedef enum Section {
    HELP       = 0,
    OTHER      = 1,
} Section;

/// @brief Options for the project
typedef struct Options {
    Section  which;
    char**   flags;
    uint32_t flag_count;
} Options;

// --- FUNCTIONS --- //

/// @brief Reads arguments from the command line to parse them into actions
/// @param argc command line arguments
/// @param argv command line argument count
/// @param exercise_options structure to be modified depending on command line arguments
void read_arguments(
        const int argc, const char* argv[],
        Options* exercise_options);

/// @brief Parses a string of `-f` flags to pass them to an exercise. Flags must be sequential and not be broken
/// by another non-flag argument.  
/// @param argv_slice 
/// @param argv_slice_length 
/// @return The amount of arguments that refer to flags
uint32_t count_flags(
        const char** argv_slice, const uint32_t argv_slice_length);

// --- FUNC DEFINITIONS --- //

int main(
        int argc, char* argv[]) 
{
    Options exercise_options = {
        .which      = OTHER,
        .flags      = NULL,
        .flag_count = 0
    };

    if(argc > 1) 
    {
        read_arguments(
                argc, (const char**)argv,
                &exercise_options);
    }

    // Help just prints a message and quits; program does not proceed.
    if (exercise_options.which == HELP) 
    {
        printf("\x1b[33mParallel Systems Postgrad Course -- Extra Project -- Christoforos-Marios Mamaloukas\n"
                "Available options:\n"
                "\t* -h, --help: Shows the current message\n"
                "Available flags:\n"
                "\t* -fread=<path>: Path with data for the algorithm to process. Data must"
                " be laid in\n\tthe following way: One line containing the \x1b[1mdumping factor\x1b[m\x1b[33m (between 0 and 1)"
                " and the\n\trest containing the \x1b[1mtransition matrix\x1b[m\x1b[33m, where every row's elements are delimited with\n\t`;`"
                " and every row is on a new line\n"
                "\t* -ftries=<number>, -fn=<number>: Number of iterations for the algorithm\n"
                "\x1b[0m");
        return 0;
    }

    uint32_t err = pagerank(
            (const char**)exercise_options.flags, exercise_options.flag_count);

    return err;
}

//

void read_arguments(
        const int argc, const char* argv[],
        Options* exercise_options) 
{
    for (uint32_t i = 1; i < argc; i++)
    {
        // Help causes the program to ignore all other arguments
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) 
        {
            exercise_options->which = HELP;
            return;
        }

        exercise_options->flag_count = count_flags(
                &(argv[i]), argc - i);
        exercise_options->flags = (char**)&(argv[i]);
        i += exercise_options->flag_count + 1;
        
        // If any of the above checks fails, this means an invalid parameter was passed.
        // Quit and show help message 
        if (exercise_options->flag_count == 0) 
        {
            fprintf(stderr, "\x1b[31mERROR! Argument %s is unknown!\n", argv[i]);
            exercise_options->which = HELP;
            break; 
        }
    }
}

uint32_t count_flags(
        const char** argv_slice, const uint32_t argv_slice_length)
{
    uint32_t count = 0;
    for (uint32_t i = 0; i < argv_slice_length; i++)
    {
        if (strstr(argv_slice[i], "-f") != NULL)
        {
            count++;
        } else {
             // if the array of flags is broken by something that does not start with `-f`, we consider that there are no
             // more flags for the exercise
            break;
        }
    }

    return count;
}
