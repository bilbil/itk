# This is the root ITK CMakeLists file.
cmake_minimum_required(VERSION 2.4)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif()


# This project is designed to be built outside the Insight source tree.
project(VolSum)

# Find ITK.
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_VERBOSE_MAKEFILE ON)

#set(CMAKE_CXX_FLAGS "-O2")
add_executable(VolSum TestFilter.cxx SummationImageFilter.h SummationImageFilter.hxx)

target_link_libraries(VolSum ${ITK_LIBRARIES})
