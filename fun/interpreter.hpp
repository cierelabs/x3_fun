/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_FUN_INTERPRETER_HPP)
#define BOOST_SPIRIT_X3_FUN_INTERPRETER_HPP

#include <map>
#include <functional>
#include <boost/mpl/int.hpp>
#include "ast.hpp"
#include "error_handler.hpp"

namespace fun { namespace ast
{
    // INTERPRETER_CLASS_VISIT_BEGIN
    class interpreter
    {
    public:

        typedef std::function<
            void(x3::position_tagged, std::string const&)>
        error_handler_type;

        template <typename ErrorHandler>
        interpreter(ErrorHandler const& error_handler);

        template <typename F>
        void add_function(std::string name, F f);

        float eval(ast::expression const& ast);

    private:

        std::map<
            std::string
          , std::pair<std::function<double(double* args)>, std::size_t>
        >
        fmap;

        error_handler_type error_handler;
    };
    // INTERPRETER_CLASS_VISIT_END

    ///////////////////////////////////////////////////////////////////////////
    // Implementation
    ///////////////////////////////////////////////////////////////////////////
    template <typename ErrorHandler>
    inline interpreter::interpreter(ErrorHandler const& error_handler)
        : error_handler(
            [&](x3::position_tagged pos, std::string const& msg)
            { error_handler(pos, msg); }
        )
    {}

    namespace detail
    {
        std::size_t const max_arity = 5;

        template <typename T>
        struct arity : arity<decltype(&T::operator())> {};

        template <typename F, typename RT, typename ...Args>
        struct arity<RT(F::*)(Args...) const>
        {
            enum { value = sizeof...(Args) };
        };

        template <typename F>
        struct adapter_function
        {
            adapter_function(F f)
                : f(f) {}

            double dispatch(double* args, boost::mpl::int_<0>) const
            {
                return f();
            }

            double dispatch(double* args, boost::mpl::int_<1>) const
            {
                return f(args[0]);
            }

            double dispatch(double* args, boost::mpl::int_<2>) const
            {
                return f(args[0], args[1]);
            }

            double dispatch(double* args, boost::mpl::int_<3>) const
            {
                return f(args[0], args[1], args[2]);
            }

            double dispatch(double* args, boost::mpl::int_<4>) const
            {
                return f(args[0], args[1], args[2], args[3]);
            }

            double dispatch(double* args, boost::mpl::int_<5>) const
            {
                return f(args[0], args[1], args[2], args[3], args[4]);
            }

            double operator()(double* args) const
            {
                return dispatch(args, boost::mpl::int_<detail::arity<F>::value>());
            }

            F f;
        };
    }

    // INTERPRETER_ADD_FUNCTION_VISIT_BEGIN
    template <typename F>
    inline void interpreter::add_function(std::string name, F f)
    {
        static_assert(detail::arity<F>::value <= detail::max_arity,
            "Function F has too many arguments (maximum == 5).");

        std::function<double(double* args)> f_adapter = detail::adapter_function<F>(f);
        fmap[name] = std::make_pair(f_adapter, detail::arity<F>::value);
    }
    // INTERPRETER_ADD_FUNCTION_VISIT_END

}}

#endif
