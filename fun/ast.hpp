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

    struct nil {};
    struct signed_;
    struct expression;
    struct function_call;

    struct variable : x3::position_tagged
    {
        variable(std::string const& name = "") : name(name) {}
        std::string name;
    };

    struct operand :
        x3::variant<
            nil
          , unsigned int
          , variable
          , x3::forward_ast<signed_>
          , x3::forward_ast<expression>
          , x3::forward_ast<function_call>
        >
    {
        using base_type::base_type;
        using base_type::operator=;
    };

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

    struct assignment : x3::position_tagged
    {
        variable lhs;
        expression rhs;
    };

    struct variable_declaration
    {
        assignment assign;
    };

    struct statement :
        x3::variant<
            variable_declaration
          , assignment>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    typedef std::list<statement> statement_list;

    // print functions for debugging
    inline std::ostream& operator<<(std::ostream& out, nil)
    {
        out << "nil";
        return out;
    }

    inline std::ostream& operator<<(std::ostream& out, variable const& var)
    {
        out << var.name; return out;
    }
}}

#endif
