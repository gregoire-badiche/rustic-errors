#include <stdio.h>

#define __RC_CUSTOM
#define __RC_STATUS_TYPE status_t 
#define __RC_SUCCESS success
#define __RC_DEFAULT_ERR default_error
#define __RC_DEFAULT_TYPE long

typedef enum
{
    success,
    default_error,
    couldnt_read,
    not_enough_memory
} status_t;

#include "../include/rustic-errors.h"

result_t returns_default_error()
{
    err("Erm unknown error")
}

result_t returns_custom_error()
{
    err(not_enough_memory, "Cannot `malloc`: memory is full")
}

result_t no_error()
{
    // Inferring the type
    ok(12);
}

int main(int argc, char const *argv[])
{
    result_t res = returns_default_error();

    printf("Default error : %d\n", res.code);
    printf("Error message : %s\n", res.data);

    // Throws away the data (free it) if there is some
    unwrap_or_else(res) {
        printf("No data to be freed (no result)\n");
    };

    res = returns_custom_error();

    printf("Custom error : %d\n", res.code);
    printf("Error message : %s\n", res.data);

    if (res.code == not_enough_memory)
    {
        printf("I can tell that my system doesn't have enough memory!\n");
    }

    // Free the malloc'd data if there is some
    unwrap_or_else(res) {
        printf("No data to be freed (no result, again)\n");
    };

    res = no_error();

    // Free the malloc'd data (this time for real)
    unwrap_or_else(res)
    {
        printf("will not be printed, as unwrap was successful\n");
    }

    return 0;
}
