#!/usr/bin/bash

for i in {1..2};
do
    sleep 0.1
    ./lemipc 1&
    ./lemipc 2&
    # ./lemipc 5&
    # ./lemipc 3&
    # ./lemipc 4&
done
./lemipc