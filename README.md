# yt2mp3
A simple YouTube to mp3 converter.

## Building the Binaries
First, unpack the `assets.zip` file into the main project directory. You must have the file structure `assets/ffmpeg` and `assets/yt-dlp` available from the project's build directory. Then, simply run `zig build --release` to build all targets. This will output binaries for Windows, Linux (x64), Macos (arm64) and Macos (x64) to `zig-out/bin`. This is compute heavy, and you may only want to build one binary for your platform. In that case, you should run one of the following:
- `zig build --release windows` will build for the standard x86_64 Win32 API
- `zig build --release linux` will build for x86_64 architectures for Linux distros
- `zig build --release macos-arm` will build for the Apple Silicon architecture
- `zig build --release macos-x64` will build for the legacy Intel CPU Macs

You can also just download your platform's release bundle!

## Running the Program
`yt2mp3` is very simple. Just pass the URL you are hoping to download as a command line argument, and then the program will download an mp3 for that video to the working directory!
- Passing the `-v` flag will print the command passes to the internal binaries to stdout
- While the program will work for long YouTube URLs copied from the search bar, you will get the best results by using the short URL found in the share menu 

You may notice that the program's binary is _very_ large. This is because there is the respective platform's binaries for ffmpeg and yt-dlp built into the binaries. I download the binaries for ffmpeg from [here](https://github.com/eugeneware/ffmpeg-static), and the binaries for yt-dlp from [here](https://github.com/yt-dlp/yt-dlp). If you are uncomfortable with this, then you can get your hands on the binaries yourself. Just make sure they are in the `assets` folder under the same name!