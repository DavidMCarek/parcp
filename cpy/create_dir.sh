#!/bin/bash

for i in `seq 1 100`; do
    mkdir dir$i
    cd dir$i
    for k in `seq 1 100`; do
        for n in `seq 1 100`; do
            echo "Long file" >> ./file$k
        done
    done
done

