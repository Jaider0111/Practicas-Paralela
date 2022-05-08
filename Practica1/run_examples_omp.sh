#!/bin/bash
if [ -z "$1" ]
then
    echo "Usage: ./run.sh <threads_number>  <path_to_output>"
	exit 1
fi
if [ -z "$2" ]
then
	echo "Usage: ./run.sh <threads_number>  <path_to_output>"
	exit 1
fi

echo "Running with $1 threads" > $2

for i in {1..12}
do
	echo -e "\n\n"./image-filter-omp perrito720.jpg "'720 filtrado/perrito720F$i.jpg'" $i $1 >> $2
   	{ time ./image-filter-omp perrito720.jpg "720 filtrado/perrito720F$i.jpg" $i $1; } 2>> $2
done

for i in {1..12}
do
	echo -e  "\n\n"./image-filter-omp perrito1080.jpg "'1080 filtrado/perrito1080F$i.jpg'" $i $1 >> $2
   	{ time ./image-filter-omp perrito1080.jpg "1080 filtrado/perrito1080F$i.jpg" $i $1; } 2>> $2
done

for i in {1..12}
do
	echo -e  "\n\n"./image-filter-omp perrito4K.jpg "'4k filtrado/perrito4KF$i.jpg'" $i $1 >> $2
   	{ time ./image-filter-omp perrito4K.jpg "4k filtrado/perrito4KF$i.jpg" $i $1; } 2>> $2
done
