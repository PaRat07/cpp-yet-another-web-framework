cmake_minimum_required(VERSION 3.22)
project(yawf)

set(CMAKE_CXX_STANDARD 23)

find_package(boost_json REQUIRED CONFIG)
find_package(httplib CONFIG REQUIRED)
find_package(boost_pfr REQUIRED CONFIG)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE httplib::httplib Boost::pfr Boost::json)

