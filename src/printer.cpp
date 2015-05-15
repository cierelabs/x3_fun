/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "../fun/printer.hpp"
#include <boost/foreach.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/assert.hpp>
#include <iostream>

namespace fun { namespace ast { namespace
{
    ////////////////////////////////////////////////////////////////////////////
    //  The AST printer
    ////////////////////////////////////////////////////////////////////////////
    // AST_PRINTER1_VISIT_BEGIN
    struct printer
    {
        typedef void result_type;

        printer(std::ostream& out)
            : out(out)
        {}

        void operator()(ast::nil) const { BOOST_ASSERT(0); }
        void operator()(double ast) const;
        void operator()(ast::operation const& ast) const;
        void operator()(ast::signed_ const& ast) const;
        void operator()(ast::expression const& ast) const;
        void operator()(ast::function_call const& ast) const;

        std::ostream& out;
    };
    // AST_PRINTER1_VISIT_END

    // AST_PRINTER2_VISIT_BEGIN
    void printer::operator()(double ast) const
    {
        out << ast;
    }

    void printer::operator()(ast::operation const& ast) const
    {
        switch (ast.operator_)
        {
            case '+': out << " + "; break;
            case '-': out << " - "; break;
            case '*': out << " * "; break;
            case '/': out << " / "; break;

            default:
               BOOST_ASSERT(0);
               return;
        }
        boost::apply_visitor(*this, ast.operand_);
    }
    // AST_PRINTER2_VISIT_END

    void printer::operator()(ast::signed_ const& ast) const
    {
        switch (ast.sign)
        {
            case '-': out << "- "; break;
            case '+': break;

            default:
               BOOST_ASSERT(0);
               return;
        }
        boost::apply_visitor(*this, ast.operand_);
    }

    // AST_PRINTER3_VISIT_BEGIN
    void printer::operator()(ast::expression const& ast) const
    {
        if (ast.rest.size())
            out << '(';
        boost::apply_visitor(*this, ast.first);
        for (auto const& oper : ast.rest)
            (*this)(oper);
        if (ast.rest.size())
            out << ')';
    }
    // AST_PRINTER3_VISIT_END

    // AST_PRINTER4_VISIT_BEGIN
    void printer::operator()(ast::function_call const& ast) const
    {
        out << ast.name;
        if (ast.arguments.size())
            out << '(';
        bool first = true;
        for (auto const& arg : ast.arguments)
        {
            if (first)
                first = false;
            else
                out << ", ";
            (*this)(arg);
        }
        if (ast.arguments.size())
            out << ')';
    }
    // AST_PRINTER4_VISIT_END
}}}


namespace fun { namespace ast
{
    void print(std::ostream& out, ast::expression const& ast)
    {
        printer p(out);
        p(ast);
        out << std::endl;
    }
}}
