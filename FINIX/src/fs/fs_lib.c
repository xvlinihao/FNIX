#include "fs_lib.h"

#include <stddef.h>


void *fs_memcpy(void *dest, const void *src, uint64_t n)
{
	char *dest_ptr = dest;
	const char *src_ptr = src;
	while (n-- > 0)
		*(dest_ptr++) = *(src_ptr++);
	return dest;
}
size_t fs_strcspn(const char *s, const char *reject)
{
	size_t size = 0;
	for(; *s != NULL; s ++, size ++)
		for(const char *ptr = reject; *ptr != NULL; ptr ++)
			if (*s == *ptr)
				return size;
	return size;
}
int fs_strcmp(const char *s1, const char *s2)
{
	while((*s1 == *s2) && (*s1 != '\0'))
	{
		s1 ++;
		s2 ++;
	}
	if (*s1 < *s2)
		return -1;
	else if (*s1 > *s2)
		return 1;
	else
		return 0;
}
wchar *fs_wstrncpy(wchar *dst, const wchar *src, uint64_t n)
{
	while(n -- >0 && (*dst ++ = *src ++) != 0x0000);
	return dst;
}
int fs_unicode_into_utf8(uint32_t unicode, char *utf8)
{
	uint64_t size;
	if(unicode <= 0x7f)
	{
		*utf8 = (unicode & 0x7f);
		size=1;
	}
	if(0x80 <= unicode && unicode <= 0x7ff)
	{
		*(utf8+1) = (unicode & 0x3f) | 0x80;
		*utf8 =((unicode>>6) & 0x1f) | 0xc0;
		size=2;
	}
	if(0x800 <= unicode && unicode <= 0xffff)
	{
		*(utf8+2) = (unicode & 0x3f) | 0x80;
		*(utf8+1) =((unicode>>6) & 0x3f) | 0x80;
		*utf8 =((unicode>>12) & 0x0f) | 0xe0;
		size=3;
	}
	if(0x10000 <= unicode && unicode <= 0x10ffff)
	{
		*(utf8+3) = (unicode & 0x3f) | 0x80;
		*(utf8+2) =((unicode>>6) & 0x3f) | 0x80;
		*(utf8+1) =((unicode>>12) & 0x3f) | 0x80;
		*utf8 =((unicode>>18) & 0x07) | 0xf0;
		size=4;
	}
	if(0x200000 <= unicode && unicode <= 0x3ffffff)
	{
		*(utf8+4) = (unicode & 0x3f) | 0x80;
		*(utf8+3) =((unicode>>6) & 0x3f) | 0x80;
		*(utf8+2) =((unicode>>12) & 0x3f) | 0x80;
		*(utf8+1) =((unicode>>18) & 0x3f) | 0x80;
		*utf8 =((unicode>>24) & 0x03) | 0xf8;
		size=5;
	}
	if(0x4000000 <= unicode && unicode<= 0x7fffffff)
	{
		*(utf8+5) = (unicode & 0x3f) | 0x80;
		*(utf8+4) =((unicode>>6) & 0x3f) | 0x80;
		*(utf8+3) =((unicode>>12) & 0x3f) | 0x80;
		*(utf8+2) =((unicode>>18) & 0x3f) | 0x80;
		*(utf8+1) =((unicode>>24) & 0x3f) | 0x80;
		*utf8 =((unicode>>30) & 0x01) | 0xfc;
		size=6;
	}
	return size;
}
int fs_str_unicode_to_utf8(uint16_t *unicode, char *utf8)
{
	uint64_t sum_size=0;
	for(; *unicode!= 0x0000; unicode++)
	{
		sum_size += fs_unicode_into_utf8(*unicode,utf8 + sum_size);
	}
	*(utf8 + sum_size) = '\0';
	return sum_size;
}
void *fs_memset(void *dest, char ch, uint64_t n)
{
	char *dest_ptr = dest;
	while(n-- > 0)
		*(dest_ptr++) = ch;
	return dest;
}
int fs_strlen(const char *str)
{
	int len = 0;
	while (*str++ != '\0')
		len ++;
	return len;
}
char *fs_strcpy(char *dst, const char *src)
{
	char *save = dst;
	while((*dst++ = *src++) != '\0');
	return save;
}
size_t fs_strspn(const char *s, const char *accept)
{
	size_t size = 0;
	const char *ptr;
	for(; *s != NULL; s ++, size ++)
	{
		for(ptr = accept; *s != *ptr && *ptr != NULL; ptr ++);
		if (*ptr == NULL)
			return size;
	}
	return size;
}
char *fs_strpbrk(const char *s, const char *accept)
{
	for (; *s != NULL; s++)
		for (const char *p = accept; *p != NULL; p ++)
			if (*s == *p)
				return (char *)s;
	return NULL;
}
char *fs_strchr(const char *s, int c)
{
	for(; *s != c && *s != NULL; s ++);
	return (char *)s;
}
char *fs_strtok_r(char *s, const char *delim, char **save_ptr)
{
	char *token;  
  
	if (s == NULL) s = *save_ptr;  
  
	/* Scan leading delimiters.  */  
	s += fs_strspn(s, delim);  
	if (*s == '\0')   
		return NULL;  
  
	/* Find the end of the token.  */  
	token = s;  
	s = fs_strpbrk(token, delim);  
	if (s == NULL)  
		/* This token finishes the string.  */  
		*save_ptr = fs_strchr(token, '\0');  
	else {  
		/* Terminate the token and make *SAVE_PTR point past it.  */  
		*s = '\0';  
		*save_ptr = s + 1;  
	}  
	return token;  
}
char *fs_strrchr(const char *s, int c)
{
	const char *rs = strlen(s) + s - 1;
	for(; rs != s - 1; rs --)
		if (*rs == c)
			return (char *)rs;
	return NULL;
}