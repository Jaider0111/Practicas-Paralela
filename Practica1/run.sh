#!/bin/bash
for i in 1 2 4 8 16 32 64 128 256 512
do
   	time ./practica1 perrito1440p.jpg perrito1440pF.jpg 3 $i
done
