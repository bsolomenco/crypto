cmake_minimum_required(VERSION 3.9)
#set(CMAKE_SUPPRESS_REGENERATION true)

project(Base64)

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
    "src/Base64_tst.cpp"
    "src/Base64.hpp"
    "src/Base64.cpp"
)

#Visual Studio file filters
source_group(""    FILES ${SRC})
source_group("ref" FILES ${SRC_REF})

add_executable(Base64 ${SRC})
install(TARGETS Base64 DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/inst/")
