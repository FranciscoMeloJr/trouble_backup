#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER cct

#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "tp.h"

#if !defined(_TP_H) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _TP_H

#include <lttng/tracepoint.h>

TRACEPOINT_EVENT(
    cct,
    tracepoint,
    TP_ARGS(
        int, my_integer_arg,
        char*, my_string_arg
    ),
    TP_FIELDS(
        ctf_string(my_string_field, my_string_arg)
        ctf_integer(int, my_integer_field, my_integer_arg)
    )
)

TRACEPOINT_EVENT(
    cct,
    getinfo,
    TP_ARGS(
        int, my_integer_arg,
        int, my_integer_arg2,
        char*, my_string_arg
    ),
    TP_FIELDS(
        ctf_string(my_string_field, my_string_arg)
        ctf_integer(int, my_integer_field, my_integer_arg)
        ctf_integer(int, my_integer_field2, my_integer_arg2)
    )
)
#endif /* _TP_H */

#include <lttng/tracepoint-event.h>

