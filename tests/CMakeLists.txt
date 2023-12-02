find_package(example-robot-data REQUIRED)
find_package(GTest REQUIRED)

enable_testing()

# add example
macro(add_unit_test exname)
  set(src_file "${exname}.cpp")
  add_executable(${exname} ${src_file})
  target_link_libraries(${exname} PRIVATE GTest::gtest_main)
  gtest_discover_tests(${exname})
endmacro(add_init_test)

add_unit_test(load_robot)
target_link_libraries(load_robot PRIVATE example-robot-data::example-robot-data)
