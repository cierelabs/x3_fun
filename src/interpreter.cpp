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
    //  The AST interpreter
    ////////////////////////////////////////////////////////////////////////////
    struct interpreter
    {
        typedef void result_type;

        interpreter(std::ostream& out)
            : out(out)
        {}

        void operator()(ast::nil) const { BOOST_ASSERT(0); }
        void operator()(unsigned int ast) const;
        void operator()(ast::variable const& ast) const;
        void operator()(ast::operation const& ast) const;
        void operator()(ast::signed_ const& ast) const;
        void operator()(ast::expression const& ast) const;
        void operator()(ast::function_call const& ast) const;

        std::ostream& out;
    };

    void interpreter::operator()(unsigned int ast) const
    {
        out << ast;
    }

    void interpreter::operator()(ast::variable const& ast) const
    {
        out << ast;
    }

    void interpreter::operator()(ast::operation const& ast) const
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

    void interpreter::operator()(ast::signed_ const& ast) const
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

    void interpreter::operator()(ast::expression const& ast) const
    {
        if (ast.rest.size())
            out << '(';
        boost::apply_visitor(*this, ast.first);
        for (auto const& oper : ast.rest)
            (*this)(oper);
        if (ast.rest.size())
            out << ')';
    }

    void interpreter::operator()(ast::function_call const& ast) const
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
}}}


namespace fun { namespace ast
{
    void print(std::ostream& out, ast::expression const& ast)
    {
        interpreter p(out);
        p(ast);
        out << std::endl;
    }
}}
