#!/bin/bash

set -x

./build/bin/llama-cpp-wrapper-exe \
    -d true \
    -c 2048 \
    --temp 0.7 \
    --top_k 40 \
    --top_p 0.5 \
    --repeat_last_n 256 \
    --batch_size 1024 \
    --repeat_penalty 1.17647 \
    -m ./models/ggml-vic13b-uncensored-q5_1.bin \
    -f ./prompts/bob.txt
