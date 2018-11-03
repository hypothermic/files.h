#!/bin/bash

printf "\nMaking all tests...\n"
cd "$(dirname "$0")"

if [[ "$OSTYPE" == "bsd"* ]]; then
    gcc ./*.c -o a.out
else
    if ! pkg-config --cflags --libs libudev; then
        echo "Dependency not found: libudev"
        exit 1
    fi
    gcc ./*.c -o a.out -ludev
fi

rm ./*.txt

./a.out

rm a.out