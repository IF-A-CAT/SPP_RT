cmake_minimum_required(VERSION 3.4)

add_library(matrix SHARED "./Matrix.cpp")
add_library(timecoor SHARED "./TimeCoor.cpp")
add_library(srif SHARED "./SrifPro.cpp")
add_library(decode  SHARED "./Decode.cpp")
add_library(brdcpro  SHARED "./BrdcPro.cpp")
add_library(utils  SHARED "./Utilities.cpp")

target_link_libraries(decode timecoor brdcpro)
target_link_libraries(srif matrix)
target_link_libraries(brdcpro matrix timecoor )
target_link_libraries(utils decode timecoor)

