#!/bin/sh

printf "\nMaking all tests...\n"
cd "$(dirname "$0")"

gcc ./*.c -o a.out

rm ./*.txt

./a.out

rm a.out