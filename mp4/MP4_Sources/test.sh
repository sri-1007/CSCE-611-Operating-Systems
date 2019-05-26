#! /bin/bash

find -type f -exec touch {} +
make clean
make
./copykernel.sh
bochs -f bochsrc.bxrc
