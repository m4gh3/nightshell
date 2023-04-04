#!/bin/sh
g++ -x c++ -S - -o- | grep "^_.*:$" | sed -e 's/:$//'
