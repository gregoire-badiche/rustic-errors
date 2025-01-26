#include <stdlib.h>
#include <stdio.h>
#include "../include/rustic-errors.h"

result_t fn_no_return(int param)
{
    if(param != 1)
        err("Quite unpeculiar error, innit?")
    
    // Return type specified
    ok(42, int)
}

result_t fn(int param)
{
    result_t res = fn_no_return(param);

    int answer_to_life = unwrap(res, int);

    printf("%s\n", "Does this gets printed?");

    answer_to_life *= 2;

    // Return type inferred
    ok(answer_to_life);
}

void end_fn(int v)
{
    result_t r = fn(v);
    
    // Sets res to the result
    int res = unwrap_or_else(r, int) {
        // This is called if there was an error
        printf("Error : %s\n", r.data);

        // Do not forget to set res!!
        res = -1;
    }

    printf("Answer : %d\n", res);
}

int main(int argc, char const *argv[])
{
    printf("No errors :\n");
    // No error
    end_fn(1);

    printf("Some errors :\n");
    end_fn(0);

    return 0;
}
