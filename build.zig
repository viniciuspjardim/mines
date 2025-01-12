const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const lib = b.addStaticLibrary(.{
        .name = "mlib",
        .target = target,
        .optimize = optimize,
        .link_libc = true,
    });

    lib.addCSourceFile(.{ .file = b.path("src/mlib.c") });

    b.installArtifact(lib);

    const exe = b.addExecutable(.{
        .name = "mines",
        .target = target,
        .optimize = optimize,
    });

    exe.linkLibrary(lib);
    exe.addCSourceFile(.{ .file = b.path("src/mines.c") });

    b.installArtifact(exe);
}
