/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_FUN_EXPRESSION_DEF_HPP)
#define BOOST_SPIRIT_X3_FUN_EXPRESSION_DEF_HPP

#include <boost/spirit/home/x3.hpp>
#include "ast.hpp"
#include "ast_adapted.hpp"
#include "expression.hpp"
#include "common.hpp"
#include "error_handler.hpp"
#include "annotation.hpp"

namespace fun { namespace parser
{
    using x3::double_;
    using x3::char_;
    using x3::raw;
    using x3::lexeme;
    using namespace x3::ascii;

    // EXPRESSION_DEF1_VISIT_BEGIN
    struct additive_expr_class;
    struct multiplicative_expr_class;
    struct unary_expr_class;
    struct primary_expr_class;
    struct argument_list_class;
    struct function_call_class;
    // EXPRESSION_DEF1_VISIT_END

    // EXPRESSION_DEF2_VISIT_BEGIN
    typedef x3::rule<additive_expr_class, ast::expression>
    additive_expr_type;

    typedef
        x3::rule<multiplicative_expr_class, ast::expression>
    multiplicative_expr_type;

    typedef
        x3::rule<unary_expr_class, ast::operand>
    unary_expr_type;

    typedef
        x3::rule<primary_expr_class, ast::operand>
    primary_expr_type;

    typedef
        x3::rule<argument_list_class, std::list<ast::expression>>
    argument_list_type;

    typedef
        x3::rule<function_call_class, ast::function_call>
    function_call_type;
    // EXPRESSION_DEF2_VISIT_END

    // EXPRESSION_DEF3_VISIT_BEGIN
    expression_type const
        expression = "expression";

    additive_expr_type const
        additive_expr = "additive_expr";

    multiplicative_expr_type const
        multiplicative_expr = "multiplicative_expr";

    unary_expr_type const
        unary_expr = "unary_expr";

    primary_expr_type const
        primary_expr = "primary_expr";

    argument_list_type const
        argument_list = "argument_list";

    function_call_type const
        function_call = "function_call";
    // EXPRESSION_DEF3_VISIT_END

    // EXPRESSION_DEF4_VISIT_BEGIN
    auto const additive_expr_def =
        multiplicative_expr
        >> *(   (char_('+') > multiplicative_expr)
            |   (char_('-') > multiplicative_expr)
            )
        ;

    auto const multiplicative_expr_def =
        unary_expr
        >> *(   (char_('*') > unary_expr)
            |   (char_('/') > unary_expr)
            )
        ;

    auto const unary_expr_def =
            primary_expr
        |   (char_('-') > primary_expr)
        |   (char_('+') > primary_expr)
        ;
    // EXPRESSION_DEF4_VISIT_END

    // EXPRESSION_DEF5_VISIT_BEGIN
    auto argument_list_def = expression % ',';

    auto function_call_def =
            identifier
        >> -('(' > argument_list > ')')
        ;

    auto const primary_expr_def =
            double_
        |   function_call
        |   '(' > expression > ')'
        ;

    auto const expression_def = additive_expr;
    // EXPRESSION_DEF5_VISIT_END

    // EXPRESSION_DEF6_VISIT_BEGIN
    BOOST_SPIRIT_DEFINE(
        expression
      , additive_expr
      , multiplicative_expr
      , unary_expr
      , primary_expr
      , argument_list
      , function_call
    );
    // EXPRESSION_DEF6_VISIT_END

    // EXPRESSION_DEF7_VISIT_BEGIN
    struct unary_expr_class : annotation_base {};
    struct primary_expr_class : annotation_base {};
    struct function_call_class : annotation_base {};

    struct expression_class :
        annotation_base, error_handler_base {};
    // EXPRESSION_DEF7_VISIT_END
}}

// EXPRESSION_DEF8_VISIT_BEGIN
namespace fun
{
    parser::expression_type const& expression()
    {
        return parser::expression;
    }
}
// EXPRESSION_DEF8_VISIT_END

#endif
