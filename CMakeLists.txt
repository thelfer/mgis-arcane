cmake_minimum_required(VERSION 4.1)
project(MGISArcane
  LANGUAGES CXX)

include(CMakePackageConfigHelpers)
set(export_install_path "share/mgis-arcane/cmake")

find_package(Arccore REQUIRED)
find_package(MFrontGenericInterface REQUIRED)

arccore_accelerator_enable()

add_library(MGISArcane INTERFACE)
add_library(mgis-arcane::MGISArcane ALIAS MGISArcane)
target_sources(MGISArcane PUBLIC
  FILE_SET mgis_arcane_headers
  TYPE HEADERS
  BASE_DIRS include
  FILES 
  include/MGIS/Function/Arcane/NumArray.hxx
  include/MGIS/Function/Arcane/NumArray.ixx
  include/MGIS/Function/Arcane/Algorithms.hxx
  include/MGIS/Function/Arcane/Algorithms.ixx)
target_link_libraries(MGISArcane INTERFACE
  Arccore::arccore_common mgis::MFrontGenericInterface)

install(TARGETS MGISArcane
  EXPORT MGISArcane
  FILE_SET mgis_arcane_headers
  DESTINATION "include")
install(EXPORT MGISArcane DESTINATION ${export_install_path}
  EXPORT_LINK_INTERFACE_LIBRARIES
  NAMESPACE mgis-arcane:: FILE MGISArcaneTargets.cmake)
#if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/MGISArcaneConfig.cmake.in)
#  set(_package_config_file ${CMAKE_CURRENT_SOURCE_DIR}/MGISArcaneConfig.cmake.in)
#else()
set(_package_config_file ${CMAKE_CURRENT_BINARY_DIR}/MGISArcaneConfig.cmake.in)
file(WRITE ${_package_config_file}
  "@PACKAGE_INIT@\n"
  "\n"
  "include(\"\${CMAKE_CURRENT_LIST_DIR}/MGISArcaneTargets.cmake\")")
#endif()
# generate the config file that includes the exports
configure_package_config_file(${_package_config_file}
  "${CMAKE_CURRENT_BINARY_DIR}/MGISArcaneConfig.cmake"
  INSTALL_DESTINATION ${export_install_path}
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/MGISArcaneConfig.cmake"
  DESTINATION ${export_install_path})

add_subdirectory(tests)
