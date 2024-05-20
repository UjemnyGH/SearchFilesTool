#!/bin/bash
g++ -m64 -Ofast -Os -Wall -Wextra -Wpedantic -Werror -std=c++2b -o searchfiles search_files.cpp
x86_64-w64-mingw32-g++ -m64 -Ofast -Os -Wall -Wextra -Wpedantic -Werror -std=c++2b -o searchfiles search_files.cpp