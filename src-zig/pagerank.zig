const std = @import("std");
const stdout = std.io.getStdOut().writer();

const main = @import("root");
const Matrix = @import("matrix.zig");

pub fn pagerank(options: main.Options) main.Error!void {
    stdout.print(
        \\Parallel Systems Postgrad Course - Extra Credit Project - Christoforos-Marios Mamaloukas
        \\----------  PAGERANK  ----------
        \\* You selected the {s} execution
        \\* Dumping factor: {d:.4}
        \\* Matrix dimensions: {d}x{d}
        \\* Iterations to run the algorithm for: {d}
        \\* Tries to run the experiment for: {d}
        \\* Path to save data: {s}
        \\--------------------------------
        \\
    ,
        .{
            if (options.do_serial) "serial" else "parallel",
            options.dumping,
            options.dims[0],
            options.dims[1],
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
    const matrix = Matrix.generate_matrix(
        allocator,
        options.dims[0],
        options.dims[1],
    ) catch return main.Error.MEM_ERR;
    defer matrix.destroy_matrix(allocator);

    stdout.print("Generated matrix!\n", .{}) catch return main.Error.IO_ERR;

    if (options.do_serial) pagerank_serial() else pagerank_parallel();
}

fn pagerank_serial() void {}

fn pagerank_parallel() void {}
