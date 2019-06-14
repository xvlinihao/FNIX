#ifndef FS_LIB_H
#define FS_LIB_H

#include <stdint.h>

typedef uint64_t 		size_t;
typedef uint16_t		wchar;


void *fs_memcpy(void *dest, const void *src, uint64_t n);
size_t fs_strcspn(const char *s, const char *reject);
int fs_strcmp(const char *s1, const char *s2);
wchar *fs_wstrncpy(wchar *dst, const wchar *src, uint64_t n);
int fs_unicode_into_utf8(uint32_t unicode, char *utf8);
int fs_str_unicode_to_utf8(uint16_t *unicode, char *utf8);
void *fs_memset(void *dest, char ch, uint64_t n);
int fs_strlen(const char *str);
char *fs_strcpy(char *dst, const char *src);
size_t fs_strspn(const char *s, const char *accept);
char *fs_strpbrk(const char *s, const char *accept);
char *fs_strchr(const char *s, int c);
char *fs_strtok_r(char *s, const char *delim, char **save_ptr);
char *fs_strrchr(const char *s, int c);
#endif