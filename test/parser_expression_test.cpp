/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <iostream>
#include <iterator>
#include <algorithm>

#include "../fun/ast.hpp"
#include "../fun/expression.hpp"
#include "../fun/error_handler.hpp"
#include "../fun/config.hpp"
#include "../fun/printer.hpp"

#include "test.hpp"

namespace fs = boost::filesystem;

int main(int argc, char* argv[])
{
    if (argc < 1)
    {
       std::cout << "usage: " << argv[0] << " path/to/test/files" << std::endl;
       return -1;
    }

    auto processor = [](fs::path input_path, fs::path expect_path)
    {
//        test::compare(input_path, expect_path, [](auto const& s){ return s; });

        std::cout << "=============================================" << std::endl;
        std::cout << "Parsing: " << input_path << std::endl;
        std::cout << "=============================================" << std::endl;
        std::string source = test::load(input_path);

        using fun::parser::iterator_type;
        iterator_type iter(source.begin());
        iterator_type end(source.end());

        // Our AST
        fun::ast::expression ast;

        // Our error handler
        using boost::spirit::x3::with;
        using fun::parser::error_handler_type;
        error_handler_type error_handler(iter, end, std::cerr); // Our error handler

        // Our parser
        auto const parser =
            // we pass our error handler to the parser so we can access
            // it later on in our on_error and on_sucess handlers
            with<fun::parser::error_handler_tag>(std::ref(error_handler))
            [
                fun::expression()
            ];

        using boost::spirit::x3::ascii::space;
        bool success = phrase_parse(iter, end, parser, space, ast);
       
        if (success)
            fun::ast::print(std::cout, ast);

    };

    test::for_each_file(fs::path(argv[1]), processor);
    return 0;
}