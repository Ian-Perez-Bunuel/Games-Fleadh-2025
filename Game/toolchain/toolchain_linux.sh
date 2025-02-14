#!/bin/bash

# Clone functions
source ./toolchain/clone_repo.sh

# Message functions
source ./toolchain/messages.sh

info_msg "Linux Toolchain Setup Starts"

# Exit immediately if a command exits with a non-zero status
set -e

# Function to handle errors
handle_error() {
    error_msg "Environment Setup Failed: Error during step '$1'"
    exit 1
}

# Function to check if a package is installed
apt_install() {
    PACKAGE_NAME=$1
    if dpkg-query -W -f='${Status}' $PACKAGE_NAME 2>/dev/null | grep -q "install ok installed"; then
		success_msg "$PACKAGE_NAME is already installed, skipping..."
    else
        info_msg "Installing $PACKAGE_NAME..."
        sudo apt install -y $PACKAGE_NAME
    fi
}

# Trap to catch any error and run the handle_error function
trap 'handle_error $STEP' ERR

# Step 1: Check for destination directory argument and build type
STEP="Checking for destination directory argument and build type"
info_msg "Step 1: $STEP"
DEST_DIR=${1:-$(pwd)}  # Use the first argument as the destination directory, or default to the current directory
BUILD_TYPE=${2:-"all"}  # Use the second argument as the build type, or default to the all toolchains

info_msg "Installing in: $DEST_DIR"

# Step 2: Create the destination directory if it doesn't exist
STEP="Creating the destination directory if it doesn't exist"
info_msg "Step 2: $STEP"
mkdir -p "$DEST_DIR"
cd "$DEST_DIR" || { error_msg "Failed to navigate to directory $DEST_DIR"; exit 1; }

# Step 3: Verify the Operating System
STEP="Verify the operating system"
info_msg "Step 3: $STEP"
OS="$(uname -s)"
case "$OS" in
	Linux*)

		if [[ -z "$BUILD_TYPE" || "$BUILD_TYPE" == "build" ]]; then
			info_msg "Running Linux setup..."

			# Step 4: Update and upgrade package lists and installed packages
			STEP="Updating and upgrading package lists and installed packages"
			info_msg "Step 4: $STEP"
			sudo apt update && sudo apt upgrade -y

			# Step 5: Install core build tools
			STEP="Installing core build tools (build-essential, cmake)"
			info_msg "Step 5: $STEP"
			apt_install build-essential
			apt_install cmake

			# Step 6: Install audio libraries
			STEP="Installing audio libraries (libasound2-dev, libpulse-dev, libopenal-dev)"
			info_msg "Step 6: $STEP"
			apt_install libasound2-dev
			apt_install libpulse-dev
			apt_install libopenal-dev

			# Step 7: Install graphics and rendering libraries
			STEP="Installing graphics and rendering libraries (libgl1-mesa-dev, libegl1-mesa-dev, etc.)"
			info_msg "Step 7: $STEP"
			apt_install libgl1-mesa-dev
			apt_install libegl1-mesa-dev
			apt_install libgbm-dev
			apt_install libdrm-dev
			apt_install libglfw3-dev

			# Step 8: Install X Window system libraries
			STEP="Installing X Window system libraries (libx11-dev, libxcursor-dev, libxi-dev, libxrandr-dev)"
			info_msg "Step 8: $STEP"
			apt_install libx11-dev
			apt_install libxcursor-dev
			apt_install libxi-dev
			apt_install libxrandr-dev

			# Install 'wslu' Windows Subsystem for Linux utility
			if ( grep -qi Microsoft /proc/version ); then
				apt_install wslu
			fi
			
			# Multimonitor support
			apt_install libxinerama-dev
			
			# Handling keyboard input
			apt_install libxkbcommon-dev 

			# Step 9: Clone the Raylib source code repository
			STEP="Cloning Raylib source code repository"
			cd "$DEST_DIR"
			info_msg "Step 9: $STEP"

			clone_repo https://github.com/raysan5/raylib.git "raylib"

			# Step 10: Navigate to the Raylib source directory
			STEP="Navigating to the Raylib source directory"
			info_msg "Step 10: $STEP"
			cd raylib

			# Step 11: Create a build directory for Raylib
			STEP="Creating a build directory for Raylib"
			info_msg "Step 11: $STEP"
			mkdir build && cd build

			# Step 12: Generate Makefiles using CMake
			STEP="Generating Makefiles using CMake"
			info_msg "Step 12: $STEP"
			cmake ..

			# Step 13: Build Raylib using Make
			STEP="Building Raylib using Make"
			info_msg "Step 13: $STEP"
			make

			# Step 14: Install Raylib system-wide
			STEP="Installing Raylib system-wide"
			info_msg "Step 14: $STEP"
			sudo make install

			success_msg "raylib Linux Environment Setup Complete"

		elif [[ -z "$BUILD_TYPE" || "$BUILD_TYPE" == "build_web" ]]; then

			# Step 15: Clone the Emscripten SDK repository
			STEP="Cloning Emscripten SDK repository"
			cd "$DEST_DIR"
			info_msg "Step 15: $STEP"

			clone_repo https://github.com/emscripten-core/emsdk.git "emsdk"

			# Step 16: Navigate to the Emscripten SDK source directory
			STEP="Navigating to the Emscripten SDK source directory"
			info_msg "Step 16: $STEP"
			cd "$DEST_DIR/emsdk"

			# Step 17: Install and activate the latest Emscripten version
			STEP="Installing and activating the latest Emscripten version"
			info_msg "Step 17: $STEP"
			./emsdk install latest
			./emsdk activate latest

			# Step 18: Set up the Emscripten environment variables
			STEP="Setting up Emscripten environment variables"
			info_msg "Step 18: $STEP"
			source ./emsdk_env.sh

			# Step 19: Navigate back to Raylib and create a build directory for web targets
			STEP="Navigating back to Raylib and creating a build directory for web targets"
			info_msg "Step 19: $STEP"
			cd "$DEST_DIR/raylib"
			mkdir build_web && cd build_web

			# Step 20: Generate Makefiles using emcmake for the web platform
			STEP="Generating Makefiles using emcmake for building Raylib for web"
			info_msg "Step 20: $STEP"
			emcmake cmake .. -DBUILD_EXAMPLES=OFF -DBUILD_GLFW=OFF -DUSE_WAYLAND=OFF -DUSE_X11=OFF -DPLATFORM=Web

			# Step 21: Build Raylib for web targets using emmake
			STEP="Building Raylib for web targets using emmake"
			info_msg "Step 21: $STEP"
			emmake make

			success_msg "Emscripten Linux Environment Setup Complete"
	
		else
			# Actions for any other case
    		warning_msg "BUILD_TYPE is set to something else: $BUILD_TYPE"
		fi
		
        ;;
    *)
        error_msg "Unsupported Operating System: $OS"
        exit 1
        ;;
esac
