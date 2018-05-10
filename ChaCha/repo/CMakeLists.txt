cmake_minimum_required(VERSION 3.9)
#set(CMAKE_SUPPRESS_REGENERATION true)

project(ChaCha)

if (UNIX)
    SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC -m64 -fvisibility=hidden")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -m64 -fvisibility=hidden -std=c++17")
endif (UNIX)

#add_definitions(-Wall)
add_definitions(-W4)

include_directories(
    "src"
)

set(SRC
    "src/ChaCha_tst.cpp"
    "src/ChaCha.hpp"
    "src/ChaCha.cpp"
)

#Visual Studio file filters
source_group(""    FILES ${SRC})
source_group("ref" FILES ${SRC_REF})

add_executable(ChaCha ${SRC})
install(TARGETS ChaCha DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/inst/")
