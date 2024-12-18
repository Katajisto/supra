#!/bin/bash

OS_NAME=`uname -o 2>/dev/null || uname -s`

if [ $OS_NAME == "Msys" ]; then
    GLFLAG="-lopengl32"
elif [ $OS_NAME == "Darwin" ]; then
    GLFLAG="-framework OpenGL"
else
    GLFLAG="-lGL"
fi

CFLAGS="-I../src -Wall -std=c11 -pedantic `pkg-config sdl3 --cflags --libs`  $GLFLAG -lm -O3 -g"

gcc *.c $CFLAGS
