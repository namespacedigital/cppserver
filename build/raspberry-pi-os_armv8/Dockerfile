# syntax = docker/dockerfile:1.1.1-experimental
FROM ubuntu:18.04

ARG PACKAGE_NAME

LABEL jp.shiguredo.momo=$PACKAGE_NAME

RUN rm -f /etc/apt/apt.conf.d/docker-clean; echo 'Binary::apt::APT::Keep-Downloaded-Packages "true";' > /etc/apt/apt.conf.d/keep-cache

COPY script/apt_install_arm.sh /root/
RUN --mount=type=cache,id=$PACKAGE_NAME,target=/var/cache/apt --mount=type=cache,id=$PACKAGE_NAME,target=/var/lib/apt \
  /root/apt_install_arm.sh

# RootFS 
COPY script/init_rootfs_arm64.sh /root/
COPY rpi-raspbian.conf /root/
RUN --mount=type=cache,id=$PACKAGE_NAME,target=/var/cache/apt --mount=type=cache,id=$PACKAGE_NAME,target=/var/lib/apt \
  /root/init_rootfs_arm64.sh /root/rootfs /root/rpi-raspbian.conf


# clang comp
ARG LLVM_VERSION
COPY _cache/llvm/ /root/_cache/llvm/
COPY script/get_llvm.sh /root/
RUN /root/get_llvm.sh  "$LLVM_VERSION" /root/llvm /root/_cache/llvm

# Boost 
ARG BOOST_VERSION
COPY _cache/boost/ /root/_cache/boost/
COPY script/setup_boost.sh /root/
RUN \
  set -ex \
  && /root/setup_boost.sh "$BOOST_VERSION" /root/boost-source /root/_cache/boost \
  && cd /root/boost-source/source \
  && echo 'using clang : : /root/llvm/clang/bin/clang++ : ;' > project-config.jam \
  && ./b2 \
    cxxstd=17 \
    cxxflags=' \
      -D_LIBCPP_ABI_NAMESPACE=Cr \
      -D_LIBCPP_ABI_VERSION=2 \
      -D_LIBCPP_DISABLE_AVAILABILITY \
      -nostdinc++ \
      -isystem/root/llvm/libcxx/include \
      --target=aarch64-linux-gnu \
      --sysroot=/root/rootfs \
      -I/root/rootfs/usr/include/aarch64-linux-gnu \
    ' \
    linkflags=' \
      -L/root/rootfs/usr/lib/aarch64-linux-gnu \
      -B/root/rootfs/usr/lib/aarch64-linux-gnu \
    ' \
    toolset=clang \
    visibility=global \
    target-os=linux \
    architecture=arm \
    address-model=64 \
    link=static \
    variant=release \
    install \
    -d+0 \
    -j`nproc` \
    --ignore-site-config \
    --prefix=/root/boost \
    --with-filesystem \
    --with-json


# CMake 
ARG CMAKE_VERSION
COPY script/get_cmake.sh /root/
RUN /root/get_cmake.sh "$CMAKE_VERSION" linux /root
ENV PATH "/root/cmake/bin:$PATH"
