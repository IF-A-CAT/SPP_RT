cmake_minimum_required(VERSION 3.4)

project(SPP_RT)


set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
add_subdirectory( ${PROJECT_SOURCE_DIR}/src)                #add dirs which has .cpp file
# set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE DEBUG CACHE STRING "Choose the type of build, options are:Debug, Release." FORCE)
endif ()
# SET(CMAKE_BUILD_TYPE "Debug")   
# SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -O0") 
# SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall") 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -Wall -fPIC -g")
message("${LIBRARY_OUTPUT_PATH}")





include_directories(${PROJECT_SOURCE_DIR}/include)          #header position

link_directories(${PROJECT_SOURCE_DIR}/lib)                 #library position
add_executable(test01 test01.cpp)
target_link_libraries(test01 timecoor decode brdcpro utils)

                              #build type- debug or release



