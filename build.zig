const std = @import("std");

pub fn build(b: *std.Build) void {
    const targets = [_]struct {
        name: []const u8,
        query: std.Target.Query,
        exe_name: []const u8,
    }{
        .{ .name = "windows", .query = .{ .cpu_arch = .x86_64, .os_tag = .windows }, .exe_name = "yt2mp3-win-x64" },
        .{ .name = "linux", .query = .{ .cpu_arch = .x86_64, .os_tag = .linux }, .exe_name = "yt2mp3-linux-x64" },
        .{ .name = "macos-arm", .query = .{ .cpu_arch = .aarch64, .os_tag = .macos }, .exe_name = "yt2mp3-macos-arm64" },
        .{ .name = "macos-x64", .query = .{ .cpu_arch = .x86_64, .os_tag = .macos }, .exe_name = "yt2mp3-macos-x64" },
    };

    const optimize = b.standardOptimizeOption(.{
        .preferred_optimize_mode = .ReleaseSafe,
    });

    const all_step = b.step("all", "Build all targets");
    inline for (targets) |target| {
        const target_obj = b.resolveTargetQuery(target.query);

        const exe = b.addExecutable(.{
            .name = target.exe_name,
            .root_module = b.createModule(.{
                .target = target_obj,
                .optimize = optimize,
                .strip = true,
            }),
        });

        exe.addCSourceFile(.{ .file = b.path("src/main.c"), .flags = &.{ "-std=c99", "-O3", "-Wall" } });
        exe.addIncludePath(.{ .cwd_relative = "assets" });
        exe.linkLibC();

        b.installArtifact(exe);

        const step = b.step(target.name, "Build " ++ target.name);
        const child = b.addInstallArtifact(exe, .{});
        step.dependOn(&child.step);
        all_step.dependOn(&child.step);
    }
    b.default_step = all_step;
}
