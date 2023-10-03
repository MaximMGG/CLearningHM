@echo off

mkdir ..\..\build
pushd ..\..\build
gcc c:\users\123\desktop\cprojects\handmade\code\win32_handmade.cpp --libs="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\um\x86"
popd
