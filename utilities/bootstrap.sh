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

    wget "$NVM_MIRROR/$NVM_VERSION/node-$NVM_VERSION-linux-$NVM_ARCH.tar.gz"
    tar -C /usr/local --strip-components 1 -xzf node-v18.4.0-linux-$NVM_ARCH.tar.gz
    rm node-v18.4.0-linux-$NVM_ARCH.tar.gz
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
            git build-essential ccache ninja-build pkg-config \
            python3-pip python3-all-dev \
            libicu-dev aria2 libopenblas-dev wget \
            lsb mono-complete nuget

        wget -O /usr/lib/nuget/NuGet.exe https://dist.nuget.org/win-x86-commandline/v4.9.6/nuget.exe

        install_nodejs

        update-ca-certificates -f

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

        hash cmake 2>/dev/null || { pip3 install -i https://mirrors.aliyun.com/pypi/simple cmake; }

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