/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_FUN_AST_HPP)
#define BOOST_SPIRIT_X3_FUN_AST_HPP

#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/fusion/include/io.hpp>
#include <list>

namespace fun { namespace ast
{
    ///////////////////////////////////////////////////////////////////////////
    //  The AST
    ///////////////////////////////////////////////////////////////////////////
    namespace x3 = boost::spirit::x3;

    // AST1_VISIT_BEGIN
    struct nil {};
    struct signed_;
    struct expression;
    struct function_call;

    struct operand :
        x3::variant<
            nil
          , double
          , x3::forward_ast<signed_>
          , x3::forward_ast<expression>
          , x3::forward_ast<function_call>
        >
    {
        using base_type::base_type;
        using base_type::operator=;
    };
    // AST1_VISIT_END

    // AST2_VISIT_BEGIN
    struct signed_
    {
        char sign;
        operand operand_;
    };

    struct operation : x3::position_tagged
    {
        char operator_;
        operand operand_;
    };

    struct expression : x3::position_tagged
    {
        operand first;
        std::list<operation> rest;
    };

    struct function_call : x3::position_tagged
    {
        std::string name;
        std::list<expression> arguments;
    };
    // AST2_VISIT_END

    // print functions for debugging
    inline std::ostream& operator<<(std::ostream& out, nil)
    {
        out << "nil";
        return out;
    }

    // AST_VISIT_END
}}

#endif
