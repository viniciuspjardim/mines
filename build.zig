const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    // Declare the mlib, and add the C source file
    const lib = b.addStaticLibrary(.{
        .name = "mlib",
        .target = target,
        .optimize = optimize,
        .link_libc = true,
    });
    lib.addCSourceFile(.{ .file = b.path("src/mlib.c") });

    // Declare the executable, link mlib, and add the C source file
    const exe = b.addExecutable(.{
        .name = "mines",
        .target = target,
        .optimize = optimize,
    });
    exe.linkLibrary(lib);
    exe.addCSourceFile(.{ .file = b.path("src/mines.c") });

    // Create the executable `mines` file under `./zig-out/bin`
    b.installArtifact(exe);
}
