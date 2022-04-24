#!/bin/bash
if [ -z "$1" ]
then
    echo "Usage: ./run.sh <threads_number>"
	exit 1
fi

for i in {1..12}
do
	echo ./image-filter perrito720.jpg "'720 filtrado/perrito720F$i.jpg'" $i $1
   	time ./image-filter perrito720.jpg "720 filtrado/perrito720F$i.jpg" $i $1
done
for i in {1..12}
do
	echo ./image-filter perrito1080.jpg "'1080 filtrado/perrito1080F$i.jpg'" $i $1
   	time ./image-filter perrito1080.jpg "1080 filtrado/perrito1080F$i.jpg" $i $1
done
for i in {1..12}
do
	echo ./image-filter perrito4K.jpg "'4k filtrado/perrito4KF$i.jpg'" $i $1
   	time ./image-filter perrito4K.jpg "4k filtrado/perrito4KF$i.jpg" $i $1
done