set(target "gtest")

add_library(${target} STATIC gtest-all.cc
        ../include/ReadCmd.h
        ../include/ReadBlock.h
        ../src/ReadCmd.cpp
        ../src/ReadBlock.cpp)

if((${CMAKE_CXX_COMPILER_ID} MATCHES "GNU" OR
    ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang") AND
    (${CMAKE_SYSTEM_NAME} MATCHES "Linux"))
    set(pthread "-pthread")
endif()

target_link_libraries(${target} ${pthread})