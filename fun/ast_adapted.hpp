/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_FUN_AST_ADAPTED_HPP)
#define BOOST_SPIRIT_X3_FUN_AST_ADAPTED_HPP

#include "ast.hpp"
#include <boost/fusion/include/adapt_struct.hpp>

// AST_ADAPTED_VISIT_BEGIN
BOOST_FUSION_ADAPT_STRUCT(
    fun::ast::signed_,
    (char, sign)
    (fun::ast::operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
    fun::ast::operation,
    (char, operator_)
    (fun::ast::operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
    fun::ast::expression,
    (fun::ast::operand, first)
    (std::list<fun::ast::operation>, rest)
)

BOOST_FUSION_ADAPT_STRUCT(
    fun::ast::function_call,
    (std::string, name)
    (std::list<fun::ast::expression>, arguments)
)
// AST_ADAPTED_VISIT_END

#endif
