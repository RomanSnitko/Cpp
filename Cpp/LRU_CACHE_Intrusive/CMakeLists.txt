cmake_minimum_required(VERSION 3.16)
project(lru_intrusive LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)

add_library(lru_intrusive INTERFACE)
target_include_directories(lru_intrusive INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(lru_intrusive INTERFACE ${Boost_INCLUDE_DIRS})

add_executable(lru_demo src/main.cpp)
target_link_libraries(lru_demo PRIVATE lru_intrusive)

#add_executable(lru_tests tests/test_lru.cpp)
#target_link_libraries(lru_tests PRIVATE lru_intrusive)
