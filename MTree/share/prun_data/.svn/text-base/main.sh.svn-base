#!/bin/bash
cd ../
export MTreeDir=${PWD}
cd -
#curDir=${PWD}
#workDir=${curDir}/prun_data
#mv input.txt ${workDir}
#cd ${workDir}
export LD_LIBRARY_PATH=${PWD}:${LD_LIBRARY_PATH}
export HOME=${PWD}:${HOME}
echo "| Compiling ConvertD3PDToMTree."
make prelim
./prelim $1
make
echo "| Done with ConvertD3PDToMTree compilation."
./main $1
#cd ${curDir}
