# yt2mp3
A simple YouTube to mp3 converter.

## System Dependencies
This program depends on `yt-dlp` which depends on `ffmpeg`. Ensure both of these dependencies can be found in your systems path.
- `choco install yt-dlp ffmpeg` will work for windows users who have [Chocolatey](https://chocolatey.org/install)
- `brew install yt-dlp ffmpeg` should work on macos

If you are on linux, I trust you know how to install a package.

## Building the Program
After installing the needed system binaries, simply run `zig build --release`. This will output the binary to `zig-out/bin`. You can also just download your platforms release bundle!

## Running the Program
`yt2mp3` is very simple. Just pass the URL you are hoping to download as a command line argument, and then the program will download an mp3 for that video to the working directory!
- `stderr` and `stdout` for the spawned process is hidden by default, but you have show both by passing the `-v` flag after the URL
- While the program will work for long YouTube URLs copied from the search bar, you will get the best results by using the short URL found in the share menu 

## Future Plans
While this project exists for Discord's soundboard, I may revisit it in the future to create a single binary that has the above system binaries baked in. 