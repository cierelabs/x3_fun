/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "../fun/interpreter.hpp"
#include <boost/foreach.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/assert.hpp>
#include <iostream>

namespace fun { namespace ast { namespace
{
    ////////////////////////////////////////////////////////////////////////////
    //  The interpreter
    ////////////////////////////////////////////////////////////////////////////
    struct interpreter
    {
        typedef double result_type;

        double operator()(ast::nil) const { BOOST_ASSERT(0); }
        double operator()(double ast) const;
        double operator()(double lhs, ast::operation const& ast) const;
        double operator()(ast::signed_ const& ast) const;
        double operator()(ast::expression const& ast) const;
        double operator()(ast::function_call const& ast) const;
    };

    double interpreter::operator()(double ast) const
    {
        return ast;
    }

    double interpreter::operator()(double lhs, ast::operation const& ast) const
    {
        double rhs = boost::apply_visitor(*this, ast.operand_);
        switch (ast.operator_)
        {
            case '+': return lhs + rhs;
            case '-': return lhs - rhs;
            case '*': return lhs * rhs;
            case '/': return lhs / rhs;

            default:
               BOOST_ASSERT(0);
               return 0;
        }
    }

    double interpreter::operator()(ast::signed_ const& ast) const
    {
        double r = boost::apply_visitor(*this, ast.operand_);
        switch (ast.sign)
        {
            case '-': return -r;
            case '+': return r;

            default:
               BOOST_ASSERT(0);
               return 0;
        }
    }

    double interpreter::operator()(ast::expression const& ast) const
    {
        double r = boost::apply_visitor(*this, ast.first);
        for (auto const& oper : ast.rest)
            r = (*this)(r, oper);
         return r;
    }

    double interpreter::operator()(ast::function_call const& ast) const
    {
        return 0; // $$$ for now $$$
    }
}}}

namespace fun { namespace ast
{
    float eval(ast::expression const& ast)
    {
        return interpreter()(ast);
    }
}}
