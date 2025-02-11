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
#define __RC_DEFAULT_TYPE long

typedef enum
{
    success,
    error
} status_t;

#endif

typedef struct
{
    __RC_STATUS_TYPE code;
    void *data; // Error message or result data
} result_t;

#define __RC_RETURN_TYPE result_t

#define __RC_VARIADIC(_0, _1, _2, NAME, ...) NAME

#define __RC_CLEANUP(result)                                    \
    if ((result).data != NULL && (result).code == __RC_SUCCESS) \
    {                                                           \
        free((result).data);                                    \
        (result).data = NULL;                                   \
    }

#define __RC_UNWRAP_CLR(result) __RC_CLEANUP(result) if ((result).code != __RC_SUCCESS) return (result);
#define __RC_UNWRAP_VAL_NR(result, type)                          \
    (result).code == __RC_SUCCESS ? *(type *)((result).data) : 0; \
    __RC_CLEANUP(result)
#define __RC_UNWRAP_VAL(result, type) __RC_UNWRAP_VAL_NR(result, type) __RC_UNWRAP_CLR(result)
#define unwrap(...) __RC_VARIADIC(_0, __VA_ARGS__, __RC_UNWRAP_VAL, __RC_UNWRAP_CLR)(__VA_ARGS__)

#define __RC_UNWRAP_CLR_ELSE(result) __RC_CLEANUP(result) if ((result).code != __RC_SUCCESS)
#define __RC_UNWRAP_VAL_ELSE(result, type) __RC_UNWRAP_VAL_NR(result, type) if ((result).code != __RC_SUCCESS)
#define unwrap_or_else(...) __RC_VARIADIC(_0, __VA_ARGS__, __RC_UNWRAP_VAL_ELSE, __RC_UNWRAP_CLR_ELSE)(__VA_ARGS__)

#define __RC_OK_VAL(data, type)        \
    type *__rc = malloc(sizeof(type)); \
    *__rc = (data);                    \
    return (__RC_RETURN_TYPE){__RC_SUCCESS, (void *)__rc};
#define __RC_OK_UNK(data)              \
    __RC_DEFAULT_TYPE *__rc = malloc(sizeof(__RC_DEFAULT_TYPE)); \
    *__rc = *(__RC_DEFAULT_TYPE *)(&data);              \
    return (__RC_RETURN_TYPE){__RC_SUCCESS, (void *)__rc};
#define __RC_OK_CLR() return (__RC_RETURN_TYPE){__RC_SUCCESS, NULL};
#define ok(...) __RC_VARIADIC(_0 __VA_OPT__(, ) __VA_ARGS__, __RC_OK_VAL, __RC_OK_UNK, __RC_OK_CLR)(__VA_ARGS__)

#define __RC_ERR_VAL(error_code, message) return (__RC_RETURN_TYPE){(error_code), (void *)(message)};
#define __RC_ERR_CLR(message) return (__RC_RETURN_TYPE){__RC_DEFAULT_ERR, (void *)(message)};
#define err(...) __RC_VARIADIC(_0, __VA_ARGS__, __RC_ERR_VAL, __RC_ERR_CLR)(__VA_ARGS__)

#endif