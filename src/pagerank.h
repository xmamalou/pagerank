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

#ifndef __EXERCISES_H__
#define __EXERCISES_H__

#include <stdint.h>
#include <stdbool.h>

/// @brief Runs PageRank
/// @param flags the flags to parametrize the execution
/// @param flag_count the count of the flags
/// @return Error code
///
/// - 0 on success
///
/// - 1 on failure to find file
///
/// - 2 on failure to read dumping factor
///
/// - 3 on failure to read transition matrix
uint32_t pagerank(
        const char** flags, const uint32_t flag_count);

#endif /* __EXERCISES_H__ */ 