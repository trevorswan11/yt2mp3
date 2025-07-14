CC = zig cc
SRC = src/main.c

all: windows linux macos

windows:
	$(CC) -target x86_64-windows-gnu -O2 -o yt2mp3.exe $(SRC)

linux:
	$(CC) -target x86_64-linux-gnu -O2 -o yt2mp3-linux $(SRC)

macos:
	$(CC) -target aarch64-macos -O2 -o yt2mp3-macos-arm64 $(SRC)

clean:
	rm -f yt2mp3.exe yt2mp3-linux yt2mp3-macos*
