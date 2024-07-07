#!/bin/env bash

for i in {0..700}
do
    O=$(echo -e "read\n$i\nquit" | ./level07)
    if [ $? -ne 0 ]; then
        break
    fi
    NL=$(echo "$O" | grep "Number")
    NB=$(echo "$NL" | awk '{print $NF}')
    if [ $NB -ne 0 ]; then
        echo "a[ $i ] -> $NB -> 0x$(echo "obase=16; $NB" | bc)"
    fi
done
