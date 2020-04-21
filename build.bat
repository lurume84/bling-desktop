cd build
conan install .. --profile ../profiles/ProfileWin32MT --build fmt --build spdlog
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release