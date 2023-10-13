#!/bin/bash

if [ $# -lt 3 ]; then
  echo "$0 <llvm_version> <output_dir> <cache_dir>"
  exit 1
fi

LLVM_VERSION=$1
OUTPUT_DIR=$2
CACHE_DIR=$3

set -ex

mkdir -p $OUTPUT_DIR

# From chromium prebuilt clang
pushd $OUTPUT_DIR
  if [ ! -e tools/.git ]; then
    git clone https://chromium.googlesource.com/chromium/src/tools
  fi
  pushd tools
    python3 clang/scripts/update.py --output-dir=$OUTPUT_DIR/clang
  popd
popd


# libcxx
pushd $OUTPUT_DIR
  if [ ! -e libcxx/.git ]; then
    git clone https://chromium.googlesource.com/external/github.com/llvm/llvm-project/libcxx
  fi
  pushd libcxx
    # git fetch
    # git reset --hard $LIBCXX_COMMIT
  popd
popd

# __config_site buildtools
pushd $OUTPUT_DIR
  if [ ! -e buildtools/.git ]; then
    git clone https://chromium.googlesource.com/chromium/src/buildtools
  fi
  pushd buildtools
    # git fetch
    # git reset --hard $BUILDTOOLS_COMMIT
  popd
  cp $OUTPUT_DIR/buildtools/third_party/libc++/__config_site $OUTPUT_DIR/libcxx/include/
popd

