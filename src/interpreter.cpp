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
#include <sstream>

namespace fun { namespace ast { namespace
{
    typedef interpreter::error_handler_type error_handler_type;

    typedef
        std::map<
            std::string
          , std::pair<std::function<double(double* args)>, std::size_t>
        >
    fmap_type;

    struct evaluator
    {
        typedef double result_type;

        evaluator(fmap_type const& fmap, error_handler_type const& error_handler)
            : fmap(fmap)
            , error_handler(error_handler)
        {}

        double operator()(ast::nil) const { BOOST_ASSERT(0); }
        double operator()(double ast) const;
        double operator()(double lhs, ast::operation const& ast) const;
        double operator()(ast::signed_ const& ast) const;
        double operator()(ast::expression const& ast) const;
        double operator()(ast::function_call const& ast) const;

        fmap_type const& fmap;
        error_handler_type const& error_handler;
    };

    double evaluator::operator()(double ast) const
    {
        return ast;
    }

    double evaluator::operator()(double lhs, ast::operation const& ast) const
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

    // INTERPRETER_SIGNED_VISIT_BEGIN
    double evaluator::operator()(ast::signed_ const& ast) const
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
    // INTERPRETER_SIGNED_VISIT_END

    double evaluator::operator()(ast::expression const& ast) const
    {
        double r = boost::apply_visitor(*this, ast.first);
        for (auto const& oper : ast.rest)
            r = (*this)(r, oper);
         return r;
    }

    double evaluator::operator()(ast::function_call const& ast) const
    {
        auto iter = fmap.find(ast.name);

        if (iter == fmap.end())
        {
            error_handler(ast, "Undefined function " + ast.name + '.');
            return 0;
        }

        if (iter->second.second != ast.arguments.size())
        {
            std::stringstream out;
            out
                << "Wrong number of arguments to function " << ast.name << " ("
                << iter->second.second << " expected)." << std::endl;

            error_handler(ast, out.str());
            return 0;
        }

        // Get the args
        double args[detail::max_arity];
        double* p = args;
        for (auto const& arg : ast.arguments)
            *p++ = (*this)(arg);

        // call user function
        return iter->second.first(args);
    }
}}}

namespace fun { namespace ast
{
    float interpreter::eval(ast::expression const& ast)
    {
        return evaluator(fmap, error_handler)(ast);
    }
}}
