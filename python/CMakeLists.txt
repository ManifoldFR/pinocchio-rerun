include(${PROJECT_SOURCE_DIR}/cmake-module/stubs.cmake)
include(${PROJECT_SOURCE_DIR}/cmake-module/python.cmake)

find_package(eigenpy REQUIRED)

file(GLOB py_SRC CONFIGURE_DEPENDS "*.cpp")

set(PYLIB "${LIB_NAME}_pywrap")
python3_add_library(${PYLIB} MODULE WITH_SOABI ${py_SRC})

target_link_libraries(${PYLIB} PUBLIC ${LIB_NAME} eigenpy::eigenpy)
target_compile_definitions(${PYLIB} PRIVATE MODULE_NAME=${PYLIB})

set(PYLIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${PYTHON_SITELIB}/${LIB_NAME}")
GET_RELATIVE_RPATH(${PYLIB_INSTALL_DIR} PYLIB_INSTALL_RPATH)

set_target_properties(
  ${PYLIB}
  PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${LIB_NAME}
    INSTALL_RPATH "${PYLIB_INSTALL_RPATH}"
)

install(TARGETS ${PYLIB} DESTINATION ${PYLIB_INSTALL_DIR})

PYTHON_INSTALL_ON_SITE(${LIB_NAME} __init__.py)

# Generate stubs

if(GENERATE_PYTHON_STUBS)
  LOAD_STUBGEN()
  GENERATE_STUBS(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${LIB_NAME}
    ${CMAKE_INSTALL_PREFIX}/${PYTHON_SITELIB}
    ${PYLIB}
  )
endif(GENERATE_PYTHON_STUBS)
