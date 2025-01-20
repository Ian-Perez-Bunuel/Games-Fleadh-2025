#!/bin/bash

# Message Color Codes
BLUE="\033[0;34m"
GREEN="\033[0;32m"
YELLOW="\033[0;33m"
RED="\033[0;31m"
RESET="\033[0m"

# Color output function (Linux / MYSYS on Windows)
print_msg_unix() {
    echo -e "${1}${2}${RESET}"
}

print_msg_win() {
    printf "%b%s%b\n" "$1" "$2" "$RESET"
}

# OS detection
UNAME_S=$(uname -s)
if [[ "$UNAME_S" == *"MINGW"* ]]; then
    print_msg=print_msg_win
else
    ECHO_TEST=$(echo -e "test" 2>/dev/null)
    if [[ "$ECHO_TEST" == "test" ]]; then
        print_msg=print_msg_unix
    else
        print_msg=print_msg_win
    fi
fi

# Message macros
info_msg() {
    $print_msg "$BLUE" "INFO: $1"
}

success_msg() {
    $print_msg "$GREEN" "SUCCESS: $1"
}

warning_msg() {
    $print_msg "$YELLOW" "WARNING: $1"
}

error_msg() {
    $print_msg "$RED" "ERROR: $1"
}

