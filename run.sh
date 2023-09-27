#!/usr/bin/bash

for i in {1..1};
do
    ./lemipc 0&
    ./lemipc 1&
    ./lemipc 2&
    ./lemipc 3&
    ./lemipc 4&
    # ./lemipc 3&
    # ./lemipc 4&
done
./lemipc