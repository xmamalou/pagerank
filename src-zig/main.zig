const std = @import("std");
const proc = std.process;
const mem = std.mem;

const stdin = std.io.getStdIn().reader();
const stdout = std.io.getStdOut().writer();
const stderr = std.io.getStdErr().writer();

const Pagerank = @import("pagerank.zig");

// --- TYPE DEFINITIONS --- //

const Section = enum {
    help,
    version,
    exercise,
};

pub const Options = struct {
    app_name: []const u8 = "exe",
    file_path: []const u8 = "",
    do_serial: bool = true,
    command: *const fn (Options) Error!void = print_help,
    iterations: u64 = 1,
    dumping: f64 = 0.5,
    dims: [2]u64 = [_]u64{ 2, 2 },
    tries: u64 = 1,
};

pub const Error = error{
    UNSUPPORTED_TYPE_ERR,
    NO_VALUE_ERR,
    INSUFFICIENT_COMMANDS_ERR,
    INVALID_CMD_ARG_ERR,
    IO_ERR,
    OOM_ERR,
    MEM_ERR,
};

// --- FUNCTION DEFINITIONS --- //

// Main function
pub fn main() !void {
    var args = proc.args();
    defer args.deinit();

    const options = read_args(&args) catch Options{};

    try options.command(options);
}

/// Read arguments from the command line
fn read_args(
    arguments_p: *proc.ArgIterator, // the argument iterator
) !Options {
    var options = Options{};

    options.app_name = arguments_p.*.next().?;

    while (arguments_p.*.next()) |arg| {
        if (arg.len > 2 and std.mem.eql( // argument is a flag
            u8,
            arg[0..2],
            "-f",
        )) {
            options.do_serial = set_flag(
                arg,
                &[_][]const u8{
                    "-fserial",
                    "-fs",
                },
            ) orelse options.do_serial;

            options.do_serial = !(set_flag(
                arg,
                &[_][]const u8{
                    "-fparallel",
                    "-fp",
                },
            ) orelse !options.do_serial);
        } else if (arg.len > 2 and std.mem.eql( // argument is an option
            u8,
            arg[0..2],
            "--",
        )) {
            const next_arg = arguments_p.*.next();

            // File path to save the data too
            options.file_path = try set_option(
                arg,
                next_arg,
                []const u8,
                &[_][]const u8{
                    "--filepath",
                    "--f",
                },
            ) orelse options.file_path;

            // How many iterations to run the algorithm for
            options.iterations = try set_option(
                arg,
                next_arg,
                u64,
                &[_][]const u8{
                    "--iter",
                    "--n",
                },
            ) orelse options.iterations;

            // The dumping factor
            options.dumping = try set_option(
                arg,
                next_arg,
                f64,
                &[_][]const u8{
                    "--dumping",
                    "--d",
                },
            ) orelse options.dumping;

            // The x dimension of the matrix
            options.dims[0] = try set_option(
                arg,
                next_arg,
                u64,
                &[_][]const u8{
                    "--x",
                },
            ) orelse options.dims[0];

            options.dims[1] = try set_option(
                arg,
                next_arg,
                u64,
                &[_][]const u8{
                    "--y",
                },
            ) orelse options.dims[1];

            // Tries for the experiment
            options.tries = try set_option(
                arg,
                next_arg,
                u64,
                &[_][]const u8{
                    "--tries",
                    "--t",
                },
            ) orelse options.tries;
        }

        // if the above checks fail, the argument might be a command
        options.command = try set_commands(
            arg,
            &[_][]const u8{
                "help",
                "run",
            },
            &[_]*const fn (Options) Error!void{
                print_help,
                Pagerank.pagerank,
            },
        ) orelse options.command;
    }

    return options;
}

fn set_commands(
    argument: []const u8,
    comptime names: []const []const u8,
    comptime funcs: []const *const fn (Options) Error!void,
) !?*const fn (Options) Error!void {
    if (names.len != funcs.len) return Error.INSUFFICIENT_COMMANDS_ERR;

    for (0..names.len) |i| {
        if (mem.eql(
            u8,
            names[i],
            argument,
        )) {
            return funcs[i];
        }
    }

    return null;
}

fn set_option(
    argument: []const u8,
    value: ?[]const u8,
    comptime target_type: type,
    comptime names: []const []const u8,
) !?target_type {
    if (value == null) {
        return Error.NO_VALUE_ERR;
    }

    for (names) |name| {
        if (mem.eql(
            u8,
            argument,
            name,
        )) {
            return switch (target_type) {
                i8, i16, i32, i64 => try std.fmt.parseInt(
                    target_type,
                    value.?,
                    10,
                ),
                u8, u16, u32, u64 => try std.fmt.parseUnsigned(
                    target_type,
                    value.?,
                    10,
                ),
                f16, f32, f64 => try std.fmt.parseFloat(
                    target_type,
                    value.?,
                ),
                []const u8 => value.?,
                else => Error.UNSUPPORTED_TYPE_ERR,
            };
        }
    }

    return null;
}

/// @brief
fn set_flag(
    argument: []const u8,
    comptime names: []const []const u8,
) ?bool {
    for (names) |name| {
        if (mem.eql(
            u8,
            argument,
            name,
        )) {
            return true;
        }
    }

    return null;
}

fn print_help(options: Options) Error!void {
    stdout.print(
        \\Parallel Systems Postgrad Course -- Extra Project -- Christoforos-Marios Mamaloukas
        \\USAGE:
        \\  {s} [COMMAND] [OPTIONS.. | FLAGS..]
        \\Available commands:
        \\  * help: Shows the current message
        \\  * run: Run the algorithm with the passed parameters
        \\Available options:
        \\  * --file <path>, --f <path>: Where to save the experiment data
        \\  * --iter <number>, --n <number>: How many iterations to run the algorithm
        \\  * --tries <number>, --t <number>: How many times to run the experiment
    ,
        .{
            options.app_name,
        },
    ) catch {
        return Error.IO_ERR;
    };
}
