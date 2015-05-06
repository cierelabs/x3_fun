/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_FUN_PRINTER_HPP)
#define BOOST_SPIRIT_X3_FUN_PRINTER_HPP

#include <ostream>
#include "ast.hpp"

namespace fun { namespace ast
{
    void print(std::ostream& out, ast::expression const& ast);
}}

#endif
