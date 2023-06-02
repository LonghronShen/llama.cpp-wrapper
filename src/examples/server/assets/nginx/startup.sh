#!/bin/bash

set -x

export NGINX_BACKEND_HOST=$BACKEND_HOST

cd /etc/nginx/conf.d
smalte build --scope NGINX\.* \
    nginx.conf.template:default.conf

cat default.conf
