#!/bin/bash

set -o errexit # set -e: exit the script if any statement returns a non-true value

# Program constants
ROOT_DIR="$(
  cd "$(dirname "${BASH_SOURCE[0]}")"
  pwd -P
)"
BUILD_DIR="$ROOT_DIR/build"

# Program options
CXX="g++"

function usage() {
  echo -e "Usage:\n$0 [options]"
  echo -e "\nConfigure a build directory using the provided options."
  echo -e "\nGeneral options:"
  echo -e "  -h,--help                    Display this help message"
  echo -e "  -c,--clang                   Use clang as compiler"
  echo -e "  -t,--test                    Enable the testbed build target"
  echo -e "\nClang-specific options:"
  echo -e "  -y,--tidy                    Enable clang-tidy static analysis target"
  echo
  exit "${1:-1}"
}

function configure() {
  cmake \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DBUILD_TYPE="$HITBOX_BUILDER_BUILD_TYPE" \
    -DHITBOX_BUILDER_ENABLE_CLANGTIDY="$HITBOX_BUILDER_ENABLE_CLANGTIDY" \
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
  -y | --tidy)
    HITBOX_BUILDER_ENABLE_CLANGTIDY="ON"
    shift
    ;;
  -*)
    echo -e "unknown option '$1'"
    usage
    ;;
  esac
done

# Prevent use of clang-only options with other compilers
if [[ "$HITBOX_BUILDER_ENABLE_CLANGTIDY" == "ON" && "$CXX" != "clang++" ]]; then
  echo -e "cannot use clang-tidy if compiler is not clang"
  usage
fi

mkdir -p "$BUILD_DIR" && cd "$BUILD_DIR" && configure
