cmake_minimum_required(VERSION 2.8.3)
project(twist_adapter)

add_compile_options(-std=c++11)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  geometry_msgs
  prius_msgs
)

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS roscpp std_msgs geometry_msgs prius_msgs
  
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(twist_adapter src/twist_adapter.cpp)
target_link_libraries(twist_adapter ${catkin_LIBRARIES})
add_dependencies(twist_adapter ${catkin_EXPORTED_TARGETS})
