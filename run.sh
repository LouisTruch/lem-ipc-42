#!/usr/bin/bash

nb_player=3
for i in {1..${nb_player}};
do
    ./lemipc 1&
done
for i in {1..${nb_player}};
do
    ./lemipc 2&
done
for i in {1..${nb_player}};
do
    ./lemipc 3&
done
./lemipc 1