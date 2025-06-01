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
