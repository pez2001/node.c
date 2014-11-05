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

char *str_Base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
char *str_base64url_Chars= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

char str_Base64Char(int index)
{
  return(str_Base64Chars[index]);
}

int str_IndexOfBase64Char(char b64_char)
{
  int count = strlen(str_Base64Chars);
  for(int i=0;i<count;i++)
    if(b64_char == str_Base64Chars[i])
      return(i);
  return(-1);
}

char *str_EncodeBase64(char *data,unsigned long len)
{
  char *encoded_data = str_CreateEmpty();
  unsigned long in_offset = 0;
  unsigned long out_len = 0;
  unsigned long padding = 0;
  while(in_offset < len)
  {
    unsigned char in1 = 0;
    unsigned char in2 = 0;
    unsigned char in3 = 0;
    unsigned long in = 0;
    if(in_offset<len)
      in1 = data[in_offset++];
    if(in_offset<len)
      in2 = data[in_offset++];
    else
      padding++;
    if(in_offset<len)
      in3 = data[in_offset++];
    else
      padding++;
    in = in3;
    in |= in2<<8;
    in |= in1<<16;
 
    unsigned char out1 = in&63;
    unsigned char out2 = (in>>6)&63;
    unsigned char out3 = (in>>12)&63;
    unsigned char out4 = (in>>18)&63;
    encoded_data = str_AddChar(encoded_data,str_Base64Char(out4));
    encoded_data = str_AddChar(encoded_data,str_Base64Char(out3));
    out_len+=2;
    if(padding<2)
    {
      encoded_data = str_AddChar(encoded_data,str_Base64Char(out2));
      out_len++;
    }
    if(!padding)
    {
      encoded_data = str_AddChar(encoded_data,str_Base64Char(out1));
      out_len++;
    }
    if(!(out_len%76))
      encoded_data = str_CatFree(encoded_data,"\r\n");

    for(unsigned long p=0;p<padding;p++)
      encoded_data = str_AddChar(encoded_data,'=');
  }
  return(encoded_data);
}

int str_DecodeBase64(char *data,char **decoded,unsigned long *len)
{
  int r = 0;
  unsigned long encoded_len = strlen(data);
  unsigned long offset = 0;
  unsigned long out_offset = 0;
  unsigned long padding = 0;
  if(data[encoded_len-1]=='=')
    padding++;
  if(data[encoded_len-2]=='=')
    padding++;
  unsigned long decoded_len = (((encoded_len)/4)*3)-padding;
  *len = decoded_len;
  char *decoded_data = (char*)malloc(decoded_len);
  while(offset<encoded_len)
  {
    unsigned char in1 = 0;
    unsigned char in2 = 0;
    unsigned char in3 = 0;
    unsigned char in4 = 0;
    while(data[offset]=='\r' || data[offset]=='\n')
      offset++;
    in1 = str_IndexOfBase64Char(data[offset++]);
    while(data[offset]=='\r' || data[offset]=='\n')
      offset++;
    in2 = str_IndexOfBase64Char(data[offset++]);
    while(data[offset]=='\r' || data[offset]=='\n')
      offset++;
    in3 = str_IndexOfBase64Char(data[offset++]);
    while(data[offset]=='\r' || data[offset]=='\n')
      offset++;
    in4 = str_IndexOfBase64Char(data[offset++]);
    unsigned long out = 0;
    if(in4!=255)
      out = (in4&63);
    if(in3!=255)
    out |= (in3&63)<<6;
    out |= (in2&63)<<12;
    out |= (in1&63)<<18;
    if(in3!=255)
      decoded_data[out_offset++] = (out>>16)&255;
    if(in4!=255)
      decoded_data[out_offset++] = (out>>8)&255;
    decoded_data[out_offset++] = out&255;
  }
  *decoded = decoded_data;
  return(r);
}


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
  //long len = snprintf(NULL,0,"%13g",d);
  //printf("converting double:%f\n",d);
  long len = snprintf(NULL,0,"%f",d);
  if(len)
  {
    ret = (char*)malloc(len+1);
    //snprintf(ret,len+1,"%13g",d);
    snprintf(ret,len+1,"%f",d);
  }
  else
    ret=str_CreateEmpty();
  //printf("converted double to:[%s]\n",ret);
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
  if(a==NULL || (long)strlen(a)<=start)
  {
  	return(str_CreateEmpty());
  }
  long l=0;
  if(len>0)
  {
    long e = start + len;
    l=len;
    if(e>(long)strlen(a))
      l = strlen(a) - start;
  }
  else
  {
    l = strlen(a) - start;
  }
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

char *str_Replace(char *hay,char *needle,char*diamond)
{
  if(!hay)
    return(str_CreateEmpty());
  if(!strlen(hay) || !needle || !strlen(needle))
    return(str_Copy(hay));
  char *pos = strstr(hay,needle);
  if(pos)
  {
    long index = (long)(pos-hay);
    unsigned long total_len = (strlen(hay)-strlen(needle))+strlen(diamond);
    char *new_hay = (char*)malloc(total_len+1);
    memset(new_hay+total_len,0,1);
    if(index)
      memcpy(new_hay,hay,index);
    if(strlen(diamond))
      memcpy(new_hay+index,diamond,strlen(diamond));
    unsigned long remainder_len = total_len - index - strlen(diamond);
    if(remainder_len)
      memcpy(new_hay+index+strlen(diamond),hay+index+strlen(needle),remainder_len);
    return(new_hay);
  }
  return(str_Copy(hay));
}

char *str_ReplaceAll(char *hay,char *needle,char*diamond)
{
  if(!hay)
    return(str_CreateEmpty());
  if(!strlen(hay) || !needle || !strlen(needle))
    return(str_Copy(hay));
  long offset = 0;
  char *ret = str_CreateEmpty();
  long hay_len = strlen(hay);
  long needle_len = strlen(needle);
  while(offset<hay_len)
  {
    if(strstr(hay+offset,needle)==(hay+offset))
    {
      ret = str_CatFree(ret,diamond);
      offset+=needle_len;
    }
    else
    {
      ret = str_AddChar(ret,hay[offset]);
      offset++;
    }

  }
  return(ret);
}

long str_IndexOf(char *hay,char *needle)
{
  long index = -1;
  char *pos = strstr(hay,needle);
  if(pos)
  {
    index = (long)(pos-hay);
  } 
  return(index);
}

long str_LastIndexOf(char *hay,char *needle)
{
  long index = -1;
  char *pos = NULL;
  while((pos=strstr(hay+index+1,needle))!=NULL)
  {
    index = (long)(pos-hay);
  } 
  return(index);
}

char *str_CreateEmpty(void)
{
    char *string = (char*)malloc(1);
    string[0] = 0;
    return(string);
}

