# DOT-8 Fantasy Console

An upgraded version of possibly one of the [worst fantasy game consoles](https://github.com/Vglin21/dot-5).

## Hardware Specifications
- **Display:** 16x16 Monochrome
- **Refresh Rate:** 11.97 Hz
- **RAM:** 8 Bytes
- **Cartridge Size:** 248 Bytes
- **Inputs:** 4-Way D-pad

## Controls

| Key                 | Function                |
| :-----------------: | ----------------------- |
| `Up`                | Up button is pressed    |
| `Left`              | Left button is pressed  |
| `Down`              | Down button is pressed  |
| `Right`             | Right button is pressed |
| `Escape`            | Closes the emulator     |
| `Alt` + `Enter`/`F` | Toggles fullscreen mode |

## Getting Started

### Prerequisites
- A C compiler toolchain
- CMake (3.16+)
- SDL3
- Emscripten SDK (for web version)

### Building for Windows/Linux
```sh
# Clone the repository
git clone https://github.com/Vglin21/dot-8
cd dot-8

# Configure and compile the emulator
cmake -B build
cmake --build build
```

### Building for Web
```sh
# Clone the repository
git clone https://github.com/Vglin21/dot-8
cd dot-8

# Configure and compile the emulator
emcmake cmake -B build-web
cmake --build build-web
```

### Running a Program
- **NOTE:** The repository includes example `.d8` binaries in `examples/`.

```sh
# Launch the binary file in the emulator
./bin/dot-5 examples/pong.d8
```

## Other

### Configuration
- `dot-8.cfg` configuration file is created (on the firt run) and stored in `%localappdata%\DOT-8\` (Windows) or `~/.config/DOT-8` (Linux). alternatively it can be put inside the same folder as `dot-8` or set via `-c`/`--config` flag in the terminal.

### Creating Binaries
- Games for this system are written exclusively in assembly and can be assembled with `d8asm`, which you can find [here](https://github.com/Vglin21/d8asm).