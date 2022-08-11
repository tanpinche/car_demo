cmake_minimum_required(VERSION 2.8.3)
project(kopilot_launch)

add_compile_options(-std=c++11)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS
)

catkin_package(
  INCLUDE_DIRS launch
  CATKIN_DEPENDS 
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)
