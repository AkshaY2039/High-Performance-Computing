#!/bin/bash

#PBS -N HW

#PBS -q default

#PBS -l nodes=5:ppn=20

#PBS -j oe

#PBS -V


cd $PBS_O_WORKDIR/

mpirun -machinefile $PBS_NODEFILE -np 100 ./HW
