#!/usr/bin/bash

for i in {1..2};
do
    ./lemipc 1&
    ./lemipc 2&
    ./lemipc 3&
    # ./lemipc 4&i
    # ./lemipc 5&
    # ./lemipc 6&
    # ./lemipc 7&
    # ./lemipc 8&
    # ./lemipc 9&
done
sleep 0.5
./lemipc-visualizer
# ./lemipc