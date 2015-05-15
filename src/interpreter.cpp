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

    struct interpreter_impl
    {
        typedef double result_type;

        interpreter_impl(fmap_type const& fmap, error_handler_type const& error_handler)
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

    double interpreter_impl::operator()(double ast) const
    {
        return ast;
    }

    // INTERPRETER_OPERATION_VISIT_BEGIN
    double interpreter_impl::operator()(double lhs, ast::operation const& ast) const
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
               return -1;
        }
    }
    // INTERPRETER_OPERATION_VISIT_END

    // INTERPRETER_SIGNED_VISIT_BEGIN
    double interpreter_impl::operator()(ast::signed_ const& ast) const
    {
        double r = boost::apply_visitor(*this, ast.operand_);
        switch (ast.sign)
        {
            case '-': return -r;
            case '+': return r;

            default:
               BOOST_ASSERT(0);
               return -1;
        }
    }
    // INTERPRETER_SIGNED_VISIT_END

    double interpreter_impl::operator()(ast::expression const& ast) const
    {
        double r = boost::apply_visitor(*this, ast.first);
        for (auto const& oper : ast.rest)
            r = (*this)(r, oper);
         return r;
    }

    // INTERPRETER_FUNCTION_CALL_VISIT_BEGIN
    double interpreter_impl::operator()(ast::function_call const& ast) const
    {
        auto iter = fmap.find(ast.name);
        if (iter == fmap.end()) {
            error_handler(ast, "Undefined function " + ast.name + '.');
            return -1;
        }

        if (iter->second.second != ast.arguments.size()) {
            std::stringstream out;
            out << "Wrong number of arguments to function " << ast.name << " ("
                << iter->second.second << " expected)." << std::endl;

            error_handler(ast, out.str());
            return -1;
        }

        // Get the args
        double args[detail::max_arity];
        double* p = args;
        for (auto const& arg : ast.arguments)
            *p++ = (*this)(arg);

        // call user function
        return iter->second.first(args);
    }
    // INTERPRETER_FUNCTION_CALL_VISIT_END
}}}

namespace fun { namespace ast
{
    float interpreter::eval(ast::expression const& ast)
    {
        return interpreter_impl(fmap, error_handler)(ast);
    }
}}
