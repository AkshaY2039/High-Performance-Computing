#!/bin/bash

#PBS -N HW

#PBS -q default

#PBS -l nodes=6:ppn=10

#PBS -j oe

#PBS -V


cd $PBS_O_WORKDIR/

mpirun -machinefile $PBS_NODEFILE -np 60 ./HW
