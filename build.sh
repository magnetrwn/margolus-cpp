#!/usr/bin/sh
set -e

cd build
[ -f cpp-margolus ] && rm cpp-margolus
cmake ..
make
rm -r CMakeFiles cmake_install.cmake CMakeCache.txt Makefile 2> /dev/null
zip -r "cpp-margolus.zip" .
[ -f ../cpp-margolus.zip ] && mv ../cpp-margolus.zip ../cpp-margolus.zip.old
mv cpp-margolus.zip ../cpp-margolus.zip

echo
echo "\x1B[96mA build zip \"cpp-margolus.zip\" has been saved in project root, and an executable is ready in \"build/\".\x1B[0m"