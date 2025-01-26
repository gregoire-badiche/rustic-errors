#include <stdlib.h>
#include <stdio.h>
#include "../include/rustic-errors.h"

result_t fn_no_return(int param)
{
    if(param != 1)
        err("Quite unpeculiar error, innit?")
    
    ok(42, int)
}

result_t fn(int param)
{
    result_t res = fn_no_return(param);

    int answer_to_life = unwrap(res, int);

    printf("%s\n", "Does this gets printed?");

    ok(answer_to_life);
}

int main(int argc, char const *argv[])
{
    result_t a = fn(1);
    
    int res = unwrap_or_else(a, int) {
        printf("%s - oops\n", a.data);

        // Do not forget to set res!!
        res = -1;
    }

    printf("Answer : %d\n", res);

    return 0;
}
