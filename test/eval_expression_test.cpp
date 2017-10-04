/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <iostream>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cmath>

#include "../fun/ast.hpp"
#include "../fun/expression.hpp"
#include "../fun/error_handler.hpp"
#include "../fun/config.hpp"
#include "../fun/interpreter.hpp"

#include <boost/spirit/home/x3/support/utility/testing.hpp>

namespace fs = boost::filesystem;
namespace testing = boost::spirit::x3::testing;

auto parse = [](std::string const& source, fs::path input_path)-> std::string
{
    std::stringstream out;

    using fun::parser::iterator_type;
    iterator_type iter(source.begin());
    iterator_type end(source.end());

    // Our AST
    fun::ast::expression ast;

    // Our error handler
    using boost::spirit::x3::with;
    using fun::parser::error_handler_type;
    error_handler_type error_handler(iter, end, out, input_path.c_str()); // Our error handler

    // Our parser
    auto const parser =
        // we pass our error handler to the parser so we can access
        // it later on in our on_error and on_sucess handlers
        with<fun::parser::error_handler_tag>(std::ref(error_handler))
        [
            fun::expression()
        ];

    // Our Interpreter
    fun::ast::interpreter interp(error_handler);

    // Add some functions:
    interp.add_function("pi", []{ return M_PI; });
    interp.add_function("sin", [](double x){ return std::sin(x); });
    interp.add_function("cos", [](double x){ return std::cos(x); });

    // Go forth and parse!
    using boost::spirit::x3::ascii::space;
    bool success = phrase_parse(iter, end, parser, space, ast);

    if (success)
    {
        if (iter != end)
            error_handler( iter, "Error! Expecting end of input here:" );
        else {
            double result = interp.eval(ast);
            if (out.str().empty())
                out << result << std::endl;
        }
    }

    return out.str();
};

auto compare = [](fs::path input_path, fs::path expect_path)
{
   testing::compare(input_path, expect_path, parse);
};

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
       std::cout << "usage: " << fs::path(argv[0]).filename() << " path/to/test/files" << std::endl;
       return -1;
    }

    return testing::for_each_file(fs::path(argv[1]), compare);
}