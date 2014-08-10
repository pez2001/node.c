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

#include "strings.h"

char *str_FromLong(long i)
{
  char *ret=NULL;
  long len = snprintf(NULL,0,"%ld",i);
  if(len)
  {
    ret = (char*)malloc(len+1);
    snprintf(ret,len+1,"%ld",i);
  }
  else
    ret=str_CreateEmpty();
  return(ret);
} 

char *str_FromDouble(double d)
{
  char *ret=NULL;
  long len = snprintf(NULL,0,"%13g",d);
  if(len)
  {
    ret = (char*)malloc(len+1);
    snprintf(ret,len+1,"%13g",d);
  }
  else
    ret=str_CreateEmpty();
  return(ret);
} 

char *str_Trim(char *string)
{
  if(string==NULL)
  	return(str_CreateEmpty());
  long len = strlen(string);
  if(!len)
    return(string);
  long s=0;
  long e=len-1;
  while(s<len && (isspace(string[s]) || string[s]=='\t'))s++;
  while(e>0 && (isspace(string[e]) || string[s]=='\t'))e--;
  e=e+1;
  long nlen = e-s;
  if(nlen<=0)
  {
    free(string);
    return(str_CreateEmpty());
  }
  if(nlen==len)
    return(string);
  char *r = (char*)malloc(nlen+1);
  memcpy(r,string+s,nlen);
  r[nlen] = 0;
  free(string);
  return(r);
}

char *str_Copy(char *src)
{
  if(src==NULL)
  	return(str_CreateEmpty());
  //	return(NULL);
  char *a = (char*)malloc(strlen(src)+1);
  memcpy(a, src, strlen(src)+1);
  return(a);
}

char *str_Cat(char *a,char *b)
{
  if(a == NULL && b != NULL)
    return(str_Copy(b));
  else
    if(a != NULL && b == NULL)
      return(str_Copy(a));
  else
    if(a == NULL && b == NULL)
      return(str_CreateEmpty());
  char *tmp = (char*)malloc(strlen(a) + strlen(b) + 1);
  memcpy(tmp, a, strlen(a));
  memcpy(tmp + strlen(a), b, strlen(b)+1);
  return(tmp);
}

char *str_CatFree(char *a,char *b)
{
  if(a == NULL && b != NULL)
    return(str_Copy(b));
  else
    if(a != NULL && b == NULL)
      return(a);
  else
    if(a == NULL && b == NULL)
      return(str_CreateEmpty());
  char *tmp = (char*)malloc(strlen(a) + strlen(b) + 1);
  memcpy(tmp, a, strlen(a));
  memcpy(tmp + strlen(a), b, strlen(b)+1);
  free(a);
  return(tmp);
}

char *str_Mult(char *a,long count)
{
  if(a == NULL || !count)
      return(str_CreateEmpty());
  char *tmp = (char*)malloc((strlen(a)*count) + 1);
  long i=0;
  for(i=0;i<count;i++)
  {
    memcpy(tmp+(i*strlen(a)), a, strlen(a));
  }
  memset(tmp+(strlen(a)*count) + 0, 0, 1);
  return(tmp);
}

long str_MatchCount(char *a,char *b)
{
  if(a==NULL || b==NULL)
    return(0);
  long i=0;
  long alen=strlen(a);
  long blen=strlen(b);
  long match_max_len = alen;
  if(blen<match_max_len)
    match_max_len=blen;
  for(i=0;i<match_max_len;i++)
  {
    if(a[i]!=b[i])
      return(i);
  }
  return(match_max_len);
}

char *str_Sub(char *a,long start,long len)
{
  if(a==NULL || strlen(a)<=start)
  {
  	return(str_CreateEmpty());
  }
  long e = start + len;
  long l=len;
  if(e>strlen(a))
    l = strlen(a) - start;
  char *tmp = (char*)malloc(l + 1);
  memset(tmp+l, 0, 1);
  memcpy(tmp, a+start, l);
  return(tmp);
}

char *str_AddChar(char *string,char letter)
{ 
  if(string==NULL)
  {
    string = (char*)malloc(2);
    string[0] = letter;
    string[1] = 0;
    return(string);
  }
  int len=strlen(string);
  string = (char*)realloc(string,len+2);
  string[len+1] = 0;
  string[len] = letter;
  return(string);
}

char *str_AddChars(char *string,char *append,long len)
{
  if(string==NULL)
  {
    string = (char*)malloc(len+1);
    memcpy(string,append,len);
    string[len] = 0;
    return(string);
  }
  int slen=strlen(string);
  string = (char*)realloc(string,slen+len+1);
  string[slen+len] = 0;
  memcpy(string+slen,append,len);
  return(string);
}


char *str_CreateEmpty(void)
{
    char *string = (char*)malloc(1);
    string[0] = 0;
    return(string);
}

