# rustic-errors

A set of C macros for implementing Rust-inspired errors and results returns

## Howto

This library is just one header to include, containing multiples macros.

It has pre-defined return types (for error and result handling), but you can also set your owns (see **Custom errors template**).

### Returning a result

#### `ok`

In order to return a successful result, the macro `ok` is used. It expands to a block of code containing the `return` keyword and a semicolon (`;`) at the end, so you can just `ok` your way out of the function.

The `ok` macro has three overloads :

- `ok(data, type)`

Here, the data with the explicit type given is returned. The data is wrapped in a `result_t` type, with a `success` code. Note that the data is tranmitted inside a malloc'd block, that is then free'd when unwrapping.

- `ok(data)`

Without any type provided, the data is casted to a `long` type for transport, and then casted back when retrieving. The size of the data can't be larger than `sizeof(long)` for obvious reasons, however this casting type can be customized. to accept larger data types. Note that the `long` types supports pointers in both 32 and 64 bits.

- `ok()`

Used to signal only a success, with no return. Simply returns the `success` code wrapped in a `result_t` struct, but with no `malloc()`. Can readuce overhead.

#### `err`

The `err` macro is used to return an error. This macro has two overloads

- `err(message)`

This is the error in its simplest form. It return an erro just like `ok` returns a result. Please use hardcoded stack-stored error messages, like `err("I am defined at compile time!")` for it to work properly. You can return malloc'd messages, but have to make sure they are correctly free'd.

- `err(custom_error_code, message)`

When using custom errors, you can specify the error code and then retrieve it using (for example, depending on the context) `res.code` if the result is stored in `result_t res`.

### Unwrapping the result

As the result is encapsulated in a `result_t` type (with some malloc'd data), there is a safe way to retrieve it content.

#### `unwrap`

Suppose the result is stored in the `result_t res` variable.

This macro is used to immediatly return the error if there is one. It has two overloads, described below. Note that both correctly `free` the data if there is some, and set the pointer to `NULL` in order to avoid use-after-free.

- `unwrap(res, type)`

Used in the form `type mydata = unwrap(res, type)`, for example if the returned data is an integer : `int myint = unwrap(res, int)`. Will cause a core dump if you try to retrieve the data from a no-parameter `ok()` (dereferencing a `NULL` pointer).

- `unwrap(res)`

Used if you do not wish to retrieve the data, or if the function does not returns data (in pair with `ok()` without any parameter).

#### `unwrap_or_else`

This macro works like `unwrap`, however it allows you to execute a code block instead if returning the error in case of an error. Just like `unwrap`, it has two overloads :

- `unwrap_or_else(res, type)`

Used to set a variable to the given `type`, extracted from the `res` variable. Executes the code block given right after if the result returned is an error. Used like :
```c
result_t res = myfunc();
int myint = unwrap_or_else(res, int) {
    // Reached if there was an error
    printf("there has been an error");
    myint = -1;
}
```

- `unwrap_or_else(res)`

Used if you do not wish to set a variable to the returned result. Used just like the previous overload, without the variable assignment.

### Using the library

Feel free to check the `examples/*.c` examples. A very basic example is :
```c

result_t myfn()
{
    void *m = malloc(sizeof(int));
    if(m == NULL)
        err("Not enough memory!")
    // Not executed if m == NULL
    free(m);
    // Type '''inferring'''
    ok(42)
}

int main()
{
    // Storing the result in a variable
    result_t res = myfn();
    // Unwrapping the result
    int result = unwrap_or_else(res, int) {
        // Executed if error
        printf("%s\n", res.data);
        return 1;
    }
    printf("Result : %d\n", result);
    return 0;
}
```

## Custom errors

A template for setting custom errors codes :

```c
#include "rustic-errors.h"

// Enables the custom errors
#define __RC_CUSTOM

// Define the enum containing all the error codes
// Note : this could also be `int`, with 1 instead of `success` and 0 instead of `default_error`
#define __RC_STATUS_TYPE status_t 

// Define the default value representing a success
#define __RC_SUCCESS success

// Define the default value for error
#define __RC_DEFAULT_ERR default_error

// Defines the default type in which the value of ok(value) will be casted. Must be large enough to contain any possible return type. Default value is `long`
#define __RC_DEFAULT_TYPE long

// This is an example! Note that the default one only has `success` and `default error` (as `error`) defined.
typedef enum
{
    success,
    default_error,
    couldnt_read,
    not_enough_memory
} status_t;
```

The `status_t` type is used in the following return struct :

```c
typedef struct
{
    // status_t code
    __RC_STATUS_TYPE code;

    // Error message or result data
    void *data;
} result_t;
```

## Examples

- `examples/answer_to_life.c`

A overall presentation of the features of this library.

- `examples/custom_errors.c`

A example for custom errors

## Contribute

Feel free to fork, I am open to every improvement!
