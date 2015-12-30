@echo off
if %1$==$ goto help
g++ -std=c++1z -fmax-errors=5 -I../stl2/include %*
goto done

:help
echo Build using g++
echo Invoke: build [gcc-option...] path...
:done

