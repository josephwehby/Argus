@echo off

set BUILD="GLFWBUILD"
set LIBS="libs"

echo [!] Creeating %BUILD%

cd %LIBS% 
md %BUILD%
cd %BUILD%

echo [!] Building glfw static library

cmake "../glfw"
cmake --build .
