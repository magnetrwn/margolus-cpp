#!/usr/bin/sh
set -e

cd build
[ -f margolus ] && rm margolus
cmake ..
make
rm -r CMakeFiles cmake_install.cmake CMakeCache.txt Makefile 2> /dev/null
zip -r "margolus.zip" .
[ -f ../margolus.zip ] && mv ../margolus.zip ../margolus.zip.old
mv margolus.zip ../margolus.zip

echo
printf "[Done] \x1B[96mA build zip \"margolus.zip\" has been saved in project root, and an executable is ready in \"build/\".\x1B[0m\n"

#sh -c 'cd build; ./margolus'
