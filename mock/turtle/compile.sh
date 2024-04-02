BUILD_DIR="$HOME/devel/upnplib-dev/upnplib_project/build"
g++ -std=c++20 -pedantic-errors -Wall -I$BUILD_DIR/_deps/googletest-src/googletest/include -I$BUILD_DIR/_deps/googletest-src/googlemock/include test_turtle.cpp -L$BUILD_DIR/lib -lgtest -lgmock

echo 'run with: LD_LIBRARY_PATH="'$BUILD_DIR'/lib" ./a.out'
