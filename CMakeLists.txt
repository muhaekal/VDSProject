cmake_minimum_required(VERSION 3.10)
project(VDSProject CXX C)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fPIC -std=c++17 ${LLVM_CXX_FLAGS} ")


# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in ${CMAKE_SOURCE_DIR}/gtest/googletest-download/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .

        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/gtest/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
       WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/gtest/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.


add_subdirectory(${CMAKE_SOURCE_DIR}/gtest/googletest-src
        ${CMAKE_SOURCE_DIR}/gtest/googletest-build
        EXCLUDE_FROM_ALL)

if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

#if(NOT TARGET Boost)
    #find_package(Boost REQUIRED)
#endif()

#Include dirs
include_directories(${CMAKE_SOURCE_DIR}/lib)
link_directories(${CMAKE_SOURCE_DIR}/lib)

include_directories(${CMAKE_SOURCE_DIR}/src)
link_directories(${CMAKE_SOURCE_DIR}/src)

include_directories(${CMAKE_SOURCE_DIR}/src/test/)
link_directories(${CMAKE_SOURCE_DIR}/src/test/)

include_directories(${CMAKE_SOURCE_DIR}/src/bench/)
link_directories(${CMAKE_SOURCE_DIR}/src/bench/)

include_directories(${CMAKE_SOURCE_DIR}/src/verify/)
link_directories(${CMAKE_SOURCE_DIR}/src/verify/)

include_directories(${CMAKE_SOURCE_DIR}/src/reachability/)
link_directories(${CMAKE_SOURCE_DIR}/src/reachability/)

include_directories(/Users/mhaekal/Downloads/boost_1_84_0)
link_directories(Users/mhaekal/Downloads/boost_1_84_0)

add_subdirectory(src)
add_executable(VDSProject src/main.cpp src/reachability/Reachability.cpp
        src/reachability/ReachabilityInterface.h)
target_link_libraries(VDSProject Manager)

enable_testing()

add_executable(main_test src/test/main_test.cpp src/Manager.cpp src/reachability/Reachability.cpp)
add_executable(benchmark src/bench/main_bench.cpp src/bench/BenchmarkLib.cpp src/bench/BenchParser.cpp src/bench/CircuitToBDD.cpp src/Manager.cpp)
add_executable(verify src/verify/main_verify.cpp)
#add_executable(reachability src/reachability/main_test.cpp src/reachability/Reachability.cpp
 #       src/reachability/ReachabilityInterface.h)


target_link_libraries(VDSProject Manager ${Boost_LIBRARIES})
target_link_libraries(main_test GTest::gtest_main)
#target_link_libraries(reachability Manager Reachability gtest gtest_main pthread)

include(GoogleTest)
gtest_discover_tests(main_test)

