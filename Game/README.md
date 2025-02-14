# Raylib Starter Kit <a name="raylib-starter-kit"></a>

THIS IS A COPY OF PHILIP BOURKE'S REPO. IT HAS BEEN EDITTED TO WORK WITH C++

A cross-platform Makefile starter kit for building desktop and web applications using Raylib and Emscripten. This toolkit enables developers to create games that run on Linux, Windows, macOS, and web browsers from a single codebase.

## Table of Contents

- [Overview](#overview)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
  - [Linux Installation](#linux-installation)
  - [Windows Installation](#windows-installation)
  - [macOS Installation](#macos-installation)
- [Building and Running](#building-and-running)
- [Web Development](#web-development)
- [Resources](#resources)
- [Support](#support)

## Overview <a name="overview"></a>

This starter kit provides:

- Cross-platform development environment setup
- Desktop builds (Linux, Windows, macOS)
- Web builds (HTML/JavaScript via Emscripten)
- Automated toolchain setup scripts
- Sample project structure

	```
	raylib_starter/
	├── include/ 
	│   ├── game.h                # Game update methods
	│   ├── player.h              # (Optional) Player-related methods and data
	│   ├── enemy.h               # (Optional) Enemy-related methods and data
	│   └── collision.h           # (Optional) Collision detection functions
	├── src/
	│   ├── main.c                # Hello Raylib and GameLoop
	│   ├── game.c                # Game update implementation
	│   ├── player.c              # (Optional) Player logic and update implementation
	│   ├── enemy.c               # (Optional) Enemy logic and update implementation
	│   └── collision.c           # (Optional) Collision detection logic
	├── template/
	│   └── template.html         # Basic index HTML game page template
	├── toolchain/
	│   ├── build_web.sh          # Build emcc web target
	│   ├── clone_repo.sh         # Clone and build from source
	│   ├── toolchain_linux.sh    # Debian, Ubuntu, Linux Mint, and WSL setup
	│   ├── toolchain_macos.sh    # macOS setup
	│   ├── toolchain_windows.sh  # Windows setup using MSYS2
	│   └── resources/            # Resource files for messages, helpers, etc.
	│       ├── messages.mk       # Standard messages for Makefile outputs
	│       └── messages.sh       # MEssage functions
	├── Makefile                  # Central build management with platform detection
	└── README.md                 # Project setup, usage, and contribution guide
	```


## Prerequisites <a name="prerequisites"></a>

- C/C++ development environment
- CMake (3.0 or higher)
- Git
- Platform-specific build tools
- Python (for web server)

## Installation <a name="installation"></a>

### Linux Installation <a name="linux-installation"></a>

1. **Install Dependencies**
   ```bash
   # Update system packages
   sudo apt update
   sudo apt upgrade

   # Install build tools
   sudo apt install -y build-essential cmake git
   ```

2. **Clone and Setup**
   ```bash
   # Clone repository
   git clone https://bitbucket.org/MuddyGames/raylib_starter.git
   cd raylib_starter

   # Set up toolchain
   chmod +x toolchain_linux.sh
   chmod +x build_web.sh
   make
   ```

### Windows Installation <a name="windows-installation"></a>

1. **Install MSYS2**

	- Download MSYS2 from [msys2.org](https://www.msys2.org/)
	- Run the installer and follow the setup instructions
	- Open MSYS2 UCRT64 terminal

2. **Setup Development Environment**
   ```bash
   # Update package database
   pacman -Syu

   # Install required tools
   pacman -S git make mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake
   ```

3. **Clone and Setup**
   ```bash
   git clone https://bitbucket.org/MuddyGames/raylib_starter.git
   cd raylib_starter
   make
   ```

### macOS Installation <a name="macos-installation"></a>

1. **Install Dependencies**
   ```bash
   # Install Homebrew if not already installed
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

   # Install required packages
   brew update
   brew install cmake
   brew install gcc
   brew install git
   ```

2. **Clone and Setup**
   ```bash
   # Clone repository
   git clone https://bitbucket.org/MuddyGames/raylib_starter.git
   cd raylib_starter

   # Set up toolchain
   chmod +x toolchain_macos.sh
   make
   ```

## Building and Running <a name="building-and-running"></a>

Use the following make commands to build and run your project:

```bash
# Build all targets (desktop)
make all

# Build desktop version
make build

# Run desktop version
make run

# Build web version
make build_web

# Run web version (starts local server)
make run_web

# Clean build files
make clean

# Clean toolchain files
make clean_toolchain

# Build release (desktop)
make CONFIG=release
```

## Web Development <a name="web-fevelopment"></a>

The web version requires Emscripten for compilation. The toolchain script will handle the installation automatically. To run the web version:

1. Build the web version:
   ```bash
   make build_web
   ```

2. Start the local server:
   ```bash
   make run_web
   ```

3. Open your browser and navigate to `http://localhost:8000`

## Resources <a name="resources"></a>

- [Raylib Official Documentation](https://www.raylib.com/)
- [Emscripten Documentation](https://emscripten.org/)
- [GNU Make Manual](https://www.gnu.org/software/make/manual/html_node/Introduction.html)

## Support <a name="support"></a>

For questions and support, contact:
- MuddyGames


[Back to top](#raylib-starter-kit)
