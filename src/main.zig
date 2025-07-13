const std = @import("std");
const builtin = @import("builtin");

pub fn main() !void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const stdout = std.io.getStdOut().writer();
    const stderr = std.io.getStdErr().writer();

    const args = try std.process.argsAlloc(allocator);
    defer allocator.free(args);

    const url: []const u8 = if (args.len > 1) blk: {
        break :blk args[1];
    } else {
        try stderr.print("Usage: yt2mp3 <url>", .{});
        return error.MalformedArgs;
    };
    const url_trimmed = std.mem.trim(u8, url, "\"\'");

    const verbose = if (args.len > 2) blk: {
        break :blk std.mem.eql(u8, args[2], "-v");
    } else false;

    const shell = if (builtin.os.tag == .windows) "cmd.exe" else "sh";
    const shell_flag = if (builtin.os.tag == .windows) "/C" else "-c";

    const header = "yt-dlp -x --audio-format mp3 --audio-quality 192K --output %(title)s.%(ext)s";
    const full_cmd = try std.fmt.allocPrint(allocator, "{s} {s}", .{ header, url_trimmed });
    defer allocator.free(full_cmd);

    const result = std.process.Child.run(.{
        .allocator = allocator,
        .argv = &[_][]const u8{ shell, shell_flag, full_cmd },
        .max_output_bytes = std.math.maxInt(usize),
    }) catch |err| {
        try stderr.print("Failed to run yt-dlp\n", .{});
        return err;
    };

    if (verbose) {
        try stdout.print("{s}", .{result.stdout});
        try stderr.print("{s}", .{result.stderr});
    }
}
