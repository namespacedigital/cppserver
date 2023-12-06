#!/bin/bash

apt-get update
apt-get -y upgrade

# Ubuntu 18.04 tzdata  noninteractive
apt-get -y install tzdata
echo 'Europe/Bucharest' > /etc/timezone
dpkg-reconfigure -f noninteractive tzdata

DEBIAN_FRONTEND=noninteractive apt-get -y install \
  binutils-aarch64-linux-gnu \
  binutils-arm-linux-gnueabi \
  binutils-arm-linux-gnueabihf \
  build-essential \
  curl \
  g++-multilib \
  git \
  gtk+-3.0 \
  lbzip2 \
  libgtk-3-dev \
  lsb-release \
  multistrap \
  python \
  rsync \
  sudo \
  vim \
  xz-utils

# Ubuntu 18.04  multistrap。
# https://github.com/volumio/Build/issues/348#issuecomment-462271607 を参照
sed -e 's/Apt::Get::AllowUnauthenticated=true/Apt::Get::AllowUnauthenticated=true";\n$config_str .= " -o Acquire::AllowInsecureRepositories=true/' -i /usr/sbin/multistrap
