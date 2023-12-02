#include "pinviz.hpp"
#include <eigenpy/eigenpy.hpp>

#include <string_view>

#define PYMODULE() BOOST_PYTHON_MODULE(MODULE_NAME)

PYMODULE() { using namespace pinviz; }
