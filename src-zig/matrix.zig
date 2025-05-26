const Matrix = @This();

const std = @import("std");
const stdout = std.io.getStdOut().writer();

// --- MEMBERS --- //
data: ?[]f64 = null,
/// how many rows the matrix has
x: u64 = 0,
/// how many columns the matrix has
y: u64 = 0,

// --- FUNCTIONS --- //
pub fn generate_matrix(
    allocator: std.mem.Allocator,
    x: u64,
    y: u64,
) !Matrix {
    var matrix = Matrix{};

    matrix.data = try allocator.alloc(
        f64,
        x * y,
    );
    matrix.x = x;
    matrix.y = y;

    var dev_rand = try std.fs.openFileAbsolute(
        "/dev/urandom",
        .{
            .mode = .read_only,
        },
    );
    defer dev_rand.close();
    errdefer {
        allocator.free(matrix.data.?);
        matrix.data = null;
    }

    var buff: [4]u8 = undefined;

    for (0..matrix.x) |i| {
        for (0..matrix.y) |j| {
            _ = try dev_rand.read(&buff);
            const rand = @as(
                f64,
                @floatFromInt((@as(u64, buff[2]) << 3) +
                    (@as(u64, buff[2]) << 2) +
                    (@as(u64, buff[1]) << 1) +
                    @as(u64, buff[0])),
            ) / 20.0; // You're a wizard, Harry

            matrix.data.?[i + matrix.y * j] = rand;
        }
    }

    return matrix;
}

pub fn destroy_matrix(
    self: Matrix,
    allocator: std.mem.Allocator,
) void {
    if (self.data != null) allocator.free(self.data.?);
}
