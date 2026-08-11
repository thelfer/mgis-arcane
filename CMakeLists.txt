cmake_minimum_required(VERSION 4.1)
project(MGISArcane
  LANGUAGES CXX)

include(CMakePackageConfigHelpers)

find_package(Arcane REQUIRED)
find_package(MFrontGenericInterface REQUIRED)

arccore_accelerator_enable()

add_subdirectory(src)
add_subdirectory(tests)
