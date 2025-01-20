#!/bin/bash

# Message functions
source ./toolchain/messages.sh

# Function repo cloning
clone_repo() {
    REPO_URL=$1
    DIR_NAME=$2
    if [ -d "$DIR_NAME" ]; then
        warning_msg "Directory '$DIR_NAME' already exists. Do you want to delete and clone again? (y/n)"
        read -r choice
        if [ "$choice" == "y" ]; then
            info_msg "Deleting existing '$DIR_NAME' directory..."
            rm -rf "$DIR_NAME"
            info_msg "Cloning repository from $REPO_URL..."
            git clone "$REPO_URL" "$DIR_NAME"
        fi
    else
        info_msg "Cloning repository from $REPO_URL..."
        git clone "$REPO_URL" "$DIR_NAME"
    fi
}