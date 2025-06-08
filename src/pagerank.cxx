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
#include <random>

#include "types.hxx"

using Error     = utility::Error;
using ErrorCode = utility::Error::ErrorCode;
using Matrix    = utility::Matrix;

// --- FUNCTION DECLARATIONS --- //

static auto pagerank_serial(
  Matrix& trans_matrix,
  const utility::Options&) -> void;

static auto pagerank_parallel(
  Matrix& trans_matrix,
  const utility::Options&) -> void;

// --- FUNCTION DEFINITIONS --- //

auto exe::pagerank(const utility::Options& options) -> void {
  if (options.dump_fac >= 1 || options.dump_fac <= 0) throw Error{ "", ErrorCode::BAD_DUMPING_FAC_ERR };

  std::cout << R"(------ PAGERANK ------
-> Iterations to run the algorithm for: )" << options.iterations << R"(
-> Dimensions of the transition matrix: )" << options.dims[0] << "x" << options.dims[1] << R"(
-> Dumping factor: )" << options.dump_fac << R"(
)" << (options.do_serial
    ? R"(-> Running the serial implementation)"
    : R"(-> Threads to run the algorithm on )") << options.jobs << R"(
----------------------
)";

  Matrix matrix(options.dims[0], options.dims[1]);

  std::random_device random;
  std::mt19937 generator(random());
  std::uniform_real_distribution<double> dist(0, 1);

  for (uint32_t i{ 0 }; i < matrix.get_dimension<0>(); i++) {
    for (uint32_t j{ 0 }; j < matrix.get_dimension<1>(); j++) {
      matrix(i, j) = dist(generator);
    }
  }

  (options.do_serial
   ? pagerank_serial
   : pagerank_parallel)(matrix, options);
}

static auto pagerank_serial(
  Matrix& trans_matrix,
  const utility::Options& options) -> void {
  auto& dump_fac{ options.dump_fac };
  auto& iterations{ options.iterations };

  auto initial_vec{ Matrix(
    trans_matrix.get_dimension<1>(),
    1) };

  auto added_vec{ Matrix(
    trans_matrix.get_dimension<1>(),
    1) };

  for (uint32_t i{0}; i < added_vec.get_dimension<0>(); i++) {
    added_vec(i, 0) = 1.0 - dump_fac;
  }

  for (uint32_t i{0}; i < initial_vec.get_dimension<0>(); i++) {
    initial_vec(i, 0) = 0.0;
  }

  try {
    for (uint32_t i{0}; i <= iterations; i++) {
      auto&& interm_matrix = trans_matrix*initial_vec;

      interm_matrix += added_vec;
      initial_vec = interm_matrix;
    }

    for (uint32_t i{0}; i < initial_vec.get_dimension<0>(); i++) {
        std::cout << "x[" << i << "] = " << initial_vec(i, 0) << "\n";
    }
  } catch (Error err) {
    switch (err.error) {
      case ErrorCode::OUT_OF_BOUNDS_ERR:
        std::cerr << "\x1b[31mERROR!! Went out of bounds! (FILE: " << err.erroneous << ")\x1b[0m\n";
        break;
      case ErrorCode::WRONG_DIMS_ERR:
        std::cerr << "\x1b[31mERROR!! Matrices have wrong dimensions! (FILE: " << err.erroneous << ")\x1b[0m\n";
        break;
      default:
        break;
    }

    throw;
  }
}

static auto pagerank_parallel(
  Matrix& trans_matrix,
  const utility::Options& options) -> void {

  auto initial_vec{ Matrix(
    trans_matrix.get_dimension<1>(),
    1) };

  auto added_vec{ Matrix(
    trans_matrix.get_dimension<1>(),
    1) };

  auto iterations{ options.iterations };
  auto& dump_fac{ options.dump_fac };

  #pragma omp parallel num_threads(options.jobs)
  {
    #pragma omp for
    for (uint32_t i = 0; i < initial_vec.get_dimension<0>(); i++) {
      initial_vec(i, 0) = 0.0;
      added_vec(i, 0) = 1.0 - dump_fac;
    }

    while (iterations > 0) {
      double sum{ 0.0 };

      #pragma omp for
      for (uint32_t i = 0; i < trans_matrix.get_dimension<0>(); i++) {
        for (uint32_t j{ 0 }; j < trans_matrix.get_dimension<1>(); j++) {
          sum += trans_matrix(i, j)*initial_vec(j, 0);
        }
        #pragma omp critical
        initial_vec(i, 0) = sum + added_vec(i, 0);

        sum = 0.0;
      }

      #pragma omp barrier

      #pragma omp single
      iterations--;
    }
  }

  for (uint32_t i{0}; i < initial_vec.get_dimension<0>(); i++) {
        std::cout << "x[" << i << "] = " << initial_vec(i, 0) << "\n";
  }
}
