#!/bin/bash
# Script to autogen a make file

MFILE="Makefile"
touch ${MFILE}
rm -rf ${MFILE}

echo -e "# Automatically generated Makefile\n" >> ${MFILE}

############################################################################
## First bit

CC=""
CFLAGS=""
LDFLAGS=""

source ./../GCCFLAGS

echo "CC = ${CC}" >> ${MFILE}
echo "CFLAGS = ${CFLAGS}" >> ${MFILE}
echo "LDFLAGS = ${LDFLAGS}" >> ${MFILE}


echo "" >> ${MFILE}

############################################################################
## Base/source files

## EDIT THIS################################################################
############################################################################
############################################################################
## declare an array variable
declare -a othersrc=("test")

# Get the filename which begins with exer and put them into src.
exeri=0
for f in rrr*.cpp; do
    [[ -e $f ]] || continue
    src[exeri++]="${f%.*}"
done

# put files from othersrc into src
for i in "${othersrc[@]}"
do
  src[exeri++]="$i"
done

#declare -p src   # dump the array

#declare -a src=("test"
#                "exer3-10")

## now loop through the above array
# You can access them using echo "${arr[0]}", "${arr[1]}" also
srcindex=1
for i in "${src[@]}"
do
  echo "P${srcindex}_BASE = $i" >> ${MFILE}
  srcindex=$((srcindex+1))
done

echo "" >> ${MFILE}

############################################################################
## Common Sources
############################################################################
############################################################################
############################################################################
## declare an array variable
#declare -a commonsrc=("ee.cpp" "qq.cpp")###################################
declare -a commonsrc=("./../common/rr.cpp")

COMMONSRCSTR="COMMON_SOURCES = "
for i in "${commonsrc[@]}"
do
  COMMONSRCSTR="${COMMONSRCSTR} $i"
done
echo "${COMMONSRCSTR}" >> ${MFILE}

echo "" >> ${MFILE}

############################################################################
## convert base files to  base.cpp

# First do the common objects

srcindex=1
for i in "${src[@]}"
do
  TMPSTR="P${srcindex}_SOURCES = \$(P${srcindex}_BASE).cpp"

  ## Are there any other sources?
  if [ "$i" == "exer14-7" ]
  then
    TMPSTR="$TMPSTR String.cpp"
  elif [ "$i" == "exer14-10" ]
  then
    TMPSTR="$TMPSTR StringTwo.cpp"
  elif [ "$i" == "exer17-4-5-6-7-8" ]
  then
    TMPSTR="$TMPSTR Sales_data4-5-6-7-8.cpp"

  fi

  echo "$TMPSTR" >> ${MFILE}
  srcindex=$((srcindex+1))
done

echo "" >> ${MFILE}


############################################################################
## convert source (.cpp) files to  objects .o

# first do the common objects
echo "COMMON_OBJECTS = \$(COMMON_SOURCES:.cpp=.o)" >> ${MFILE}

srcindex=1
for i in "${src[@]}"
do
  echo "P${srcindex}_OBJECTS = \$(P${srcindex}_SOURCES:.cpp=.o)" >> ${MFILE}
  srcindex=$((srcindex+1))
done

echo "" >> ${MFILE}

############################################################################
## convert executable files to  from base

srcindex=1
for i in "${src[@]}"
do
  echo "P${srcindex}_EXECUTABLE = \$(P${srcindex}_BASE)" >> ${MFILE}
  srcindex=$((srcindex+1))
done

echo "" >> ${MFILE}

############################################################################
## .PHONY

echo ".PHONY: all" >> ${MFILE}

echo "" >> ${MFILE}
############################################################################
## target all

ALLSTR="all:"
srcindex=1
for i in "${src[@]}"
do
  ALLSTR="${ALLSTR} \$(P${srcindex}_EXECUTABLE)"
  srcindex=$((srcindex+1))
done

echo "${ALLSTR}" >> ${MFILE}

echo "" >> ${MFILE}

############################################################################
## target Ps

srcindex=1
for i in "${src[@]}"
do
  echo "P${srcindex}: \$(P${srcindex}_EXECUTABLE)" >> ${MFILE}
  echo "" >> ${MFILE}
  srcindex=$((srcindex+1))
done

############################################################################
## target Executables
srcindex=1
for i in "${src[@]}"
do
  echo "\$(P${srcindex}_EXECUTABLE): \$(COMMON_OBJECTS) \$(P${srcindex}_OBJECTS)" >> ${MFILE}
  echo -e "\t\$(CC) \$(LDFLAGS) \$^ -o \$@" >> ${MFILE}
  srcindex=$((srcindex+1))
  echo "" >> ${MFILE}
done

############################################################################
## .cpp.o
echo "" >> ${MFILE}
echo "" >> ${MFILE}
echo ".cpp.o:" >> ${MFILE}
echo -e "\t\$(CC) \$(CFLAGS) \$< -o \$@" >> ${MFILE}

############################################################################
## clean

echo "" >> ${MFILE}
echo "clean:" >> ${MFILE}
CLEANSTR="\trm -rf *.o"
srcindex=1
for i in "${src[@]}"
do
  CLEANSTR="${CLEANSTR} \$(P${srcindex}_EXECUTABLE)" >> ${MFILE}
  srcindex=$((srcindex+1))
done
echo -e "${CLEANSTR}" >> ${MFILE}
