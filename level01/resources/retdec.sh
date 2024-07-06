#!/usr/bin/env bash

if [ ! -z "$1" ]; then
    abs_path=$(realpath "$1")
    docker run -it --user root -v "$abs_path":/mnt/binary remnux/retdec /bin/bash -c 'retdec-decompiler.py /mnt/binary -o /tmp/source 1>/dev/null && cat /tmp/source'
else
    echo "Usage: $0 <path_to_binary>"
fi
