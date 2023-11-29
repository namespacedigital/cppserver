#!/bin/bash

set -e

# help
function show_help() {
  echo ""
  echo "$0 <working directory> <cppserver  [mount | nomount]> <Package name> <Docker> <cppserver commit>"
  echo ""
}

WORK_DIR="$1"
CPPSERVER_DIR="$2"
MOUNT_TYPE="$3"
PACKAGE_NAME="$4"
DOCKER_IMAGE="$5"
CPPSERVER_COMMIT="$6"

echo WORK_DIR = $WORK_DIR
echo CPPSERVER_DIR = $CPPSERVER_DIR
echo MOUNT_TYPE = $MOUNT_TYPE
echo PACKAGE_NAME = $PACKAGE_NAME
echo DOCKER_IMAGE = $DOCKER_IMAGE
echo CPPSERVER_COMMIT = $CPPSERVER_COMMIT

# Checking arguments
if [ $# -ne 6 ]; then
  show_help
  exit 1
fi


if [ -z "$WORK_DIR" ]; then
  echo "Error: working directory is empty"
  show_help
  exit 1
fi

if [ -z "$CPPSERVER_DIR" ]; then
  echo "Error: cppserver repository is empty"
  show_help
  exit 1
fi

if [ ! -e "$CPPSERVER_DIR/.git" ]; then
  echo "Error: $CPPSERVER_DIR not the root directory of the repository"
  show_help
  exit 1
fi

if [ "$MOUNT_TYPE" != "mount" -a "$MOUNT_TYPE" != "nomount"  ]; then
  echo "Error: <mount type> must be mount or nomount"
  show_help
  exit 1
fi

if [ -z "$PACKAGE_NAME" ]; then
  echo "Error: package name is empty"
  show_help
  exit 1
fi

if [ -z "$(docker images -q $DOCKER_IMAGE)" ]; then
  echo "Error: <Docker image> $DOCKER_IMAGE does not exists"
  show_help
  exit 1
fi

if [ ! -e "$CPPSERVER_DIR/VERSION" ]; then
  echo "Error: $CPPSERVER_DIR/VERSION does not exists"
  exit 1
fi

source $CPPSERVER_DIR/VERSION

DOCKER_PLATFORM=""
if [ "`uname -sm`" = "Darwin arm64" ]; then
  # M1 Mac の場合は --platform specify
  DOCKER_PLATFORM="--platform=linux/amd64"
fi


if [ "$MOUNT_TYPE" = "mount" ]; then
  #If you want to mount it, simply mount and build
  docker run \
    $DOCKER_PLATFORM \
    -it \
    --rm \
    -v "$WORK_DIR/../..:/root/cppserver" \
    "$DOCKER_IMAGE" \
    /bin/bash -c "
      set -ex
      mkdir -p /root/cppserver/_build/$PACKAGE_NAME
      pushd /root/cppserver/_build/$PACKAGE_NAME
        cmake \
          -DCMAKE_BUILD_TYPE=Release \
          -DCPPSERVER_PACKAGE_NAME=$PACKAGE_NAME \
          -DCPPSERVER_VERSION=$CPPSERVER_VERSION \
          -DCPPSERVER_COMMIT=$CPPSERVER_COMMIT \
          ../..
        if [ -n \"$VERBOSE\" ]; then
          export VERBOSE=$VERBOSE
        fi
        cmake --build . -j\$(nproc)
      popd
    "
else
  # If you do not mount it, start the container, transfer the necessary files to the container, 
  # build on the container, return the generated files from the container, and exit the container.

  pushd $CPPSERVER_DIR
    if git diff-index --quiet HEAD --; then
      :
    else
      #Check for local changes
      git status
      read -p "There are local changes. These changes are not reflected in the build. Do you want to continue？ (y/N): " yn
      case "$yn" in
        [yY]*)
          ;;
        *)
          exit 1
          ;;
      esac
    fi
  popd

  pushd $WORK_DIR
    # Clean up the container without fail even if an error occurs during the process
    trap "set +e; docker container stop cppserver-$PACKAGE_NAME; docker container rm cppserver-$PACKAGE_NAME" 0

    # Transfer it to the container built from the base image and build it.、
    # Once the build is complete, extract artifacts and intermediate files
    docker container create $DOCKER_PLATFORM -it --name cppserver-$PACKAGE_NAME "$DOCKER_IMAGE"
    docker container start cppserver-$PACKAGE_NAME

    # Generate cppserver source for transfer (including intermediate files)
    rm -rf cppserver
    git clone $CPPSERVER_DIR cppserver

    # Copying intermediate files
    mkdir -p $CPPSERVER_DIR/_build
    if [ -e $CPPSERVER_DIR/_build/$PACKAGE_NAME ]; then
      mkdir -p cppserver/_build
      cp -r $CPPSERVER_DIR/_build/$PACKAGE_NAME cppserver/_build/$PACKAGE_NAME
    fi

    # Match update date and time to original file
    pushd cppserver
      find . -type f | while read file; do
        if [ -e "$CPPSERVER_DIR/$file" ]; then
          # -c: do not generate files
          # -m: Update modification date and time
          # -r <file>: set the date and time of this file
          touch -c -m -r "$CPPSERVER_DIR/$file" "$file"
        fi
      done
    popd

    tar czf cppserver.tar.gz cppserver
    rm -rf cppserver

    # Transfer sources and build in Docker
    docker container cp cppserver.tar.gz cppserver-$PACKAGE_NAME:/root/
    rm cppserver.tar.gz

    docker container exec cppserver-$PACKAGE_NAME /bin/bash -c 'cd /root && tar xf cppserver.tar.gz && rm cppserver.tar.gz'
    docker container exec cppserver-$PACKAGE_NAME \
      /bin/bash -c "
        set -ex
        mkdir -p /root/cppserver/_build/$PACKAGE_NAME
        pushd /root/cppserver/_build/$PACKAGE_NAME
          cmake \
            -DCMAKE_BUILD_TYPE=Release \
            -DCPPSERVER_PACKAGE_NAME=$PACKAGE_NAME \
            -DCPPSERVER_VERSION=$CPPSERVER_VERSION \
            -DCPPSERVER_COMMIT=$CPPSERVER_COMMIT \
            ../..
          if [ -n \"$VERBOSE\" ]; then
            export VERBOSE=$VERBOSE
          fi
          cmake --build . -j\$(nproc)
        popd
      "

    # 中間ファイル類を取り出す
    rm -rf $CPPSERVER_DIR/_build/$PACKAGE_NAME
    docker container cp cppserver-$PACKAGE_NAME:/root/cppserver/_build/$PACKAGE_NAME/ $CPPSERVER_DIR/_build/$PACKAGE_NAME
  popd
fi
