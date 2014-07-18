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
#ifndef MEMORY_H
#define MEMORY_H


#ifdef USE_MEMORY_DEBUGGING

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define malloc mem_malloc
#define realloc mem_realloc
#define free mem_free
 
typedef struct
{
	void *ptr;
	char *description;
	unsigned long size;
	int is_freed;
} mem_chunk;

#ifdef __cplusplus
extern "C"  {
#endif

void mem_Init(void);

int mem_Close(void);

void mem_Dump(void *mem);

void mem_SetVerbosity(int verbose);

int mem_DebugHeapWalk(int show_leaked);

void *mem_malloc(unsigned long size);

void *mem_malloc_debug(unsigned long size, char *description);

void *mem_realloc(void *ptr, unsigned long size);

//void *mem_copy(void *src);

void mem_free(void *ptr);


#ifdef __cplusplus
} 
#endif
#endif

#endif
