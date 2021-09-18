# LIBSTRINGS

Simple C strings lib.

This lib provides simple manipulation of strings that ensures null termination.
The strings can still be manipulated with standard C libs.

## Example

```C
#include "libstrings.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_error(const char *msg, int error_code)
{
        fprintf(stderr, "%s : %s\n", msg, strerror(error_code));
}

int main()
{
        int status = EXIT_FAILURE;
        int res;

        string msg = string_dup_c("Hello world!");
        printf("%s\n", msg);

        string end = string_sub_v(msg, 5, 7);
        string bye = string_empty();
        res = string_copy_c(&bye, "Bye");
        if (res < 0) {
                print_error("Could not copy the string", -res);
                goto out;
        } else {
                printf("The string was successfully copied\n");
        }

        string_cat(&bye, end);
        printf("%s\n", bye);

        printf("Before fit : %lu / %lu\n",
                        string_len(bye), string_capacity(bye));

        string_fit(&bye);

        printf("After fit : %lu / %lu\n",
                        string_len(bye), string_capacity(bye));

        status = EXIT_SUCCESS;
out:
        string_destroy(msg);
        string_destroy(end);
        string_destroy(bye);

        return status;
}
```

Expected output :
```
Hello world!
The string was successfully copied
Bye world!
Before fit : 10 / 21
After fit : 10 / 11
```
