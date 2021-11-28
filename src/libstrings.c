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

static struct meta *string_to_meta(const struct string *str)
{
        return (struct meta *)str - 1;
}

static struct string *meta_to_string(const struct meta *meta)
{
        return (struct string *)(meta + 1);
}

/**
 * @brief Sets the capacity of 'str' to 'capacity' bytes.
 * 
 * @return 0 on success.
 * @return -ENOMEM on failure.
 */
static int set_string_capacity(struct string *str, size_t capacity)
{
        struct meta *meta = string_to_meta(str);
        char *new_value = realloc(str->value, capacity);
        if (!new_value)
                return -ENOMEM;

        str->value = new_value;
        meta->capacity = capacity;
        return 0;
}

/**
 * @brief Sets the length of 'str' to 'len' characters excluding the null
 * terminating byte and reallocates if needed.
 * 
 * @return 0 on success.
 * @return -ENOMEM on failure.
 */
static int set_string_length(struct string *str, size_t len)
{
        struct meta *meta = string_to_meta(str);
        int res;

        if (meta->capacity < len + 1) {
                res = set_string_capacity(str, len * 2 + 1);
                if (res < 0)
                        return res;
        }
        
        meta->len = len;
        return 0;
}

/**
 * @brief Creates a string that can hold 'len' characters.
 * 
 * @return Pointer to the new string on success.
 * @return NULL on failure. 
 */
static struct string *create_string(size_t len)
{
        struct meta *meta = malloc(sizeof(*meta) + sizeof(struct string));
        if (!meta)
                goto error_alloc_meta;

        struct string *str = meta_to_string(meta);
        str->value = calloc(len + 1, sizeof(*str->value));
        if (!str->value)
                goto error_alloc_value;

        meta->len = len;
        meta->capacity = len + 1;
        return str;

        /* Errors */
error_alloc_value:
        free(meta);
error_alloc_meta:
        return NULL;
}

/**
 * @brief Creates a substring of the char array 'src' of 'len' characters
 * starting from 'start'.
 * 
 * @return Pointer to the new string on success.
 * @return NULL on failure.
 */
static struct string *sub_string(
                const char *src, unsigned int start, size_t len)
{
        struct string *str = create_string(len);
        if (!str)
                return NULL;

        strncpy(str->value, src + start, len);
        return str;
}

/**
 * @brief Copies the content of the char array 'src' of length 'len' into
 * 'dest'.
 * 
 * @return 0 on success.
 * @return -ENOMEM on failure.
 */
static int copy_string(struct string *dest, const char *src, size_t len)
{
        const int res = set_string_length(dest, len);
        if (res < 0)
                return res;

        strncpy(dest->value, src, len);
        dest->value[len] = '\0';
        return 0;
}

/**
 * @brief Appends the content of the char array 'src' of length 'len' at the end
 * of 'dest'.
 * 
 * @return 0 on success.
 * @return -ENOMEM on failure.
 */
static int append_string(struct string *dest, const char *src, size_t len)
{
        const size_t cur_len = string_to_meta(dest)->len;
        const int res = set_string_length(dest, cur_len + len);
        if (res < 0)
                return res;

        strncpy(dest->value + cur_len, src, len);
        dest->value[cur_len + len] = '\0';
        return 0;
}

/**
 * @brief Prepends the content of the char array 'src' of length 'len' at the
 * beginning of 'dest'.
 * 
 * @return 0 on success.
 * @return -ENOMEM on failure.
 */
static int prepend_string(struct string *dest, const char *src, size_t len)
{
        const size_t cur_len = string_to_meta(dest)->len;
        const int res = set_string_length(dest, cur_len + len);
        if (res < 0)
                return res;

        memmove(dest->value + len, dest->value, cur_len);
        strncpy(dest->value, src, len);
        dest->value[cur_len + len] = '\0';
        return 0;
}

/* API -----------------------------------------------------------------------*/

/* Creation functions ----------------*/

struct string *string_empty()
{
        return create_string(0);
}

struct string *string_dup(const struct string *src)
{
        if (!src)
                return NULL;

        return sub_string(src->value, 0, string_to_meta(src)->len);
}

struct string *string_dup_c(const char *src)
{
        if (!src)
                return NULL;

        return sub_string(src, 0, strlen(src));
}

struct string *string_dup_v(const char *src, size_t len)
{
        if (!src)
                return NULL;

        return sub_string(src, 0, len);
}

struct string *string_sub_v(const char *src, unsigned int start, size_t len)
{
        if (!src)
                return NULL;

        return sub_string(src, start, len);
}

/* Modification functions ------------*/

void string_destroy(const struct string *str)
{
        if (!str)
                return;

        free(str->value);
        free(string_to_meta(str));
}

int string_clear(struct string *str)
{
        if (!str)
                return -EINVAL;

        struct meta *meta = string_to_meta(str);
        meta->len = 0;
        str->value[0] = '\0';
        return 0;
}

int string_copy(struct string *dest, const struct string *src)
{
        if (!dest || !src)
                return -EINVAL;

        return copy_string(dest, src->value, string_to_meta(src)->len);
}

int string_copy_c(struct string *dest, const char *src)
{
        if (!dest || !src)
                return -EINVAL;

        return copy_string(dest, src, strlen(src));
}

int string_copy_v(struct string *dest, const char *src, size_t len)
{
        if (!dest || !src)
                return -EINVAL;

        return copy_string(dest, src, len);
}

int string_append(struct string *dest, const struct string *src)
{
        if (!dest || !src)
                return -EINVAL;

        return append_string(dest, src->value, string_to_meta(src)->len);
}

int string_append_c(struct string *dest, const char *src)
{
        if (!dest || !src)
                return -EINVAL;

        return append_string(dest, src, strlen(src));
}

int string_append_v(struct string *dest, const char *src, size_t len)
{
        if (!dest || !src)
                return -EINVAL;

        return append_string(dest, src, len);
}

int string_prepend(struct string *dest, const struct string *src)
{
        if (!dest || !src)
                return -EINVAL;

        return prepend_string(dest, src->value, string_to_meta(src)->len);
}

int string_prepend_c(struct string *dest, const char *src)
{
        if (!dest || !src)
                return -EINVAL;

        return prepend_string(dest, src, strlen(src));
}

int string_prepend_v(struct string *dest, const char *src, size_t len)
{
        if (!dest || !src)
                return -EINVAL;

        return prepend_string(dest, src, len);
}

int string_cut(struct string *str, unsigned int start, size_t len)
{
        if (!str)
                return -EINVAL;

        struct meta *meta = string_to_meta(str);
        if (meta->len < start + len)
                return -ERANGE;

        memmove(str->value, str->value + start, len);
        str->value[len] = '\0';
        meta->len = len;
        return 0;
}

int string_printf(struct string *str, const char *format, ...)
{
        if (!str || !format)
                return -EINVAL;

        va_list args;
        va_start(args, format);
        struct meta *meta = string_to_meta(str);

        const int res = vsnprintf(str->value, meta->capacity, format, args);
        meta->len = ((size_t)res < meta->capacity ? res : meta->capacity - 1);
        
        va_end(args);
        return res;
}

/* Utility functions -----------------*/

ssize_t string_len(const struct string *str)
{
        if (!str)
                return -EINVAL;

        return (ssize_t)string_to_meta(str)->len;
}

ssize_t string_capacity(const struct string *str)
{
        if (!str)
                return -EINVAL;

        return (ssize_t)string_to_meta(str)->capacity;
}

int string_reserve(struct string *str, size_t size)
{
        if (!str)
                return -EINVAL;

        if (size <= string_to_meta(str)->capacity)
                return 0;

        return set_string_capacity(str, size);
}

int string_fit(struct string *str)
{
        if (!str)
                return -EINVAL;

        return set_string_capacity(str, string_to_meta(str)->len + 1);
}
