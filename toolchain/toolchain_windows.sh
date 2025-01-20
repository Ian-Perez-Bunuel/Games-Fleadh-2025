#!/bin/bash

# Clone functions
source ./toolchain/clone_repo.sh

# Message functions
source ./toolchain/messages.sh

info_msg "Windows (MSYS2) Toolchain Setup Starts"

# Exit immediately if a command exits with a non-zero status
set -e

# Function to handle errors
handle_error() {
	error_msg "Environment Setup Failed: Error during step '$1'"
	exit 1
}

# Trap to catch any error and run the handle_error function
trap 'handle_error $STEP' ERR

# Function to check if a package is installed
pacman_install() {
	PACKAGE_NAME=$1
	if pacman -Q "$PACKAGE_NAME" &>/dev/null; then
		success_msg "$PACKAGE_NAME is already installed, skipping..."
    else
		info_msg "Installing $PACKAGE_NAME..."
		pacman -S --noconfirm "$PACKAGE_NAME"
	fi
}

# Step 1: Check for destination directory argument
STEP="Checking for destination directory argument"
info_msg "Step 1: $STEP"
DEST_DIR=${1:-$(pwd)}  # Use the first argument as the destination directory, or default to the current directory
BUILD_TYPE=${2:-"all"}  # Use the second argument as the build type, or default to the all toolchains

info_msg "Installing in: $DEST_DIR"

# Step 2: Create the destination directory if it doesn't exist
STEP="Creating the destination directory if it doesn't exist"
info_msg "Step 2: $STEP"
mkdir -p "$DEST_DIR"
cd "$DEST_DIR" || { error_msg "Failed to navigate to directory $DEST_DIR"; exit 1; }

# Step 3: Detect the Operating System
STEP="Detecting the operating system"
info_msg "Step 3: $STEP"
OS="$(uname -s)"
case "$OS" in
	MINGW* | MSYS*)

		if [[ -z "$BUILD_TYPE" || "$BUILD_TYPE" == "build" ]]; then

			info_msg "Running Windows (MSYS2) setup..."
			
			# Step 4: Update MSYS2 package manager
			STEP="Updating MSYS2 package manager"
			info_msg "Step 4: $STEP"

			# Display warning message and wait for user input
			warning_msg "UGRADE pacman MSYS2 may shutdown and you will need to restart the ucrt64 terminal via the executable: c:/mysys64/ucrt64.exe"
			warning_msg "pacman will now be updated. Press any key to continue."
			read -n 1 -s -r  # Read any key

			# Proceed with the pacman update
			pacman -Syu --noconfirm
			
			# Step 5: Install core build tools for UCRT
			STEP="Installing core build tools (mingw-w64-ucrt-x86_64-gcc, mingw-w64-ucrt-x86_64-cmake, mingw-w64-ucrt-x86_64-make)"
			info_msg "Step 5: $STEP"
			pacman_install mingw-w64-ucrt-x86_64-gcc
			pacman_install mingw-w64-ucrt-x86_64-cmake
			pacman_install mingw-w64-ucrt-x86_64-make
			pacman_install mingw-w64-ucrt-x86_64-toolchain

			# Step 6: Install additional libraries for Project
			STEP="Installing additional libraries for Project (python3, unzip, mingw-w64-ucrt-x86_64-ninja, mingw-w64-ucrt-x86_64-nodejs)"
			info_msg "Step 6: $STEP"
			pacman_install python3
			pacman_install unzip
			pacman_install mingw-w64-ucrt-x86_64-ninja
			pacman_install mingw-w64-ucrt-x86_64-nodejs
			pacman_install mingw-w64-ucrt-x86_64-glfw

			# Step 7: Clone the Raylib source code repository
			STEP="Cloning Raylib source code repository"
			cd "$DEST_DIR"
			info_msg "Step 7: $STEP"

			clone_repo https://github.com/raysan5/raylib.git "raylib"

			# Step 8: Navigate to the Raylib source directory
			STEP="Navigating to the Raylib source directory"
			info_msg "Step 8: $STEP"
			cd raylib

			# Step 9: Create a build directory for Raylib
			STEP="Creating a build directory for Raylib"
			info_msg "Step 9: $STEP"
			mkdir build && cd build
			
			# Step 10: Generate Makefiles using CMake
			STEP="Generating Makefiles using CMake"
			info_msg "Step 10: $STEP"
			cmake .. -G "Unix Makefiles" \
			-DCMAKE_BUILD_TYPE=Release \
			-DBUILD_EXAMPLES=OFF \
			-DUSE_EXTERNAL_GLFW=OFF

			# Step 11: Build Raylib using Make
			STEP="Building Raylib using Make"
			info_msg "Step 11: $STEP"
			make

			success_msg "raylib MYSYS2 Environment Setup Complete"

		elif [[ -z "$BUILD_TYPE" || "$BUILD_TYPE" == "build_web" ]]; then

			# Step 12: Clone the Emscripten SDK repository
			STEP="Cloning Emscripten SDK repository"
			cd "$DEST_DIR"
			info_msg "Step 12: $STEP"

			clone_repo https://github.com/emscripten-core/emsdk.git "emsdk"

			# Step 13: Navigate to the Emscripten SDK source directory
			STEP="Navigating to the Emscripten SDK source directory"
			info_msg "Step 13: $STEP"
			cd "$DEST_DIR/emsdk"

			# Step 14: Install and activate the latest Emscripten version
			STEP="Installing and activating the latest Emscripten version"
			info_msg "Step 14: $STEP"
			./emsdk install latest
			./emsdk activate latest

			# Step 15: Set up the Emscripten environment variables
			STEP="Setting up Emscripten environment variables"
			info_msg "Step 15: $STEP"
			source ./emsdk_env.sh

			# Step 16: Navigate back to Raylib and create a build directory for web targets
			STEP="Navigating back to Raylib and creating a build directory for web targets"
			info_msg "Step 16: $STEP"
			cd "$DEST_DIR/raylib"
			mkdir build_web && cd build_web

			# Step 17: Generate Makefiles using emcmake for the web platform
			STEP="Generating Makefiles using emcmake for building Raylib for web"
			info_msg "Step 17: $STEP"

			emcmake cmake -G "Ninja" -DBUILD_GLFW=OFF -DBUILD_EXAMPLES=OFF -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release ..

			# Step 18: Build Raylib for web targets using emmake
			STEP="Building Raylib for web targets using ninja"
			info_msg "Step 18: $STEP"
			ninja

			success_msg "Emscripten (MSYS2) Environment Setup Complete"

		else
			# Actions for any other case
    		error_msg "BUILD_TYPE is set to something else: $BUILD_TYPE"
		fi

		;;
	*)
		error_msg "Unsupported Operating System: $OS"
		exit 1
		;;
esac