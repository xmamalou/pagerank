const std = @import("std");

pub fn build(b: *std.Build) void {
    const appname = b.option(
        []const u8,
        "appname",
        "Name of the application",
    ) orelse "exe-zig";

    const exe = b.addExecutable(.{
        .name = appname,
        .root_source_file = b.path("src-zig/main.zig"),
        .target = b.graph.host,
    });

    b.installArtifact(exe);
}
