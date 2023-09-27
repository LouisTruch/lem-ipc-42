#!/usr/bin/bash

for i in {1..3};
do
    ./lemipc 1&
    ./lemipc 2&
    # ./lemipc 3&
    # ./lemipc 4&
    # ./lemipc 5&
    # ./lemipc 3&
    # ./lemipc 4&
done
./lemipc