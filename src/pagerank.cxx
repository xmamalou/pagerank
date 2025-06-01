/*
    Parallel Systems Extracurricular Project -- Pagerank implementation in the context of the Parallel
    Systems Course of the "Computer Engineering" Masters Programme of NKUA
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

#include "pagerank.hxx"

#include <iostream>

#include "types.hxx"

using Error     = utility::Error;
using ErrorCode = utility::Error::ErrorCode;

auto exe::pagerank(const utility::Options& options) -> void {
  if (options.dump_fac >= 1 || options.dump_fac <= 0) throw Error{ "", ErrorCode::BAD_DUMPING_FAC_ERR };

  std::cout << R"(------ PAGERANK ------
-> Iterations to run the algorithm for: )" << options.iterations << R"(
-> Dimensions of the transition matrix: )" << options.dims[0] << "x" << options.dims[1] << R"(
-> Dumping factor: )" << options.dump_fac << R"(
)" << (options.do_serial
    ? R"(-> Running the serial implementation)"
    : R"(-> Threads to run the algorithm on )") << options.jobs << R"(
----------------------)";
}
