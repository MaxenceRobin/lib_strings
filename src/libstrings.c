/**
 * @author Maxence ROBIN
 * @brief Provides a simple and safe strings manipulation API.
 */

/* Includes ------------------------------------------------------------------*/

#include "libstrings.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

/* Definitions ---------------------------------------------------------------*/

struct meta {
        size_t len;
        size_t capacity;
};

/* Static functions ----------------------------------------------------------*/

static struct meta *string_to_meta(const_string str)
{
        return (struct meta *)str - 1;
}

static string meta_to_string(const struct meta *meta)
{
        return (string)(meta + 1);
}

/**
 * @brief Sets the capacity of the given string in bytes.
 * 
 * @param str : Address of the string to reallocate.
 * @param capacity : Size in bytes to allocate for the given string.
 * @return int : 0 on success, in case of failure a negative errno is returned.
 */
static int set_string_capacity(string *str, size_t capacity)
{
        struct meta *meta = string_to_meta(*str);
        meta = realloc(meta, sizeof(*meta) + capacity);
        if (!meta)
                return -ENOMEM;

        meta->capacity = capacity;
        *str = meta_to_string(meta);
        return 0;
}

/**
 * @brief Resizes the string to fit size characters, excluding the '\0'
 * terminating byte.
 * 
 * @param str : Address of the string to resize.
 * @param len : Number of characters excluding the '\0' terminating byte that
 * must hold the string.
 * @return int : 0 on success, in case of failure a negative errno is returned.
 * 
 * @warning After calling this function, the associated meta may have to be
 * updated.
 */
static int set_string_length(string *str, size_t len)
{
        struct meta *meta = string_to_meta(*str);
        int res;

        if (meta->capacity < len + 1) {
                res = set_string_capacity(str, len * 2 + 1);
                if (res < 0)
                        return res;
        }
        
        // Re assigning meta since *str may have changed
        meta = string_to_meta(*str);
        meta->len = len;

        return 0;
}

static int cmp_with_null(const_string first, const_string second)
{
        if (!first && !second)
                return 0;
        else if (!first)
                return -1;
        else
                return 1;
}

/* API -----------------------------------------------------------------------*/

string string_empty()
{
        struct meta *meta = malloc(sizeof(*meta) + 1);
        if (!meta)
                return NULL;

        string str = meta_to_string(meta);
        meta->len = 0;
        meta->capacity = 0;
        str[0] = '\0';

        return str;
}

string string_dup(const_string src)
{
        if (!src)
                return NULL;

        return string_sub_v(src, 0, string_to_meta(src)->len);
}

string string_dup_c(const char *src)
{
        if (!src)
                return NULL;

        return string_sub_v(src, 0, strlen(src));
}

string string_dup_v(const char *src, size_t len)
{
        if (!src)
                return NULL;

        return string_sub_v(src, 0, len);
}

string string_sub_v(const char *src, unsigned int start, size_t len)
{
        if (!src)
                return NULL;

        struct meta *meta = malloc(sizeof(*meta) + len + 1);
        if (!meta)
                return NULL;

        string str = meta_to_string(meta);
        meta->len = len;
        meta->capacity = len + 1;
        strncpy(str, src + start, len);
        str[meta->len] = '\0';

        return str;
}

void string_destroy(const_string str)
{
        if (!str)
                return;

        free(string_to_meta(str));
}

int string_copy(string *dest, const_string src)
{
        if (!dest || !*dest || !src)
                return -EINVAL;

        return string_copy_v(dest, src, string_to_meta(src)->len);
}

int string_copy_c(string *dest, const char *src)
{
        if (!dest || !*dest || !src)
                return -EINVAL;

        return string_copy_v(dest, src, strlen(src));
}

int string_copy_v(string *dest, const char *src, size_t len)
{
        if (!dest || !*dest || !src)
                return -EINVAL;

        struct meta *meta = string_to_meta(*dest);
        const int res = set_string_length(dest, len);
        if (res < 0)
                return res;

        strncpy(*dest, src, len);
        (*dest)[len] = '\0';

        return 0;
}

int string_cat(string *dest, const_string src)
{
        if (!dest || !*dest || !src)
                return -EINVAL;

        return string_cat_v(dest, src, string_to_meta(src)->len);
}

int string_cat_c(string *dest, const char *src)
{
        if (!dest || !*dest || !src)
                return -EINVAL;

        return string_cat_v(dest, src, strlen(src));
}

int string_cat_v(string *dest, const char *src, size_t len)
{
        if (!dest || !*dest || !src)
                return -EINVAL;

        const size_t cur_len = string_to_meta(*dest)->len;
        const int res = set_string_length(dest, cur_len + len);
        if (res < 0)
                return res;

        strncpy(*dest + cur_len, src, len);
        (*dest)[cur_len + len] = '\0';

        return 0;
}

ssize_t string_len(const_string str)
{
        if (!str)
                return -EINVAL;

        return string_to_meta(str)->len;
}

ssize_t string_capacity(const_string str)
{
        if (!str)
                return -EINVAL;

        return string_to_meta(str)->capacity;
}

int string_reserve(string *str, size_t size)
{
        if (!str || !*str)
                return -EINVAL;

        if (string_to_meta(*str)->capacity >= size)
                return 0;

        return set_string_capacity(str, size);
}

int string_fit(string *str)
{
        if (!str || !*str)
                return -EINVAL;

        return set_string_capacity(str, string_to_meta(*str)->len + 1);
}
