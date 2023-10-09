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

if [ ! -e $CACHE_DIR/clang+llvm-$LLVM_VERSION-aarch64-linux-gnu.tar.xz ]; then
  mkdir -p $CACHE_DIR
  pushd $CACHE_DIR
    curl -fLo clang+llvm-$LLVM_VERSION-aarch64-linux-gnu.tar.xz https://github.com/llvm/llvm-project/releases/download/llvmorg-$LLVM_VERSION/clang+llvm-$LLVM_VERSION-aarch64-linux-gnu.tar.xz
    tar xf clang+llvm-$LLVM_VERSION-aarch64-linux-gnu.tar.xz
    pushd clang+llvm-$LLVM_VERSION-aarch64-linux-gnu
      # ls
      # mkdir -p $OUTPUT_DIR/clang/bin && cp -r ./bin/* $OUTPUT_DIR/clang/bin
      mkdir -p $OUTPUT_DIR/libcxx/include && cp -r ./include/c++/v1/* $OUTPUT_DIR/libcxx/include
      cp -r ./include/aarch64-unknown-linux-gnu/c++/v1/__config_site $OUTPUT_DIR/libcxx/include
    popd
  popd

  # From chromium prebuilt clang
  pushd $OUTPUT_DIR
    if [ ! -e tools/.git ]; then
      git clone https://chromium.googlesource.com/chromium/src/tools
    fi
    pushd tools
      python3 clang/scripts/update.py --output-dir=$OUTPUT_DIR/clang
    popd
  popd
fi
