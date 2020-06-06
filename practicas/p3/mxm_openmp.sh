#! /bin/bash
#
gcc -c -Wall -fopenmp mxm_openmp.c
if [ $? -ne 0 ]; then
  echo "Compile error."
  exit
fi
#
gcc -fopenmp mxm_openmp.o -lm
if [ $? -ne 0 ]; then
  echo "Load error."
  exit
fi
rm mxm_openmp.o
mv a.out mxm_openmp
#
echo "Run with 1 thread."
export OMP_NUM_THREADS=1
./mxm_openmp > mxm_openmp.txt
if [ $? -ne 0 ]; then
  echo "Run error."
  exit
fi
#
echo "Run with 2 threads."
export OMP_NUM_THREADS=2
./mxm_openmp >> mxm_openmp.txt
if [ $? -ne 0 ]; then
  echo "Run error."
  exit
fi
#
echo "Run with 4 threads."
export OMP_NUM_THREADS=4
./mxm_openmp >> mxm_openmp.txt
if [ $? -ne 0 ]; then
  echo "Run error."
  exit
fi
#
echo "Run with 8 threads."
export OMP_NUM_THREADS=8
./mxm_openmp >> mxm_openmp.txt
if [ $? -ne 0 ]; then
  echo "Run error."
  exit
fi
rm mxm_openmp
#
echo "Normal end of execution."
