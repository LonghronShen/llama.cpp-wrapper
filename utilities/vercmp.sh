#!/bin/bash

set -x

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

vercmp "$1" "$2"
