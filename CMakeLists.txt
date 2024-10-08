cmake_minimum_required(VERSION 3.22)
project(yawf)

set(CMAKE_CXX_STANDARD 23)

find_package(httplib CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE httplib::httplib)

