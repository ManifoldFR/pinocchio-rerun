find_package(example-robot-data REQUIRED)
find_package(GTest REQUIRED)

enable_testing()

set(lib_test_util ${LIB_NAME}_test_util)
add_library(${lib_test_util} STATIC util.cpp)
set_standard_output_directory(${lib_test_util})
target_link_libraries(
  ${lib_test_util}
  PUBLIC example-robot-data::example-robot-data
)
target_link_libraries(${lib_test_util} PUBLIC fmt::fmt)
target_link_libraries(${lib_test_util} PRIVATE pinocchio::pinocchio)
target_compile_definitions(
  ${lib_test_util}
  PUBLIC ASSET_DIR="${PROJECT_SOURCE_DIR}/assets"
)

# add example
macro(pinrerun_add_unit_test exname)
  set(src_file "${exname}.cpp")
  add_executable(${exname} ${src_file})
  set_standard_output_directory(${exname})
  target_link_libraries(${exname} PRIVATE GTest::gtest_main)
  target_link_libraries(${exname} PRIVATE ${LIB_NAME})
  target_link_libraries(${exname} PRIVATE ${lib_test_util})
  gtest_discover_tests(${exname})
endmacro(pinrerun_add_unit_test)

pinrerun_add_unit_test(test_load_mesh)
pinrerun_add_unit_test(test_load_robot)
pinrerun_add_unit_test(test_rerun)
