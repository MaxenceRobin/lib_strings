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

typedef char *string;
typedef const char *const_string;

/* API -----------------------------------------------------------------------*/

/* Creation functions ----------------*/

/**
 * @brief Creates an empty string.
 * 
 * @return string : An empty string with a length of 0.
 */
string string_empty();

/**
 * @brief Dupplicates the given string.
 * 
 * @param src : The string to dupplicate.
 * @return string : A string identical to the given one. In case of failure
 * NULL is returned.
 */
string string_dup(const_string src);

/**
 * @brief Dupplicates the given string literal.
 * 
 * @param src : The string literal to dupplicate.
 * @return string : A string identical to the given one. In case of failure
 * NULL is returned.
 * 
 * @note This version uses strlen() to determine the string literal length.
 * It can be better for performances to use string_dup_v() coupled with sizeof()
 * instead.
 */
string string_dup_c(const char *src);

/**
 * @brief Dupplicates the given char array.
 * 
 * @param src : The char array to dupplicate.
 * @param len : Length of the char array to dupplicate.
 * @return string : A string identical to the given one. In case of failure
 * NULL is returned.
 */
string string_dup_v(const char *src, size_t len);

/**
 * @brief Creates a subtring from the given char array.
 * 
 * @param src : Char array from which to create the substring.
 * @param start : Position to start the substring from, first index is 0.
 * @param len : Number of characters to include in the substring.
 * @return string : A string of len characters taken from the given char array.
 * 
 * @warning No check is made on src for the '\0' terminating byte, start and len
 * have to be correctly used.
 */
string string_sub_v(const char *src, unsigned int start, size_t len);

/* Modification functions ------------*/

/**
 * @brief Destroys a string.
 * 
 * @param str : String to destroy
 */
void string_destroy(const_string str);

/**
 * @brief Clears a string, making it empty.
 * 
 * @param str : String to clear.
 * @return int : 0 on success, a negative errno on failure.
 */
int string_clear(string str);

/**
 * @brief Copies the content of a string into another one.
 * 
 * @param dest : Address of the string in which to copy the content into.
 * @param src : String to copy.
 * @return int : 0 on success, in case of failure a negative errno is returned.
 */
int string_copy(string *dest, const_string src);

/**
 * @brief Copies the content of a string literal into a string.
 * 
 * @param dest : Address of the string in which to copy the content into.
 * @param src : String literal to copy.
 * @return int : 0 on success, in case of failure a negative errno is returned.
 * 
 * @note This version uses strlen() to determine the string literal length.
 * It can be better for performances to use string_copy_v() coupled with
 * sizeof() instead.
 */
int string_copy_c(string *dest, const char *src);

/**
 * @brief Copies the content of a char array into a string.
 * 
 * @param dest : Address of the string in which to copy the content into.
 * @param src : Char array to copy.
 * @param len : Length of the char array to copy.
 * @return int : 0 on success, in case of failure a negative errno is returned.
 */
int string_copy_v(string *dest, const char *src, size_t len);

/**
 * @brief Appends the content of a string at the end of another one.
 * 
 * @param dest : Address of the string to append the content to.
 * @param src : String to append.
 * @return int : 0 on success, in case of failure a negative errno is returned.
 */
int string_append(string *dest, const_string src);

/**
 * @brief Appends the content of a string literal at the end of a string.
 * 
 * @param dest : Address of the string to append the content to.
 * @param src : String literal to append.
 * @return int : 0 on success, in case of failure a negative errno is returned.
 * 
 * @note This version uses strlen() to determine the string literal length.
 * It cas be better for performances to use string_cat_v() coupled with sizeof
 * instead.
 */
int string_append_c(string *dest, const char *src);

/**
 * @brief Appends the content of a char array at the end of a string.
 * 
 * @param dest : Address of the string to append the content to.
 * @param src : Char array to append.
 * @param len : Length of the char array to append.
 * @return int : 0 on success, in case of failure a negative errno is returned.
 */
int string_append_v(string *dest, const char *src, size_t len);

/**
 * @brief Appends the content of a string at the end of another one.
 * 
 * @param dest : Address of the string to append the content to.
 * @param src : String to append.
 * @return int : 0 on success, in case of failure a negative errno is returned.
 */
int string_prepend(string *dest, const_string src);

/**
 * @brief Appends the content of a string literal at the end of a string.
 * 
 * @param dest : Address of the string to append the content to.
 * @param src : String literal to append.
 * @return int : 0 on success, in case of failure a negative errno is returned.
 * 
 * @note This version uses strlen() to determine the string literal length.
 * It cas be better for performances to use string_cat_v() coupled with sizeof
 * instead.
 */
int string_prepend_c(string *dest, const char *src);

/**
 * @brief Appends the content of a char array at the end of a string.
 * 
 * @param dest : Address of the string to append the content to.
 * @param src : Char array to append.
 * @param len : Length of the char array to append.
 * @return int : 0 on success, in case of failure a negative errno is returned.
 */
int string_prepend_v(string *dest, const char *src, size_t len);

/**
 * @brief Cuts the substring to a subpart of what it contains.
 * 
 * @param str : String to cut.
 * @param start : Position to start to cut from, first index is 0.
 * @param len : Number of characters to cut.
 * @return int : 0 on success, a negative errno in case of failure.
 */
int string_cut(string *str, unsigned int start, size_t len);

/**
 * @brief Writes the formatted input into the string.
 * 
 * @param str : String in which to write the formatted input
 * @param format : Format to apply to the given arguments.
 * @param ... : Arguments to format.
 * @return int : Number of written bytes into the string on success, a negative
 * errno on failure.
 * 
 * @note If the output was truncated, the return valud is the number of
 * chraracters which would have been written to the string if enough space had
 * been available.
 */
int string_printf(string str, const char *format, ...);

/* Utility functions -----------------*/
/**
 * @brief Returns the length of the string.
 * 
 * @param str : String for which to get the length.
 * @return ssize_t : The length of the given string, if the given string is
 * NULL -EINVAL is returned.
 */
ssize_t string_len(const_string str);

/**
 * @brief Returns the capacity of the string.
 * 
 * @param str : String for which to get the capacity.
 * @return ssize_t : The capacity of the given string, if the given string is
 * NULL -EINVAL is returned.
 */
ssize_t string_capacity(const_string str);

/**
 * @brief Reserves extra memory for the string in order to avoid future
 * reallocation. If the given size is lower than the current string size, this
 * function does nothing.
 * 
 * @param str : Address of the string to reserve memory for.
 * @param size : Number of bytes to reserve for the string.
 * @return int : 0 on success, in case of failure a negative errno is returned.
 */
int string_reserve(string *str, size_t size);

/**
 * @brief Reduces the capacity of the string in order to match the current
 * string length.
 * 
 * @param str : Address of the string to resize.
 * @return int : 0 on success, in case of failure a negative errno is returned.
 */
int string_fit(string *str);

#endif /* LIB_STRINGS_H */
