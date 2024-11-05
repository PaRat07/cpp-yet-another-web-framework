cmake_minimum_required(VERSION 3.8)
set(CMAKE_TOOLCHAIN_FILE /opt/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(VCPKG_OVERLAY_TRIPLETS ${CMAKE_CURRENT_LIST_DIR})
set(VCPKG_TARGET_TRIPLET linux-clang-triplet)
set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/clang-toolchain.cmake)
include(clang-toolchain.cmake)
project(yawf)


find_package(simdjson CONFIG REQUIRED)
find_package(httplib CONFIG REQUIRED)
find_package(boost_pfr REQUIRED CONFIG)
find_package(boost_json REQUIRED CONFIG)

add_executable(${PROJECT_NAME} main.cpp
        src/zserver.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE httplib::httplib Boost::pfr simdjson::simdjson Boost::json)

