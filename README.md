# pwl

## build/build_setup.csh
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

## pwl::PWL::readFile(const std::string& fileName), writeFile(const std::string& fileName) file format

> index1 time1 value1

> index2 time2 value2

> index3 time3 value3

> ...

> indexN timeN valueN

## pwl test reference file

1. reference file path(pwl file)
    - data/i1vin.txt
    - data/vout.txt
2. reference result
    - i1vin_avg           =  -2.678997e-11 from=  5.050000e-08 to=  5.150050e-08
    - i1vin_min           =  -5.982921e-02 at=  5.109950e-08
    - i1vin_max           =   5.965893e-02 at=  5.119950e-08
    - i1vin_pp            =   1.194881e-01 from=  5.050000e-08 to=  5.150000e-08
    - i1vin_rms           =   2.31769e-02  from=  5.05000e-08 to=  5.15000e-08
    - vout_avg            =  1.001001e-01 from=  5.050000e-08 to=  5.150050e-08
    - vout_min            =  2.431209e-248 at=  5.099950e-08
    - vout_max            =  8.801812e-01 at=  5.111350e-08
    - vout_pp             =  8.801812e-01 from=  5.050000e-08 to=  5.150000e-08
    - vout_rms            =   2.49450e-01 from=  5.05000e-08 to=  5.15000e-08
