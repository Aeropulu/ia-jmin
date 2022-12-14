# CMakeList.txt : projet CMake pour Test, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)
project(MyBot)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -Wall -pedantic")

include_directories(${CMAKE_SOURCE_DIR}/hlt)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)

foreach(dir ${dirs})
    file(GLOB_RECURSE SOURCE ${dir}/*.[ch]*)
    set(SOURCE_FILES "${SOURCE_FILES}" ${SOURCE})
endforeach()

include_directories(${CMAKE_SOURCE_DIR})
set(SOURCE_FILES "${SOURCE_FILES}" MyBot.cpp)

add_executable(MyBot ${SOURCE_FILES}  "ShipBlackboard.h" "ShipBlackboard.cpp"  "BehaviorTree.h" "BehaviorTree.cpp" "ShipBehavior.h" "ShipBehavior.cpp" "ShipDestination.h" "ShipDestination.cpp" "ShipFunctions.h" "ShipFunctions.cpp")

if(MINGW)
    target_link_libraries(MyBot -static)
endif()