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
            libicu-dev aria2 libopenblas-dev \
            lsb mono-complete nuget nodejs npm

        mono -V
        node -v
        npm -v

        bash "$SCRIPTPATH/vercmp.sh" "$(npm -v)" "6.14.0"
        if [[ $? -ne 2 ]]; then
            npm install -g npm@latest-6
            npm -v
        fi

        update-ca-certificates -f

        boost_version="$(apt-cache madison libboost-all-dev | grep -oP "\d+(\.\d+)+")"
        vercmp "$boost_version" "1.66"

        if [[ $? -ne 2 ]]; then
            retry 10 apt install -y libboost-all-dev
        else
            retry 10 apt install -y software-properties-common
            add-apt-repository ppa:mhier/libboost-latest -y
            apt update
            retry 10 apt install -y libboost1.68-dev
        fi

        lsb_release -a

        hash cmake 2>/dev/null || { pip3 install -i https://mirrors.aliyun.com/pypi/simple cmake; }
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