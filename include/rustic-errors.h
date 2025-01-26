#ifndef RUSTIC_ERRORS
#define RUSTIC_ERRORS

#include <stdlib.h>

#ifndef __RC_CUSTOM

#define __RC_RETURN_TYPE result_t
#define __RC_SUCCESS success
#define __RC_DEFAULT_ERR error

typedef enum
{
    success,
    error
} status_t;

typedef struct
{
    status_t code;
    void *data; // Error message or result data
} result_t;

#endif

#define __RC_VARIADIC_NE(_1, _2, NAME, ...) NAME
#define __RC_VARIADIC_E(_0, _1, _2, NAME, ...) NAME

#define __RC_CLEANUP(result) if((result).data != NULL && (result).code == __RC_SUCCESS) { free((result).data); (result).data = NULL; }

#define __RC_UNWRAP_CLR(result) __RC_CLEANUP(result) if ((result).code != __RC_SUCCESS) return (result);
#define __RC_UNWRAP_VAL_NR(result, type) (result).code == __RC_SUCCESS ? *(type *)((result).data) : 0; __RC_CLEANUP(result)
#define __RC_UNWRAP_VAL(result, type) __RC_UNWRAP_VAL_NR(result, type) __RC_UNWRAP_CLR(result)
#define unwrap(...) __RC_VARIADIC_NE(__VA_ARGS__, __RC_UNWRAP_VAL, __RC_UNWRAP_CLR)(__VA_ARGS__)

#define __RC_UNWRAP_CLR_ELSE(result) __RC_CLEANUP(result) if ((result).code != __RC_SUCCESS)
#define __RC_UNWRAP_VAL_ELSE(result, type) __RC_UNWRAP_VAL_NR(result, type) if ((result).code != __RC_SUCCESS)
#define unwrap_or_else(...) __RC_VARIADIC_NE(__VA_ARGS__, __RC_UNWRAP_VAL_ELSE, __RC_UNWRAP_CLR_ELSE)(__VA_ARGS__)

#define __RC_OK_VAL(data, type) type *__rc = malloc(sizeof(type)); *__rc = data; return (__RC_RETURN_TYPE){__RC_SUCCESS, (void *)__rc};
#define __RC_OK_UNK(data) void **__rc = malloc(sizeof(void *)); *__rc = (void *)data; return (__RC_RETURN_TYPE){__RC_SUCCESS, (void *)__rc};
#define __RC_OK_CLR() return (__RC_RETURN_TYPE){__RC_SUCCESS, NULL};
#define ok(...) __RC_VARIADIC_E(_0 __VA_OPT__(,) __VA_ARGS__, __RC_OK_VAL, __RC_OK_UNK, __RC_OK_CLR)(__VA_ARGS__)

#define __RC_ERR_VAL(error_code, message) return (__RC_RETURN_TYPE){error_code, (void *)message};
#define __RC_ERR_CLR(message) return (__RC_RETURN_TYPE){__RC_DEFAULT_ERR, (void *)message};
#define err(...) __RC_VARIADIC_NE(__VA_ARGS__, __RC_ERR_VAL, __RC_ERR_CLR)(__VA_ARGS__)

#endif