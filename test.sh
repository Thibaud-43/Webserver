#!/bin/bash

for N in {1..50}
do
    python3 request.py &
done
wait