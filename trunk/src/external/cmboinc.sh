rm -f boinc/CMakeLists.txt
rm -f boinc/api/CMakeLists.txt
rm -f boinc/lib/CMakeLists.txt
ln -s `pwd`/boinc_cmake/CMakeLists.txt boinc/CMakeLists.txt
ln -s `pwd`/boinc_cmake/api/CMakeLists.txt boinc/api/CMakeLists.txt
ln -s `pwd`/boinc_cmake/lib/CMakeLists.txt boinc/lib/CMakeLists.txt
