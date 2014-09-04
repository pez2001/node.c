/* 
 * node.c - c based data tree with various imports & exports
 * (c) 2013 by Tim Theede aka Pez2001 <pez2001@voyagerproject.de> / vp
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * THIS SOFTWARE IS SUPPLIED AS IT IS WITHOUT ANY WARRANTY!
 *
 */

#ifndef NODE_STRINGS_H
#define NODE_STRINGS_H


#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "ctype.h"

#ifdef USE_MEMORY_DEBUGGING
#include "memory.h"
#endif



/**
 * str_FromLong() - Converts a Long to a String
 * @i: Long to be converted
 *
 * Converts the Long to a String and returns it
 *
 * Return: String with converted long as literals
 */
char *str_FromLong(long i);

/**
 * str_FromDouble() - Converts a Double to a String
 * @d: Double to be converted
 *
 * Converts the Double to a String and returns it
 *
 * Return: String with converted double as literals
 */
char *str_FromDouble(double d);

/**
 * str_Trim() - Trims a string
 * @string: Input string
 *
 * Trims whitespaces and tabs from the start and ending of the string
 * and returns it
 * Note: the input string gets freed and is != NULL
 * input can be NULL ,an empty string will be returned
 * Return: Trimmed string
 */
char *str_Trim(char *string);

/**
 * str_Copy() - Copies a String
 * @src: Source String to Copy (can be NULL)
 *
 * Copies Source String and returns it
 * if src is NULL an Empty NULL-terminated string is returned instead
 *
 * Return: Copy of Source String or Empty String
 */
char *str_Copy(char *src);

/**
 * str_Cat() - Concate two strings
 * @a: Input string
 * @b: String to append
 *
 * Creates a new string with a and b joined together and returns it
 * Note: both input string can be NULL 
 *
 * Return: New concatenated string 
 */
char *str_Cat(char *a,char *b);

/**
 * str_CatFree() - Concate two strings and freeing string a
 * @a: Input String (which will automatically be freed)
 * @b: String to append
 *
 * Appends the string b to a and returns it 
 * Note: if a is != NULL it is freed automatically
 * b will be left as it is
 * both input string can be NULL 
 *
 * Example usage:
 * char *a="Hello";
 * a=str_CatFree(a," World");
 *
 * Return: Concatenated string
 */
char *str_CatFree(char *a,char *b);

/**
 * str_Mult() - Create a Multiple of a String
 * @a: String to multiply
 * @count: Number of repititions
 *
 * Create a new string with multiple copies of string a concatenated
 * Note: if count==0 or a == NULL it returns an empty String
 *
 * Return: String with multiple copies of input concatenated
 */
char *str_Mult(char *a,long count);

/**
 * str_MatchCount() - Returns the number of matching Characters in both strings
 * @a: String to count matching characters in
 * @b: String to match against
 *
 * Counts the matching characters from the beginning of both strings and returns it
 *
 * Return: Count of matching characters in both strings
 */
long str_MatchCount(char *a,char *b);

/**
 * str_Sub() - Create a Substring
 * @a: Source String
 * @start: Start Index
 * @len: Max Length of final Substring
 *
 * Creates a Substring of the source string
 * Note:May return less characters if the source string is not long enough
 * - (start+len>strlen(a))
 * or start is larger than the length of the source string
 * len can be -1 to copy the remaining string
 *
 * Return: Substring of a
 */
char *str_Sub(char *a,long start,long len);

/**
 * str_AddChar() - Adds a character to a string
 * @string: Input string
 * @letter: Character to append
 *
 * Appends the character to the string and returns it
 * Note: the returned string pointer may differ from  
 * the input string pointer due to usage of realloc()
 * string can be NULL
 *
 * Return: String with appended character
 */
char *str_AddChar(char *string,char letter);

/**
 * str_AddChars() - Adds multiple characters to a string
 * @string: Input string
 * @append: Characters to append
 * @len: Number of Characters to append
 *
 * Appends the characters to the string and returns it
 * Note: the returned string pointer may differ from  
 * the input string pointer due to usage of realloc()
 * string can be NULL
 *
 * Return: String with appended characters
 */
char *str_AddChars(char *string,char *append,long len);

/**
 * str_Replace() - Creates new String with the first ocurrence of the search string replaced 
 * @hay: String to search in
 * @needle: String to search for
 * @diamond: Replacement String
 * 
 * Replaces the first ocurrence of the needle in the hay with the diamond 
 * Note any parameter can be null 
 * at least an empty string will be returned
 *
 * Return: a modified string
 */
char *str_Replace(char *hay,char *needle,char*diamond);

/**
 * str_IndexOf() - Returns the Index of the search string 
 * @hay: String to search in
 * @needle: String to search for
 * 
 * Returns the index of the search string in hay
 * if needle was not found the position will be returned
 * else -1 will be returned
 *
 * Return: index
 */
long str_IndexOf(char *hay,char *needle);

/**
 * str_LastIndexOf() - Returns the last Index of the search string 
 * @hay: String to search in
 * @needle: String to search for
 * 
 * Returns the index of the search string in hay
 * if needle was not found the position will be returned
 * else -1 will be returned
 *
 * Return: index
 */
long str_LastIndexOf(char *hay,char *needle);

/**
 * str_CreateEmpty() - Create an empty String
 *
 * Creates an zero length NULL-terminated string
 * 
 * Return: empty string
 */
char *str_CreateEmpty(void);


#endif
