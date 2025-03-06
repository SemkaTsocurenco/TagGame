#! /bin/sh
mkdir build 
cd build 
cmake ../ 
make -j16 
./TagGame
cd ../
