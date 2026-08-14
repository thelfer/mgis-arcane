cmake_minimum_required(VERSION 4.1)
project(MGISArcane
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

# enable tests
option(enable-testing "" ON)

include(CMakePackageConfigHelpers)

find_package(Arcane REQUIRED)
find_package(MFrontGenericInterface REQUIRED)

if(NOT MGIS_FUNCTION_SUPPORT)
  message(FATAL_ERROR "support for MGIS/Function has not been enabled in MGIS")
endif(NOT MGIS_FUNCTION_SUPPORT)

if(NOT MGIS_HAVE_TFEL)
  message(FATAL_ERROR "TFEL support has not been enabled in MGIS")
endif(NOT MGIS_HAVE_TFEL)

set(MGIS_HAVE_TFEL @MGIS_HAVE_TFEL@)


find_package(TFELTests)

arccore_accelerator_enable()

if(enable-testing)
  # testing
  set(CTEST_CONFIGURATION_TYPE "${JOB_BUILD_CONFIGURATION}")
  # (must be placed *before* any add_subdirectory, cmake bug ?)
  enable_testing()
  
  set(ctest_args )
  list(APPEND ctest_args --output-junit ctest-results.xml) 
  add_custom_target(build-tests)
  if(CMAKE_CONFIGURATION_TYPES)
    add_custom_target(check COMMAND
      ${CMAKE_CTEST_COMMAND} ${ctest_args} -T test -C $<CONFIGURATION>
      DEPENDS build-tests)
  else(CMAKE_CONFIGURATION_TYPES)
    add_custom_target(check COMMAND
      ${CMAKE_CTEST_COMMAND} ${ctest_args} -T test
      DEPENDS build-tests)
  endif(CMAKE_CONFIGURATION_TYPES)
endif(enable-testing)

#

add_subdirectory(src)
if(enable-testing)
  add_subdirectory(tests)
endif(enable-testing)
