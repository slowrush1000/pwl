# pwl

1. build/build_setup.csh
```
#!/bin/tcsh -f

set CMAKE_C_COMPILER    = "/usr/bin/gcc"
set CMAKE_CXX_COMPILER  = "/usr/bin/g++"

\rm -rf debug release
mkdir debug release

cd debug
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} ../..
cd ..

cd release
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} ../..
cd .. 
```