#include <stdlib.h>
#include <stdio.h>
#include "../include/rustic-errors-less-overhead.h"

// Register our result type
register_result(int);

result(int) fn_no_return(int param)
{
    if(param != 1)
        // Sadly here, the error "type" must be specified
        err(int, "Quite unpeculiar error, innit?")
    
    int res = 42;
    // Return type specified
    ok(int, res)
}

result(int) fn(int param)
{
    result(int) res = fn_no_return(param);

    int answer_to_life = unwrap(res);

    printf("%s\n", "Does this gets printed?");

    answer_to_life *= 2;

    // Return type must be specified
    ok(int, answer_to_life);
}

void end_fn(int v)
{
    result(int) r = fn(v);
    
    // Sets res to the result
    int res = unwrap_or_else(r, error, message) {
        // This is called if there was an error
        printf("Error : %s\n", message);

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
