# LIBSTRINGS

Simple C strings lib.

This lib provides simple manipulation of strings that ensures null termination.
The strings can still be manipulated with standard C libs.

## Example

```C
#include "libstrings.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

static void print_error(const char *msg, int error_code)
{
        fprintf(stderr, "%s : %s\n", msg, strerror(error_code));
}

static void print_string(const struct string *str)
{
        printf("content : '%s'\nlength : %ld\ncapacity : %ld\n\n",
                        str->value, string_len(str), string_capacity(str));
}

int main()
{
        int res;

        struct string *str = string_empty();
        print_string(str);

        string_copy_c(str, "world");
        print_string(str);

        string_append_c(str, "!");
        print_string(str);

        string_prepend_c(str, "Hello ");
        print_string(str);

        struct string *copy = string_dup(str);
        print_string(copy);

        string_cut(copy, 1, string_len(copy) - 2);
        print_string(copy);

        struct string *sub = string_sub_v(copy->value, 1, string_len(copy) - 2);
        print_string(sub);

        string_destroy(copy);
        string_destroy(sub);

        string_clear(str);
        print_string(str);

        string_fit(str);
        print_string(str);

        string_reserve(str, 32);
        print_string(str);

        res = string_printf(str, "Hello %u world%c", 2, '!');
        printf("res = %d\n", res);
        print_string(str);

        string_destroy(str);

        return 0;
}
```

Expected output :
```
content : ''
length : 0
capacity : 1

content : 'world'
length : 5
capacity : 11

content : 'world!'
length : 6
capacity : 11

content : 'Hello world!'
length : 12
capacity : 25

content : 'Hello world!'
length : 12
capacity : 13

content : 'ello world'
length : 10
capacity : 13

content : 'llo worl'
length : 8
capacity : 9

content : ''
length : 0
capacity : 25

content : ''
length : 0
capacity : 1

content : ''
length : 0
capacity : 32

res = 14
content : 'Hello 2 world!'
length : 14
capacity : 32
```
