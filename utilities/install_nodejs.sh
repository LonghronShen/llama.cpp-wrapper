#!/bin/bash

set -x

NVM_ARCH=""
NVM_MIRROR="https://nodejs.org/dist"
NVM_VERSION="v14.20.0"
HOST_ARCH=$(dpkg --print-architecture)

case "${HOST_ARCH}" in
i*86)
    NVM_ARCH="x86"
    NVM_MIRROR="https://unofficial-builds.nodejs.org/download/release"
    ;;
x86_64 | amd64)
    NVM_ARCH="x64"
    ;;
aarch64 | armv8l)
    NVM_ARCH="arm64"
    ;;
*)
    NVM_ARCH="${HOST_ARCH}"
    ;;
esac

FILE_NAME="node-$NVM_VERSION-linux-$NVM_ARCH.tar.gz"
URL="$NVM_MIRROR/$NVM_VERSION/$FILE_NAME"

wget "$URL"
tar -C /usr/local --strip-components 1 -xzf "$FILE_NAME"
rm "$FILE_NAME"
