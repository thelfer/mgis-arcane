cmake_minimum_required(VERSION 4.1)
project(MGISArcane
  LANGUAGES CXX)

find_package(Arccore REQUIRED)
find_package(MFrontGenericInterface REQUIRED)

add_library(MGISArcane INTERFACE)
target_sources(MGISArcane PUBLIC
  FILE_SET mgis_arcane_headers
  TYPE HEADERS
  BASE_DIRS include
  FILES 
  include/MGIS/Function/Arcane/NumArray.hxx
  include/MGIS/Function/Arcane/NumArray.ixx)
target_link_libraries(MGISArcane INTERFACE
  Arccore::arccore_common mgis::MFrontGenericInterface)

install(TARGETS MGISArcane
  FILE_SET mgis_arcane_headers
  DESTINATION "include")
add_subdirectory(tests)
