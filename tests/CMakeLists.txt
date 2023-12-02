find_package(example-robot-data REQUIRED)
find_package(GTest REQUIRED)

enable_testing()

set(lib_test_util ${LIB_NAME}_test_util)
add_library(${lib_test_util} STATIC util.cpp)
target_link_libraries(${lib_test_util} PUBLIC example-robot-data::example-robot-data)
target_link_libraries(${lib_test_util} PUBLIC fmt::fmt)
target_link_libraries(${lib_test_util} PRIVATE pinocchio::pinocchio)
target_compile_definitions(${lib_test_util}
                           PUBLIC ASSET_DIR="${PROJECT_SOURCE_DIR}/assets")

# add example
macro(add_unit_test exname)
  set(src_file "${exname}.cpp")
  add_executable(${exname} ${src_file})
  target_link_libraries(${exname} PRIVATE GTest::gtest_main)
  target_link_libraries(${exname} PRIVATE ${LIB_NAME})
  target_link_libraries(${exname} PRIVATE ${lib_test_util})
  gtest_discover_tests(${exname})
endmacro(add_unit_test)

add_unit_test(test_load_mesh)
add_unit_test(test_load_robot)
add_unit_test(test_rerun)
