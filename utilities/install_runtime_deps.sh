#!/bin/bash

set -x

retry() {
    local -r -i max_attempts="$1"
    shift
    local -i attempt_num=1
    until "$@"; do
        if ((attempt_num == max_attempts)); then
            echo "Attempt $attempt_num failed and there are no more attempts left!"
            return 1
        else
            echo "Attempt $attempt_num failed! Trying again in $attempt_num seconds..."
            sleep $((attempt_num++))
        fi
    done
}

vercmp() {
    if [[ $1 == "$2" ]]; then
        echo "0"
        return 0
    fi
    local IFS=.
    local i ver1=($1) ver2=($2)
    # fill empty fields in ver1 with zeros
    for ((i = ${#ver1[@]}; i < ${#ver2[@]}; i++)); do
        ver1[i]=0
    done
    for ((i = 0; i < ${#ver1[@]}; i++)); do
        if [[ -z ${ver2[i]} ]]; then
            # fill empty fields in ver2 with zeros
            ver2[i]=0
        fi
        if ((10#${ver1[i]} > 10#${ver2[i]})); then
            echo "1"
            return 0
        fi
        if ((10#${ver1[i]} < 10#${ver2[i]})); then
            echo "2"
            return 0
        fi
    done
    echo "0"
    return 0
}

install_boost() {
    local BOOST_VERSION
    BOOST_VERSION="$(apt-cache madison libboost-all-dev | grep -oP "\d+(\.\d+)+")"
    BOOST_VERSION_CMP="$(vercmp "$BOOST_VERSION" "1.63")"
    if [[ $BOOST_VERSION_CMP -ne 2 ]]; then
        retry 10 apt install -y libboost-all-dev
    else
        retry 10 apt install -y software-properties-common
        add-apt-repository ppa:mhier/libboost-latest -y
        apt update
        retry 10 apt install -y libboost1.68-dev
    fi
}

RUNTIME_DEPS=$(ldd ./llama-cpp-wrapper-server-exe)

apt update

if echo "$RUNTIME_DEPS" | grep -q "boost"; then
    install_boost
fi

if echo "$RUNTIME_DEPS" | grep -q "openblas"; then
    retry 10 apt install -y libopenblas-base
fi

LIB_GFORTRAN_VERSION="$(grep -Po "(?<=libgfortran\.so\.)(\d+)(?= =>)")"
if echo "$RUNTIME_DEPS" | grep -q "libgfortran"; then
    retry 10 apt install -y "libgfortran$LIB_GFORTRAN_VERSION"
fi

apt clean
