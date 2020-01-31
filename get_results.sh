#!/bin/bash
make
mkdir run
cd run
for i in {1..10}
do
  mkdir $i
  cd $i
  pwd
  ../../blackjack
  cd ..
done

#python scripts/plots.py