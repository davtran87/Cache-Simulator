
To run the program for system 1
1. first access dtran7 unixlab directory.

2. compilation: g++ sys1.cpp -o sys1

3. commands:./sys1 pathtofile CacheSize(KB) [-v ic1 ic2] // -v verbose mode is optional but must include ic1 and ic2

Examples
./sys1 ~whsu/csc656/Traces/MiBench/patricia.xex 1
./sys1 ~whsu/csc656/Traces/MiBench/mad.xex 1
./sys1 ~whsu/csc656/Traces/MiBench/patricia.xex 1 -v 0 100000 // for printing verbose mode must have bounds
./sys1 ~whsu/csc656/Traces/MiBench/mad.xex 1 -v 0 100000 // for printing verbose mode must have bounds
./sys1 ~whsu/csc656/Traces/MiBench/patricia.xex 2
./sys1 ~whsu/csc656/Traces/MiBench/mad.xex 2
./sys1 ~whsu/csc656/Traces/MiBench/patricia.xex 2 -v 0 100000 // for printing verbose mode must have bounds
./sys1 ~whsu/csc656/Traces/MiBench/mad.xex 2 -v 0 100000 // for printing verbose mode must have bounds

To run the program for system 2
1. first access dtran7 unixlab directory.

2. compilation: g++ sys2.cpp -o sys2

3. commands:./sys2 pathtofile CacheSize(KB) KwayNumber [-v ic1 ic2] // -v verbose mode is optional but must include ic1 and ic2

Execution method: [dtran7@unixlab ~]$ ./sys2 ~whsu/csc656/Traces/MiBench/patricia.xex  2 2 -v 0 100000 // Optional for printing verbose mode must have bounds
                  [dtran7@unixlab ~]$ ./sys2 ~whsu/csc656/Traces/MiBench/patricia.xex  1 2
                  [dtran7@unixlab ~]$ ./sys2 ~whsu/csc656/Traces/MiBench/patricia.xex  1 4
                  [dtran7@unixlab ~]$ ./sys2 ~whsu/csc656/Traces/MiBench/patricia.xex  2 4
