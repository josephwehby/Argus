@echo off
set BUILD="build"

mkdir %BUILD%
cd %BUILD%

cmake ..
cmake --build .

cd "Debug"
App.exe
