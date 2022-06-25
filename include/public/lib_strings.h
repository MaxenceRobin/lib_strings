/**
 * @author Maxence ROBIN
 * @brief Provides a simple and safe strings manipulation API.
 */

#ifndef LIB_STRINGS_H
#define LIB_STRINGS_H

/* Includes ------------------------------------------------------------------*/

#include <stddef.h>
#include <sys/types.h>

/* Definitions ---------------------------------------------------------------*/

struct string {
    char *value;
};

/* API -----------------------------------------------------------------------*/

/* Creation functions ----------------*/

/**
 * @brief Creates an empty string.
 *
 * @return Pointer to the new string on success.
 * @return NULL on failure.
 */
struct string *string_empty();

/**
 * @brief Dupplicates 'src'.
 *
 * @return Pointer to the new string on success.
 * @return NULL on failure.
 */
struct string *string_dup(const struct string *src);

/**
 * @brief Dupplicates the given string literal 'src'.
 *
 * @return Pointer to the new string on success.
 * @return NULL on failure.
 *
 * @note This version uses strlen() to determine the length of 'src'. It can be
 * better for performances to use string_dup_v() coupled with sizeof() instead.
 */
struct string *string_dup_c(const char *src);

/**
 * @brief Dupplicates the char array 'src' of length 'len'.
 *
 * @return Pointer to the new string on success.
 * @return NULL on failure.
 */
struct string *string_dup_v(const char *src, size_t len);

/**
 * @brief Creates a subtring of the char array 'src'.
 *
 * @return Pointer to the new string on success.
 * @return NULL on failure.
 *
 * @warning No check is made on src for the '\0' terminating byte, 'start' and
 * 'len' have to be correctly used.
 */
struct string *string_sub_v(const char *src, unsigned int start, size_t len);

/**
 * @brief Creates a string from a printf() format 'format' with the exact
 * required size.
 *
 * @return Pointer to the new string on success.
 * return NULL on failure or if 'format' is invalid.
 */
struct string *string_format(const char *format, ...);

/* Modification functions ------------*/

/**
 * @brief Destroys 'str'.
 */
void string_destroy(const struct string *str);

/**
 * @brief Makes 'str' empty.
 *
 * @return 0 on success.
 * @return -EINVAL if 'str' is invalid.
 */
int string_clear(struct string *str);

/**
 * @brief Copies the content of 'src' into 'dest'.
 *
 * @return 0 on success.
 * @return -EINVAL if 'dest' or 'src' are invalid.
 * @return -ENOMEM on failure.
 */
int string_copy(struct string *dest, const struct string *src);

/**
 * @brief Copies the content the string literal 'src' into 'dest'.
 *
 * @return 0 on success.
 * @return -EINVAL if 'dest' or 'src' are invalid.
 * @return -ENOMEM on failure.
 *
 * @note This version uses strlen() to determine the length of 'src'. It can be
 * better for performances to use string_copy_v() coupled with sizeof() instead.
 */
int string_copy_c(struct string *dest, const char *src);

/**
 * @brief Copies the content of the char array 'src' of length 'len' into
 * 'dest'.
 *
 * @return 0 on success.
 * @return -EINVAL if 'dest' or 'src' are invalid.
 * @return -ENOMEM on failure.
 */
int string_copy_v(struct string *dest, const char *src, size_t len);

/**
 * @brief Appends the content of 'src' at the end of 'dest'.
 *
 * @return 0 on success.
 * @return -EINVAL if 'dest' or 'src' are invalid.
 * @return -ENOMEM on failure.
 */
int string_append(struct string *dest, const struct string *src);

/**
 * @brief Appends the content of the string literal 'src' at the end of 'dest'.
 *
 * @return 0 on success.
 * @return -EINVAL if 'dest' or 'src' are invalid.
 * @return -ENOMEM on failure.
 *
 * @note This version uses strlen() to determine the length of 'src'. It can be
 * better for performances to use string_append_v() coupled with sizeof instead.
 */
int string_append_c(struct string *dest, const char *src);

/**
 * @brief Appends the content of the char array 'src' of length 'len' at the end
 * of 'dest'.
 *
 * @return 0 on success.
 * @return -EINVAL if 'dest' or 'src' are invalid.
 * @return -ENOMEM on failure.
 */
int string_append_v(struct string *dest, const char *src, size_t len);

/**
 * @brief Prepends the content of 'src' at the beginning of 'dest'.
 *
 * @return 0 on success.
 * @return -EINVAL if 'dest' or 'src' are invalid.
 * @return -ENOMEM on failure.
 */
int string_prepend(struct string *dest, const struct string *src);

/**
 * @brief Prepends the content of the string literal 'src' at the beginning of
 * 'dest'.
 *
 * @return int : 0 on success, in case of failure a negative errno is returned.
 *
 * @note This version uses strlen() to determine the length of 'src'. It can be
 * better for performances to use string_prepend_v() coupled with sizeof
 * instead.
 */
int string_prepend_c(struct string *dest, const char *src);

/**
 * @brief Prepends the content of the char array 'src' of length 'len' at the
 * beginning of 'dest'.
 *
 * @return 0 on success.
 * @return -EINVAL if 'dest' or 'src' are invalid.
 * @return -ENOMEM on failure.
 */
int string_prepend_v(struct string *dest, const char *src, size_t len);

/**
 * @brief Replaces the content of 'str' by a sub string of itself.
 *
 * @return 0 on success.
 * @return -EINVAL if 'str' is invalid.
 * @return -ERANGE if 'start' + 'len' is greater than the length of 'str'.
 */
int string_cut(struct string *str, unsigned int start, size_t len);

/**
 * @brief Writes the formatted input into 'str'.
 *
 * @return The number of written bytes into the string on success.
 * @return -EINVAL if 'str' or format are invalid.
 *
 * @note If the output was truncated, the return value is the number of
 * chraracters which would have been written to 'str' if enough space had
 * been available.
 */
int string_printf(struct string *str, const char *format, ...);

/* Utility functions -----------------*/
/**
 * @brief Returns the length of 'str'.
 *
 * @return The length of 'str' on success.
 * @return -EINVAL if 'str' is invalid.
 */
ssize_t string_len(const struct string *str);

/**
 * @brief Returns the capacity of 'str'.
 *
 * @return The capacity of 'str' on success.
 * @return -EINVAL if 'str' is invalid.
 */
ssize_t string_capacity(const struct string *str);

/**
 * @brief Reserves 'size' bytes for 'str' in order to avoid future reallocation.
 *
 * @return 0 on success.
 * @return -EINVAL if 'str' is invalid.
 * @return -ENOMEM on failure.
 *
 * @note If 'size' is lower than the capacity of 'str', this function does
 * nothing.
 */
int string_reserve(struct string *str, size_t size);

/**
 * @brief Reduces the capacity of 'str' in order to match its length.
 *
 * @return 0 on success.
 * @return -EINVAL if 'str' is invalid.
 * @return -ENOMEM on failure.
 */
int string_fit(struct string *str);

#endif /* LIB_STRINGS_H */
