/*
    Parallel Systems Exercise Batch 2 -- Solutions to Batch 2 of Exercises for the Parallel
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

#include <iostream>
#include <string_view>
#include <vector>
#include <utility>
#include <charconv>
#include <algorithm>
#include <cstdio>

#include <stdint.h>

#include "pagerank.hxx"
#include "types.hxx"

// --- TYPES --- //
enum class ArgType {
  OPTION,
  FLAG,
};

struct Argument {
  std::string_view long_name{ "" };
  std::string_view short_name{ "" };
  char             id{ '\0' };
  ArgType          type{ ArgType::FLAG };
  std::string_view format{ "" };
};

using Error = utility::Error;
using ErrorCode = utility::Error::ErrorCode;

using CMDLineParam  = std::string_view;
using CMDLineParams = std::vector<CMDLineParam>;
using Arguments     = std::vector<Argument>;

// --- CONSTANTS --- //
const Arguments options_gc{
  {"--iter", "-i", 'i', ArgType::OPTION, "%u"},
  {"--dims", "-d", 'd', ArgType::OPTION, "%ux%u"},
  {"--jobs", "-j", 'j', ArgType::OPTION, "%u"},
  {"--dump", "-D", 'D', ArgType::OPTION, "%f"},
  {"-fserial", "-fs", 's', ArgType::FLAG, ""},
  {"-fparallel", "-fp", 'p', ArgType::FLAG, ""},
};

// --- FUNCTION DECLARATIONS --- //

auto read_args(int argc, char** argv) -> utility::Options;
auto parse_options(
                   const CMDLineParams,
                   const Arguments,
                   utility::Options&) -> void;
auto get_option(
                const CMDLineParam,
                const Arguments,
                char&) -> char;

// Commands

auto print_help(const utility::Options&) -> void;

// --- FUNCTION DEFINITIONS --- //

auto main(int argc, char** argv) -> int {
  try {
    const auto options = read_args(argc, argv);
    const auto& command = reinterpret_cast<utility::Command>(options.command_p);
    command(options);
  } catch (Error err) {
    switch (err.error) {
    case ErrorCode::WRONG_COMMAND_ERR:
      std::cout << "\x1b[31mERROR!! Command " << err.erroneous << " is not recognized!\n";
      break;
    case ErrorCode::WRONG_OPTION_ERR:
      std::cout << "\x1b[31mERROR!! Option " << err.erroneous << " is not recognized!\n";
      break;
    case ErrorCode::NO_VALUE_ERR:
      std::cout << "\x1b[31mERROR!! Option " << err.erroneous << " has no value!\n";
      break;
    case ErrorCode::BAD_VALUE_ERR:
      std::cout << "\x1b[31mERROR!! Value " << err.erroneous << " is badly formed or out of range!\n";
    case ErrorCode::BAD_DUMPING_FAC_ERR:
      std::cout << "\x1b[31mERROR!! Dumping factor needs to be between 0 and 1 (exclusive)!\n";
    }

    const auto options = utility::Options{
      .appname{ argv[0] },
    };
    print_help(options);
    return static_cast<int>(err.error);
  }

  return 0;
}

auto read_args(int argc, char** argv) -> utility::Options {
  CMDLineParams arguments{ {std::string_view(argv[0])} };

  for (uint32_t i{ 1 }; i < static_cast<uint32_t>(argc); i++) {
      arguments.push_back(std::string_view(argv[i]));
  }

  utility::Options options{
    .command_p{ reinterpret_cast<void*>(print_help) },
    .appname{ std::move(arguments[0]) },
  };
  if (arguments.size() >= 2) {
    if (arguments[1] == "help") {
      // Help does not use any other options other than the app name
      // so we do not need to do anything else. Also, we have already
      // set the print_help function as the command to run, so we don't
      // need to set it again
      return options;
    } else if (arguments[1] == "run") {
      options.command_p= reinterpret_cast<void*>(exe::pagerank);

      parse_options(arguments, options_gc, options);
    } else {
      throw Error{ arguments[1], ErrorCode::WRONG_COMMAND_ERR };
    }
  }

  return options;
}

auto parse_options(
                   const CMDLineParams params,
                   const Arguments args,
                   utility::Options& options) -> void {
  for (uint32_t i{ 2 }; i < params.size(); i++) {
    char option = '\0';
    const char type = get_option(params[i], args, option);

    if (option == '\0') throw Error{ params[i], ErrorCode::WRONG_OPTION_ERR };

    CMDLineParam value{};
    switch (type) {
    case 'o':
      if ( (i + 1) >= params.size() ) throw Error{ params[i], ErrorCode::NO_VALUE_ERR };

      value = CMDLineParam{ params[i + 1] };
    }

    std::from_chars_result res{};
    int err;
    switch (option) {
    // amount of iterations
    case 'i':
      res = std::from_chars(
                            value.begin(), value.end(),
                            options.iterations);

      if (res.ec == std::errc::invalid_argument || res.ec == std::errc::result_out_of_range) throw Error{ value, ErrorCode::BAD_VALUE_ERR };
      i++;
      break;
    // amount of jobs
    case 'j':
      res = std::from_chars(
                            value.begin(), value.end(),
                            options.jobs);

      if (res.ec == std::errc::invalid_argument || res.ec == std::errc::result_out_of_range) throw Error{ value, ErrorCode::BAD_VALUE_ERR };
      i++;
      break;
    // dimensions of the matrix
    case 'd':
      err = std::sscanf(
                        value.data(),
                        "%ux%u",
                        &options.dims[0],
                        &options.dims[1]);

      if (err == EOF) throw Error{ value, ErrorCode::BAD_VALUE_ERR };
      i++;
      break;
    // the dumping factor
    case 'D':
      res = std::from_chars(
                            value.begin(), value.end(),
                            options.dump_fac);

      if (res.ec == std::errc::invalid_argument || res.ec == std::errc::result_out_of_range) throw Error{ value, ErrorCode::BAD_VALUE_ERR };
      i++;
      break;
    // use serial implementation
    case 's':
      options.do_serial = true;
      break;
    // use parallel implementation
    case 'p':
      options.do_serial = false;
      break;
    }

  }
}

auto get_option(
                const CMDLineParam param,
                const Arguments args,
                char& option) ->  char {
  option = '\0';
  for (const auto& arg : args) {
      if (param == arg.long_name || param == arg.short_name) {
        option = arg.id;
        switch (arg.type) {
        case ArgType::OPTION:
          return 'o';
        case ArgType::FLAG:
          return 'f';
        default:
          return '?';
        }
      }
  }

  return '\0';
}

auto print_help(const utility::Options& options) -> void {
  std::cout << "\x1b[33m" <<
    R"(Parallel Systems Extracurricular Project -- Christoforos-Marios Mamaloukas -- Parallel Systems Postgraduate Course -- NKUA
-------------------------------------------------------------------------------------------------------------
USAGE: )" << options.appname << R"( <COMMAND> [(<OPTION> <VALUE>).. | <FLAG>..]
                                    
--- AVAILABLE COMMANDS ---
help: Print this message
run: Execute PageRank
                                    
--- AVAILABLE OPTIONS ---
-> [global]
  * --jobs <number> | -j <number> : The amount of threads to run the algo on

-> run
  * --iter <number> | -n <number> : The amount of iterations to run the algorithm for
  * --dims <number>x<number> | -d <number>x<number> : The dimensions of the matrix to generate
  * --dump <number> | -D : The dumping factor to use. Has to be between 0 and 1 (exclusive)

--- AVAILABLE FLAGS ---
-> run
  * -fserial | -fs : Run the serial execution
  * -fparallel | -fp : Run the parallel execution
)" << "\x1b[0m";

  return;
}
