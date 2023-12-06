#!/bin/bash


cd "`dirname $0`"

set -ex

ARCH_NAME="aarch64-apple-darwin"

SOURCE_DIR="`pwd`/_source"
BUILD_DIR="`pwd`/_build"
INSTALL_DIR="`pwd`/_install"
CACHE_DIR="`pwd`/../../_cache"

mkdir -p $SOURCE_DIR
mkdir -p $BUILD_DIR
mkdir -p $INSTALL_DIR
mkdir -p $CACHE_DIR

source ../../VERSION

if [ -z "$JOBS" ]; then
  JOBS=`sysctl -n hw.logicalcpu_max`
  if [ -z "$JOBS" ]; then
    JOBS=1
  fi
fi

# Boost
BOOST_VERSION_FILE="$INSTALL_DIR/boost.version"
BOOST_CHANGED=0
if [ ! -e $BOOST_VERSION_FILE -o "$BOOST_VERSION" != "`cat $BOOST_VERSION_FILE`" ]; then
  BOOST_CHANGED=1
fi

CLANG="`xcodebuild -find clang`"
CLANGPP="`xcodebuild -find clang++`"

if [ $BOOST_CHANGED -eq 1 -o ! -e $INSTALL_DIR/boost/lib/libboost_filesystem.a ]; then
  rm -rf $SOURCE_DIR/boost
  rm -rf $BUILD_DIR/boost
  rm -rf $INSTALL_DIR/boost
  ../deps/setup_boost.sh $BOOST_VERSION $SOURCE_DIR/boost $CACHE_DIR/boost
  pushd $SOURCE_DIR/boost/source
    echo "using clang : : $CLANGPP : ;" > project-config.jam
    SYSROOT="`xcrun --sdk macosx --show-sdk-path`"
    ./b2 \
      cxxstd=17 \
      cflags=" \
        -target $ARCH_NAME \
        -mmacosx-version-min=11.0 \
        --sysroot=$SYSROOT \
      " \
      cxxflags=" \
        -target $ARCH_NAME \
        -mmacosx-version-min=11.0 \
        --sysroot=$SYSROOT \
      " \
      toolset=clang \
      visibility=hidden \
      link=static \
      variant=release \
      install \
      -d+0 \
      -j$JOBS \
      --build-dir=$BUILD_DIR/boost \
      --prefix=$INSTALL_DIR/boost \
      --ignore-site-config \
      --with-filesystem \
      --with-json \
      --with-thread 
  popd
fi
echo $BOOST_VERSION > $BOOST_VERSION_FILE

