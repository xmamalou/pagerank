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
    const matrix = Matrix.generate(
        allocator,
        options.dims[0],
        options.dims[1],
    ) catch return main.Error.MEM_ERR;
    defer matrix.destroy(allocator);

    stdout.print("Generated matrix!\n", .{}) catch return main.Error.IO_ERR;

    (if (options.do_serial)
        pagerank_serial(
            allocator,
            matrix,
            options,
        )
    else
        pagerank_parallel()) catch return main.Error.MEM_ERR;
}

fn pagerank_serial(
    allocator: std.mem.Allocator,
    matrix: Matrix,
    options: main.Options,
) !void {
    const initial_vec = try Matrix.create(
        allocator,
        matrix.x,
        1,
    );
    defer initial_vec.destroy(allocator);
    const added_vec = try Matrix.create(
        allocator,
        matrix.x,
        1,
    );
    defer added_vec.destroy(allocator);

    for (0..matrix.x) |i| {
        _ = added_vec.set(
            i,
            0,
            1.0 - options.dumping,
        );

        _ = initial_vec.set(
            i,
            0,
            0.0,
        );
    }

    _ = matrix.multiply_factor(options.dumping);

    const start = std.time.milliTimestamp();
    for (0..options.iterations) |_| {
        var interm_matrix = try matrix
            .multiply_matrix(allocator, initial_vec);
        defer interm_matrix.destroy(allocator);

        _ = try initial_vec.copy(try interm_matrix.add_matrix(added_vec));
    }
    const end = std.time.milliTimestamp();

    for (0..matrix.x) |i| {
        try stdout.print(
            "x[{d}] = {d:.3}\n",
            .{
                i,
                initial_vec.get(i, 0),
            },
        );
    }

    try stdout.print(
        "Completed algorithm in {d} msecs!\n",
        .{end - start},
    );
}

fn pagerank_parallel() !void {}
