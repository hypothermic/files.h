#!/bin/sh

printf "\nMaking all tests...\n"
cd "$(dirname "$0")"

gcc ./*.c -o a.out

./a.out

rm a.out