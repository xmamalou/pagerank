const Matrix = @This();

const std = @import("std");
const stdout = std.io.getStdOut().writer();

// --- MEMBERS --- //
data: ?[]f64 = null,
/// how many rows the matrix has
x: u64 = 0,
/// how many columns the matrix has
y: u64 = 0,

// --- TYPES --- //

// --- ERRORS --- //
const Error = error{
    ILLEGAL_OPERANDS_ERR,
};

// --- FUNCTIONS --- //
/// Create a matrix with the specified dimensions
pub fn create(
    allocator: std.mem.Allocator, // The allocator to use
    x: u64, // The columns
    y: u64, // The rows
) !Matrix {
    var matrix = Matrix{};

    matrix.data = try allocator.alloc(
        f64,
        x * y,
    );
    matrix.x = x;
    matrix.y = y;

    return matrix;
}

/// Generate a matrix with random values with the specified dimensions
pub fn generate(
    allocator: std.mem.Allocator, // The allocator to use
    x: u64, // The columns
    y: u64, // The rows
) !Matrix {
    var matrix = try Matrix.create(allocator, x, y);

    var dev_rand = try std.fs.openFileAbsolute(
        "/dev/urandom",
        .{
            .mode = .read_only,
        },
    );
    defer dev_rand.close();
    errdefer {
        matrix.destroy(allocator);
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

/// Destroy a matrix, freeing its resources
pub inline fn destroy(
    self: Matrix,
    allocator: std.mem.Allocator,
) void {
    if (self.data != null) allocator.free(self.data.?);
}

/// Copy a matrix to this one
pub inline fn copy(
    self: Matrix,
    matrix: Matrix,
) !Matrix {
    if (self.x != matrix.x and self.y != matrix.y) return Error.ILLEGAL_OPERANDS_ERR;

    for (0..self.x) |i| {
        for (0..self.y) |j| {
            _ = self.set(
                i,
                j,
                matrix.get(i, j),
            );
        }
    }

    return self;
}

/// Set a value on a matrix's specified cell.
pub inline fn set(
    self: Matrix,
    i: u64,
    j: u64,
    value: f64,
) Matrix {
    self.data.?[i + self.x * j] = value;

    return self;
}

/// Get a value from a matrix's specified cell
pub inline fn get(
    self: Matrix,
    i: u64,
    j: u64,
) f64 {
    return self.data.?[i + self.x * j];
}

/// Multiply a matrix by a constant
pub inline fn multiply_factor(
    self: Matrix,
    factor: f64,
) Matrix {
    for (0..self.x) |i| {
        for (0..self.y) |j| {
            _ = self.set(
                i,
                j,
                self.get(i, j) * factor,
            );
        }
    }

    return self;
}

/// Add two matrices together
pub fn add_matrix(
    self: Matrix,
    matrix: Matrix,
) !Matrix {
    if (self.x != matrix.x and self.y != matrix.y) return Error.ILLEGAL_OPERANDS_ERR;

    for (0..self.x) |i| {
        for (0..self.y) |j| {
            _ = self.set(
                i,
                j,
                self.get(i, j) + matrix.get(i, j),
            );
        }
    }

    return self;
}

/// Multiply a matrix with another matrix. Creates a new matrix
pub fn multiply_matrix(
    self: Matrix,
    allocator: std.mem.Allocator,
    matrix: Matrix,
) !Matrix {
    if (self.y != matrix.x) return Error.ILLEGAL_OPERANDS_ERR;

    var new_matrix = try Matrix.create(
        allocator,
        self.x,
        matrix.y,
    );

    for (0..new_matrix.x) |i| {
        for (0..new_matrix.y) |j| {
            _ = new_matrix.set(
                i,
                j,
                value: {
                    var sum: f64 = 0.0;
                    for (0..self.y) |k| {
                        sum += self.get(i, k) * self.get(k, j);
                    }
                    break :value sum;
                },
            );
        }
    }

    return new_matrix;
}
