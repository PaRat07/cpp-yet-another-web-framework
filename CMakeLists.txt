cmake_minimum_required(VERSION 3.8)
set(CMAKE_TOOLCHAIN_FILE /opt/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(VCPKG_OVERLAY_TRIPLETS ${CMAKE_CURRENT_LIST_DIR})
set(VCPKG_TARGET_TRIPLET linux-clang-triplet)
set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/clang-toolchain.cmake)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2c -stdlib=libc++ -fuse-ld=lld")
set(CLANG_DEFAULT_CXX_STDLIB libc++)
set(CLANG_DEFAULT_RTLIB compiler-rt)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
set(CMAKE_LINKER_TYPE LLD)
project(yawf)


find_package(boost_json REQUIRED CONFIG)
find_package(httplib CONFIG REQUIRED)
find_package(boost_pfr REQUIRED CONFIG)

add_executable(${PROJECT_NAME} main.cpp
        src/zserver.cpp
        src/json2obj.cpp
        src/obj2json.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE httplib::httplib Boost::pfr Boost::json)

