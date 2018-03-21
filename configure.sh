#!/bin/bash

set -o errexit # set -e: exit the script if any statement returns a non-true value

# Program constants
ROOT_DIR=$(dirname "${BASH_SOURCE[0]}")
BUILD_DIR="$ROOT_DIR/build"

# Program options
CXX="g++"

function usage() {
  echo -e "Usage:\n$0 [options]"
  echo -e "\nConfigure a build directory using the provided options."
  echo -e "  -h,--help                    Display this help message"
  echo -e "  -c,--clang                   Use clang as compiler"
  echo -e "  -t,--test                    Enable the testbed build target"
  echo
  exit "${1:-1}"
}

function configure() {
  cmake \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DBUILD_TYPE="$HITBOX_BUILDER_BUILD_TYPE" \
    -DHITBOX_BUILDER_BUILD_TESTBED="$HITBOX_BUILDER_BUILD_TESTBED" \
    "$ROOT_DIR"
}

# Process arguments
while [[ $# -gt 0 ]]; do
  case "$1" in
  -h | --help)
    usage 0
    ;;
  -c | --clang)
    CXX="clang++"
    shift
    ;;
  -t | --test)
    HITBOX_BUILDER_BUILD_TESTBED="ON"
    shift
    ;;
  -*)
    echo -e "unknown option '$1'"
    usage
    ;;
  esac
done

mkdir -p "$BUILD_DIR" && cd "$BUILD_DIR" && configure
