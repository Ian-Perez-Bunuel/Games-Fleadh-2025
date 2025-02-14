#!/bin/bash

# Clone functions
source ./toolchain/clone_repo.sh

# Message functions
source ./toolchain/messages.sh

info_msg "macOS Toolchain Setup Starts"

# Exit immediately if a command exits with a non-zero status
set -e

# Function to handle errors
handle_error() {
    error_msg "Environment Setup Failed: Error during step '$1'"
    exit 1
}

# Function to check if a Homebrew package is installed
brew_install() {
    PACKAGE_NAME=$1
    if brew list -1 | grep -q "^${PACKAGE_NAME}\$"; then
        echo "$PACKAGE_NAME is already installed, skipping..."
    else
        echo "Installing $PACKAGE_NAME..."
        brew install $PACKAGE_NAME
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
	Darwin*)

		if [[ -z "$BUILD_TYPE" || "$BUILD_TYPE" == "build" ]]; then
			info_msg "Running macOS setup..."

			# Step 4: Install core build tools
			STEP="Installing core build tools (cmake, glfw)"
			info_msg "Step 4: $STEP"
			brew_install cmake
            brew_install glfw

			# Step 5: Clone the Raylib source code repository
			STEP="Cloning Raylib source code repository"
			cd "$DEST_DIR"
			info_msg "Step 5: $STEP"

			clone_repo https://github.com/raysan5/raylib.git "raylib"

			# Step 6: Navigate to the Raylib source directory
			STEP="Navigating to the Raylib source directory"
			info_msg "Step 6: $STEP"
			mkdir -p raylib/build
            cd raylib/build

			# Step 7: Generate Makefiles using CMake
			STEP="Generating Makefiles using CMake"
			info_msg "Step 7: $STEP"
			cmake ..

			# Step 8: Build Raylib using Make
			STEP="Building Raylib using Make"
			info_msg "Step 8: $STEP"
			make

			# Step 9: Install Raylib system-wide
			STEP="Installing Raylib system-wide"
			info_msg "Step 9: $STEP"
			sudo make install

			# Step 10: Set PKG_CONFIG_PATH for future terminal sessions
			STEP="Set PKG_CONFIG_PATH for future terminal sessions"
			info_msg "Step 10: $STEP"
			echo 'export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH' >> ~/.bash_profile
			echo 'export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH' >> ~/.zshrc

			success_msg "raylib macOS Environment Setup Complete"

		elif [[ -z "$BUILD_TYPE" || "$BUILD_TYPE" == "build_web" ]]; then

			# Step 11: Clone the Emscripten SDK repository
			STEP="Cloning Emscripten SDK repository"
			cd "$DEST_DIR"
			info_msg "Step 11: $STEP"

			clone_repo https://github.com/emscripten-core/emsdk.git "emsdk"

			# Step 12: Navigate to the Emscripten SDK source directory
			STEP="Navigating to the Emscripten SDK source directory"
			info_msg "Step 12: $STEP"
			cd "$DEST_DIR/emsdk"

			# Step 13: Install and activate the latest Emscripten version
			STEP="Installing and activating the latest Emscripten version"
			info_msg "Step 13: $STEP"
			./emsdk install latest
			./emsdk activate latest

			# Step 14: Set up the Emscripten environment variables
			STEP="Setting up Emscripten environment variables"
			info_msg "Step 14: $STEP"
			source ./emsdk_env.sh

			# Step 15: Navigate back to Raylib and create a build directory for web targets
			STEP="Navigating back to Raylib and creating a build directory for web targets"
			info_msg "Step 15: $STEP"
			cd "$DEST_DIR/raylib"
			mkdir build_web && cd build_web

			# Step 16: Generate Makefiles using emcmake for the web platform
			STEP="Generating Makefiles using emcmake for building Raylib for web"
			info_msg "Step 16: $STEP"
			emcmake cmake .. -DBUILD_EXAMPLES=OFF -DBUILD_GLFW=OFF -DUSE_WAYLAND=OFF -DUSE_X11=OFF -DPLATFORM=Web

			# Step 17: Build Raylib for web targets using emmake
			STEP="Building Raylib for web targets using emmake"
			info_msg "Step 17: $STEP"
			emmake make

			# Step 18: Adding emsdk to PATH in shell profile
			STEP="Building Raylib for web targets using emmake"
			info_msg "Step 18: $STEP"
			echo 'source $HOME/emsdk/emsdk_env.sh' >> ~/.bash_profile
        	echo 'source $HOME/emsdk/emsdk_env.sh' >> ~/.zshrc

			success_msg "Emscripten macOS Environment Setup Complete"
	
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
