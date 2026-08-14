add_executable(NumArrayTest NumArrayTest.cxx)
arccore_accelerator_add_source_files(NumArrayTest.cxx)
arccore_accelerator_add_to_target(NumArrayTest)
target_link_libraries(NumArrayTest PRIVATE mgis-arcane::MGISArcane)

if(TFELTests_FOUND)
  get_filename_component(_source_directory ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)
  get_filename_component(_binary_directory ${CMAKE_CURRENT_BINARY_DIR} ABSOLUTE)
  if (NOT _source_directory STREQUAL _binary_directory)
    configure_file(   
      ${CMAKE_CURRENT_SOURCE_DIR}/PartialQuadratureFunctionTest.arc
      ${CMAKE_CURRENT_BINARY_DIR}/PartialQuadratureFunctionTest.arc
      COPYONLY)
    configure_file(   
      ${CMAKE_CURRENT_SOURCE_DIR}/PartialQuadratureFunctionTest.msh
      ${CMAKE_CURRENT_BINARY_DIR}/PartialQuadratureFunctionTest.msh
      COPYONLY)
  endif()
  add_executable(PartialQuadratureFunctionTest
    EXCLUDE_FROM_ALL
    PartialQuadratureFunctionTest.cxx)
  add_dependencies(build-tests PartialQuadratureFunctionTest)
  arccore_accelerator_add_source_files(PartialQuadratureFunctionTest.cxx)
  arccore_accelerator_add_to_target(PartialQuadratureFunctionTest)
  target_link_libraries(PartialQuadratureFunctionTest PRIVATE mgis-arcane::MGISArcane tfel::TFELTests)
  add_test(NAME PartialQuadratureFunctionTest
    COMMAND PartialQuadratureFunctionTest)
  add_dependencies(check PartialQuadratureFunctionTest)
endif(TFELTests_FOUND)
