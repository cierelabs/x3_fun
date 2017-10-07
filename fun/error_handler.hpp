/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_FUN_ERROR_HANDLER_HPP)
#define BOOST_SPIRIT_X3_FUN_ERROR_HANDLER_HPP

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include "expression.hpp"
#include <map>

namespace fun { namespace parser
{
    namespace x3 = boost::spirit::x3;

    ////////////////////////////////////////////////////////////////////////////
    //  Our error handler
    ////////////////////////////////////////////////////////////////////////////
    // ERROR_HANDLER1_VISIT_BEGIN
    // X3 Error Handler Utility
    template <typename Iterator>
    using error_handler = x3::error_handler<Iterator>;

    // tag used to get our error handler from the context
    using x3::error_handler_tag;

    struct error_handler_base
    {
        error_handler_base();

        template <typename Iterator, typename Exception, typename Context>
        x3::error_handler_result on_error(
            Iterator& first, Iterator const& last
          , Exception const& x, Context const& context);

        std::map<std::string, std::string> id_map;
    };
    // ERROR_HANDLER1_VISIT_END

    ////////////////////////////////////////////////////////////////////////////
    // Implementation
    ////////////////////////////////////////////////////////////////////////////

    // ERROR_HANDLER2_VISIT_BEGIN
    inline error_handler_base::error_handler_base()
      : id_map{
            { "expression", "Expression" },
            { "additive_expr", "Expression" },
            { "multiplicative_expr", "Expression" },
            { "unary_expr", "Expression" },
            { "primary_expr", "Expression" },
            { "argument_list", "Argument List" }
        }
    {
    }
    // ERROR_HANDLER2_VISIT_END

    // ERROR_HANDLER3_VISIT_BEGIN
    template <typename Iterator, typename Exception, typename Context>
    inline x3::error_handler_result
    error_handler_base::on_error(
        Iterator& first, Iterator const& last
      , Exception const& x, Context const& context)
    {
        std::string which = x.which();
        auto iter = id_map.find(which);
        if (iter != id_map.end())
            which = iter->second;

        std::string message = "Error! Expecting: " + which + " here:";
        auto& error_handler = x3::get<error_handler_tag>(context).get();
        error_handler(x.where(), message);
        return x3::error_handler_result::fail;
    }
    // ERROR_HANDLER3_VISIT_END
}}

#endif
