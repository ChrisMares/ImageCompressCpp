# ImageCompressCpp

A self-contained C++ project for resizing and compressing images using the stb library.

##Running:
Windows (quick example: copying the .exe to where your files are and saving output to a folder called /sm in that directory)
.\ImageCompressWin.exe --imgdir ./ --outdir ./sm --size 50 --quality 50

## Build Instructions

This project uses shell scripts to simplify the build process for different platforms and configurations.

### Prerequisites (for Windows Cross-Compilation)

Before building the Windows executable on Linux, install the MinGW-w64 toolchain:

```bash
sudo apt update
sudo apt install g++-mingw-w64-x86-64
```

### Running the builds

Make all shell scripts executable
```bash
chmod +x build_all.sh
chmod +x build_linux_debug.sh
chmod +x build_linux_release.sh
chmod +x build_win_release.sh
```

### Build all (build linux-release, linux-debug, windows-release)

```bash
./build_all.sh
```

### building locally on linux for debugging
```bash
./build_linux_debug.sh
```

### example run from project root (linux)
```bash
./build-linux-debug/ImageCompress --outdir /home/chris/Pictures/website/sm --imgdir /home/chris/Pictures/website/ --size 50 --quality 25 --threads 28
```

### example run with 'wine' from project root (linux)
```bash
wine ./build-windows/ImageCompressWin.exe --outdir /home/chris/Pictures/website/sm --imgdir /home/chris/Pictures/website/ --size 50 --quality 25 --threads 28
```
