/*

MIT License

Copyright (c) 2025 Grégoire Badiche

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


This is free software, made by @gregoire-badiche
Check out https://github.com/gregoire-badiche/rustic-errors for more information

Feel free to include this file in any of your project!

*/

#ifndef RUSTIC_ERRORS
#define RUSTIC_ERRORS

#include <stdlib.h>

#ifndef __RC_CUSTOM
#define __RC_CUSTOM

#define __RC_STATUS_TYPE status_t
#define __RC_SUCCESS success
#define __RC_DEFAULT_ERR error
#define __RC_DEFAULT_TYPE void * // 64 bits

typedef enum
{
    success,
    error
} status_t;

#endif

#define result(T) rc_result_##T##_t

#define register_result(T) typedef struct { \
        __RC_STATUS_TYPE code;              \
        union {                             \
            T v;                            \
            char * e;                       \
        } data;                             \
    } result(T);

#define __RC_VARIADIC(_1, _2, _3, NAME, ...) NAME
    
#define unwrap(res) (res).data.v; if ((res).code != __RC_SUCCESS) return (res);

#define unwrap_or_else(res) (res).data.v; if ((res).code != __RC_SUCCESS)

#define __RC_OK_VAL(T, d) \
    return (result(T)){.code = __RC_SUCCESS, .data.v = d};
#define __RC_OK_CLR(T) return (result(T)){.code = __RC_SUCCESS, .data.e = NULL};
#define ok(...) __RC_VARIADIC(_1, __VA_ARGS__, __RC_OK_VAL, __RC_OK_CLR)(__VA_ARGS__)

#define __RC_ERR_VAL(T, error_code, message) return (result(T)){.code = (error_code), .data.e = (message)};
#define __RC_ERR_CLR(T, message) return (result(T)){.code = __RC_DEFAULT_ERR, .data.e = (message)};
#define err(...) __RC_VARIADIC(__VA_ARGS__, __RC_ERR_VAL, __RC_ERR_CLR)(__VA_ARGS__)

#endif