cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")

find_package(
  Boost 1.65 REQUIRED
  COMPONENTS
  filesystem
  regex
)

add_executable( eval
  test/eval_expression_test.cpp
  src/expression.cpp
  src/interpreter.cpp
)
target_link_libraries( eval
  Boost::filesystem
  Boost::regex
)

add_executable( parse
  test/parse_expression_test.cpp
  src/expression.cpp
  src/printer.cpp
)
target_link_libraries( parse
  Boost::filesystem
  Boost::regex
)
