const std = @import("std");
const stdout = std.io.getStdOut().writer();

const main = @import("root");

pub fn pagerank(options: main.Options) main.Error!void {
    stdout.print(
        \\Parallel Systems Postgrad Course - Extra Credit Project - Christoforos-Marios Mamaloukas
        \\----------  PAGERANK  ----------
        \\* You selected the {s} execution
        \\* Dumping factor: {d:.4}
        \\* Iterations to run the algorithm for: {d}
        \\* Tries to run the experiment for: {d}
        \\* Path to save data: {s}
        \\--------------------------------
    ,
        .{
            if (options.do_serial) "serial" else "parallel",
            options.dumping,
            options.iterations,
            options.tries,
            options.file_path,
        },
    ) catch return main.Error.IO_ERR;

    // Generate matrix
    var gpa = std.heap.GeneralPurposeAllocator(.{}).init;
    defer {
        const status = gpa.deinit();
        if (status == .leak) @panic("ERROR: Memory leaks detected!");
    }
    const allocator = gpa.allocator();
    _ = allocator;

    if (options.do_serial) pagerank_serial() else pagerank_parallel();
}

fn pagerank_serial() void {}

fn pagerank_parallel() void {}
