@echo off

mkdir ..\..\build
pushd ..\..\build
gcc c:\users\123\desktop\cprojects\handmade\code\win32_handmade.cpp C:\msys64\ucrt64\lib\libgdi32.a
popd
