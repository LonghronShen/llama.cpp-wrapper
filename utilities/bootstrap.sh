#!/bin/bash

set -x

SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

retry() {
    local -r -i max_attempts="$1"; shift
    local -i attempt_num=1
    until "$@"
    do
        if ((attempt_num==max_attempts))
        then
            echo "Attempt $attempt_num failed and there are no more attempts left!"
            return 1
        else
            echo "Attempt $attempt_num failed! Trying again in $attempt_num seconds..."
            sleep $((attempt_num++))
        fi
    done
}

install_nodejs() {
    local NVM_ARCH
    local NVM_MIRROR="https://nodejs.org/dist"
    local NVM_VERSION="v18.4.0"
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

    local FILE_NAME="node-$NVM_VERSION-linux-$NVM_ARCH.tar.gz"
    local URL="$NVM_MIRROR/$NVM_VERSION/$FILE_NAME"

    wget "$URL"
    tar -C /usr/local --strip-components 1 -xzf "$FILE_NAME"
    rm "$FILE_NAME"
}

unameOut="$(uname -s)"
arch="x64"
case "${unameOut}" in
    Linux*)
        machine=linux

        export DEBIAN_FRONTEND=noninteractive
        export TZ=UTC

        apt clean
        apt update
        retry 10 apt install -y apt-transport-https ca-certificates \
            git build-essential gcc-8 g++-8 ccache ninja-build pkg-config \
            python3-pip python3-all-dev \
            libicu-dev aria2 libopenblas-dev wget \
            lsb mono-complete nuget

        update-alternatives --remove-all gcc
        update-alternatives --remove-all g++

        update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 10
        update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-8 10
        update-alternatives --install /usr/bin/cc cc /usr/bin/gcc 30
        update-alternatives --set cc /usr/bin/gcc
        update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++ 30
        update-alternatives --set c++ /usr/bin/g++

        wget -O /usr/lib/nuget/NuGet.exe https://dist.nuget.org/win-x86-commandline/v4.9.6/nuget.exe

        install_nodejs

        boost_version="$(apt-cache madison libboost-all-dev | grep -oP "\d+(\.\d+)+")"
        bash "$SCRIPTPATH/vercmp.sh" "$boost_version" "1.66"

        if [[ $? -ne 2 ]]; then
            retry 10 apt install -y libboost-all-dev
        else
            retry 10 apt install -y software-properties-common
            add-apt-repository ppa:mhier/libboost-latest -y
            apt update
            retry 10 apt install -y libboost1.68-dev
        fi

        hash cmake 2>/dev/null || {
            export PIP_ONLY_BINARY=cmake
            python3 -m pip install --upgrade pip
            python3 -m pip install cmake || {
                echo "Build CMake from source ..."
                cd /tmp
                git clone -b 'v3.25.1' --single-branch --depth 1 https://github.com/Kitware/CMake.git CMake
                cd CMake
                ./bootstrap --prefix=/usr/local
                make -j$(nproc)
                make install
                cd ..
                rm -rf CMake
            }
        }

        lsb_release -a
        mono -V
        node -v
        npm -v
        cmake --version
        ;;
    Darwin*)
        machine=osx
        hash sudo 2>/dev/null || { echo >&2 "Plesse ensure you have executed 'xcode-select --install' to have build tools ready."; exit; }
        brew install boost@1.67 icu4c
        ;;
    CYGWIN*)        machine=win;;
    MINGW32_NT)
        machine=win
        arch=x86
        ;;
    MINGW64_NT)
        machine=win
        ;;
    *)
        machine="UNKNOWN:${unameOut}"
        echo "Not supported platform: ${machine}"
        exit 1
esac