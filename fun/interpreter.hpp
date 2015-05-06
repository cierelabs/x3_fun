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

namespace fun { namespace ast
{
    class interpreter
    {
    public:

        template <typename F>
        void add_function(std::string name, F f);

        float eval(ast::expression const& ast);

    private:

        std::map<
            std::string
          , std::pair<std::function<double(double* args)>, std::size_t>
        >
        fmap;
    };

    ///////////////////////////////////////////////////////////////////////////
    // Implementation
    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
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

            double operator()(double* args) const
            {
                return dispatch(args, boost::mpl::int_<detail::arity<F>::value>());
            }

            F f;
        };
    }

    template <typename F>
    inline void interpreter::add_function(std::string name, F f)
    {
        std::function<double(double* args)> f_adapter = detail::adapter_function<F>(f);
        fmap[name] = std::make_pair(f_adapter, detail::arity<F>::value);
    }
}}

#endif
