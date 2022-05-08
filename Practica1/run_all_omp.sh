#!/bin/bash
if [ -z "$1" ]
then
    echo "Usage: ./run_all.sh <path_to_output>"
	exit 1
fi

echo "Running with all" > $1

for i in 1 2 4 6 8 10 12 14 16
do
	echo -e "\n\n"./image-filter-omp perrito720.jpg "'720 filtrado/perrito720F1.jpg'" 1 $i >> $1
   	{ time ./image-filter-omp perrito720.jpg "720 filtrado/perrito720F1.jpg" 1 $i; } 2>> $1
done

for i in 1 2 4 6 8 10 12 14 16
do
	echo -e  "\n\n"./image-filter-omp perrito1080.jpg "'1080 filtrado/perrito1080F1.jpg'" 1 $i >> $1
   	{ time ./image-filter-omp perrito1080.jpg "1080 filtrado/perrito1080F1.jpg" 1 $i; } 2>> $1
done

for i in 1 2 4 6 8 10 12 14 16
do
	echo -e  "\n\n"./image-filter-omp perrito4K.jpg "'4k filtrado/perrito4KF1.jpg'" 1 $i >> $1
   	{ time ./image-filter-omp perrito4K.jpg "4k filtrado/perrito4KF1.jpg" 1 $i; } 2>> $1
done
