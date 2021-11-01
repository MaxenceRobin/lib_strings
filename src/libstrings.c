/**
 * @author Maxence ROBIN
 * @brief Provides a simple and safe strings manipulation API.
 */

/* Includes ------------------------------------------------------------------*/

#include "libstrings.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
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

/**
 * @brief Creates a substring from the given const char array.
 * 
 * @param src : Const char array to create the substring from.
 * @param start : Starting position of the substring withing the given string.
 * @param len : Length of the substring to create.
 * @return string : A substring of the given one on success, NULL on failure.
 */
static string sub_string(const char *src, unsigned int start, size_t len)
{
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

/**
 * @brief Copies a const char array into a string.
 * 
 * @param dest : String in which to copy the char array/
 * @param src : Const char array to copy.
 * @param len : Length of the const char array.
 * @return int : 0 on success, a negative errno on failure.
 */
static int copy_string(string *dest, const char *src, size_t len)
{
        struct meta *meta = string_to_meta(*dest);
        const int res = set_string_length(dest, len);
        if (res < 0)
                return res;

        strncpy(*dest, src, len);
        (*dest)[len] = '\0';

        return 0;
}

/**
 * @brief Appends a string at the end of another string.
 * 
 * @param dest : String to append the other string to.
 * @param src : Char array to append at the end of the other string.
 * @param len : Length of the char array.
 * @return int : 0 on success, a negative errno on failure.
 */
int append_string(string *dest, const char *src, size_t len)
{
        const size_t cur_len = string_to_meta(*dest)->len;
        const int res = set_string_length(dest, cur_len + len);
        if (res < 0)
                return res;

        strncpy(*dest + cur_len, src, len);
        (*dest)[cur_len + len] = '\0';

        return 0;
}

/**
 * @brief Prepends a string at the beginning of another string.
 * 
 * @param dest : String to prepend the other string to.
 * @param src : Char array to prepend at the beginning of the other string.
 * @param len : Length of the char array.
 * @return int : 0 on success, a negative errno on failure.
 */
int prepend_string(string *dest, const char *src, size_t len)
{
        const size_t cur_len = string_to_meta(*dest)->len;
        const int res = set_string_length(dest, cur_len + len);
        if (res < 0)
                return res;

        memmove(*dest + len, *dest, cur_len);
        strncpy(*dest, src, len);
        (*dest)[cur_len + len] = '\0';

        return 0;
}

/* API -----------------------------------------------------------------------*/

/* Creation functions ----------------*/

string string_empty()
{
        struct meta *meta = malloc(sizeof(*meta) + 1);
        if (!meta)
                return NULL;

        string str = meta_to_string(meta);
        meta->len = 0;
        meta->capacity = 1;
        str[0] = '\0';

        return str;
}

string string_dup(const_string src)
{
        if (!src)
                return NULL;

        return sub_string(src, 0, string_to_meta(src)->len);
}

string string_dup_c(const char *src)
{
        if (!src)
                return NULL;

        return sub_string(src, 0, strlen(src));
}

string string_dup_v(const char *src, size_t len)
{
        if (!src)
                return NULL;

        return sub_string(src, 0, len);
}

string string_sub_v(const char *src, unsigned int start, size_t len)
{
        if (!src)
                return NULL;

        return sub_string(src, start, len);
}

/* Modification functions ------------*/

void string_destroy(const_string str)
{
        if (!str)
                return;

        free(string_to_meta(str));
}

int string_clear(string str)
{
        if (!str)
                return -EINVAL;

        struct meta *meta = string_to_meta(str);
        meta->len = 0;
        str[0] = '\0';

        return 0;
}

int string_copy(string *dest, const_string src)
{
        if (!dest || !*dest || !src)
                return -EINVAL;

        return copy_string(dest, src, string_to_meta(src)->len);
}

int string_copy_c(string *dest, const char *src)
{
        if (!dest || !*dest || !src)
                return -EINVAL;

        return copy_string(dest, src, strlen(src));
}

int string_copy_v(string *dest, const char *src, size_t len)
{
        if (!dest || !*dest || !src)
                return -EINVAL;

        return copy_string(dest, src, len);
}

int string_append(string *dest, const_string src)
{
        if (!dest || !*dest || !src)
                return -EINVAL;

        return append_string(dest, src, string_to_meta(src)->len);
}

int string_append_c(string *dest, const char *src)
{
        if (!dest || !*dest || !src)
                return -EINVAL;

        return append_string(dest, src, strlen(src));
}

int string_append_v(string *dest, const char *src, size_t len)
{
        if (!dest || !*dest || !src)
                return -EINVAL;

        return append_string(dest, src, len);
}

int string_prepend(string *dest, const_string src)
{
        if (!dest || !*dest || !src)
                return -EINVAL;

        return prepend_string(dest, src, string_to_meta(src)->len);
}

int string_prepend_c(string *dest, const char *src)
{
        if (!dest || !*dest || !src)
                return -EINVAL;

        return prepend_string(dest, src, strlen(src));
}

int string_prepend_v(string *dest, const char *src, size_t len)
{
        if (!dest || !*dest || !src)
                return -EINVAL;

        return prepend_string(dest, src, len);
}

int string_cut(string *str, unsigned int start, size_t len)
{
        if (!str || !*str)
                return -EINVAL;

        struct meta *meta = string_to_meta(*str);
        if (meta->len < start + len)
                return -ERANGE;

        memmove(*str, *str + start, len);
        (*str)[len] = '\0';
        meta->len = len;

        return 0;
}

int string_printf(string str, const char *format, ...)
{
        if (!str)
                return -EINVAL;

        va_list args;
        va_start(args, format);
        struct meta *meta = string_to_meta(str);

        const int res = vsnprintf(str, meta->capacity, format, args);
        meta->len = ((size_t)res < meta->capacity ? res : meta->capacity - 1);
        
        va_end(args);
        return res;
}

/* Utility functions -----------------*/

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
