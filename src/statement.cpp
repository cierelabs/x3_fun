/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "statement_def.hpp"
#include "config.hpp"

namespace fun { namespace parser
{
    BOOST_SPIRIT_INSTANTIATE(statement_type, iterator_type, context_type);
}}
