#!/bin/bash

#PBS -N HW

#PBS -q default

#PBS -l nodes=8:ppn=10

#PBS -j oe

#PBS -V


cd $PBS_O_WORKDIR/

mpirun -machinefile $PBS_NODEFILE -np 80 ./HW
