#!/bin/bash


if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters"
    exit 42
fi

# Usage: run from one of the chapter subdirectories.
#        ./run.sh test
# 
# This script will look at the driver.sh file to see if test_fun exists, if
# it does, it will run the function to drive the code test. If not, it will
# run test as it is.

# Program name
PROG="$1"

# Name for driver function (if it exists)
FUNC="${PROG}_fun"

# if the driver function exists, it would be in here.
source drivers.sh

# Generate the Makefile if it does not exist.
#if [ ! -f Makefile ]; then
#  ./genmake.sh
#fi

# make clean to get rid of unnecessary executables.
# we run make clean before regenerating the Makefile, in case the newly
# generated makefile does not contain rules to get rid of previous
# executables.
#make clean > /dev/null && \
./genmake.sh


#OUTPUT="$(type -t ${FUNC})"

# if ${} 
if [ "$(type -t ${FUNC})" = "function" ]
then
  # We run the function
  make ${PROG} && \
  ${FUNC}
else
  # There is no function for this program, so we run the program
  # directly.
  make ${PROG} && \
  ./${PROG}
fi

#make clean > /dev/null
