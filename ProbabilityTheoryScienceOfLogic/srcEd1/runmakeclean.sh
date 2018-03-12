#!/bin/bash

## declare an array variable
declare -a userdir=("ch01GettingStarted"
"ch02VariablesAndBasicTypes"
"ch03StringsVectorsAndArrays"
"ch04Expressions"
"ch05Statements"
"ch06Functions"
"ch07Classes"
"ch08TheIOLibrary"
"ch09SequentialContainers"
"ch10GenericAlgorithms"
"ch11AssociativeContainers"
"ch12DynamicMemory"
"ch13CopyControl"
"ch14OverloadedOperationsAndConversions"
"ch15ObjectOrientatedProgramming"
"ch16TemplatesAndGenericProgramming"
"ch17SpecializedLibraryFacilities"
"ch18ToolsForLargePrograms"
"ch19SpecializedToolsAndTechniques")

for i in "${userdir[@]}"
do
  cd ${i}
  if [ -f Makefile ]; then
    make clean
    rm -rf Makefile
  fi
  cd ..
done
