#!/bin/bash

for ((i=1;i<=$1;i++))
do
    python3 request.py &
done
wait