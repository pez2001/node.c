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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_MEMORY_DEBUGGING

void *(*malloc_ptr)(size_t size) = malloc;
void *(*realloc_ptr)(void *ptr,size_t size) = realloc;
void (*free_ptr)(void *ptr) = free;

#include "memory.h"


long mem_chunks_num = 0;
unsigned long mem_max_chunk_size = 0;
unsigned long mem_max_allocated_size = 0;
unsigned long mem_allocated_size = 0;
mem_chunk *mem_first_chunk = NULL;
mem_chunk *mem_last_chunk = NULL;
int mem_verbosity = 0;

void mem_Init(void)
{
	mem_chunks_num = 0;
	mem_max_chunk_size = 0;
	mem_max_allocated_size = 0;
	mem_allocated_size = 0;
	mem_verbosity=0;
	mem_first_chunk = NULL;
	mem_last_chunk = NULL;
}

void mem_SetVerbosity(int verbose)
{
	mem_verbosity=verbose;
}


mem_chunk *mem_get_chunk(void *ptr)
{
  if(mem_first_chunk)
  {
	mem_chunk *mi = mem_first_chunk;
	while(mi != NULL)
	{
		//printf("checking chunk:%x with ptr:%x against: %x\n",mi,mi->ptr,ptr);
		if(mi->ptr == ptr)
			return(mi);
		if(mi->next==NULL)
		{
			if(mi != mem_last_chunk)
				printf("error in chunk chain:%x\n",ptr);
			return(NULL);
		}
		mi = mi->next;
	}
  }	
  return(NULL);
}

mem_chunk *mem_AddChunk(void *ptr,unsigned long size)
{
	mem_chunk *chunk = (mem_chunk*)malloc_ptr(sizeof(mem_chunk));
	if(mem_verbosity>=1)
		printf("adding chunk:%x (size:%d)\n",ptr,size);
	//TODO add backtrace string
	chunk->ptr = ptr;
	chunk->size = size;
	chunk->is_freed = 0;
	chunk->prev = mem_last_chunk;
	chunk->next = NULL;
	mem_chunks_num++;
	mem_allocated_size+=size;
	if(mem_allocated_size>mem_max_allocated_size)
		mem_max_allocated_size = mem_allocated_size;

	if(mem_last_chunk)
	{
		//printf("updating prev last_chunk chunk->next\n");
		mem_last_chunk->next = chunk;
	}
	else
	{
		//printf("first chunk in chain\n");
		mem_first_chunk = chunk;
	}
	
	mem_last_chunk = chunk;

	/*if(!mem_first_chunk)
		mem_first_chunk = chunk;
	*/
	return(chunk);
}

void mem_Dump(void *mem)
{
	mem_chunk *chunk = mem_get_chunk(mem);
	if(!chunk)
	{
		printf("chunk for ptr: %x not found\n",mem);
		return;
	}
	printf("chunk address: %x\nsize: %d\nhex: ",mem,chunk->size);
    for(unsigned long b = 0;b<chunk->size;b++)
	{
	  printf("%2x ",((unsigned char*)chunk->ptr)[b]);
	}
	printf("\nraw: ");
	for(unsigned long b = 0;b<chunk->size;b++)
	{
	  printf("%c",((unsigned char*)chunk->ptr)[b]);
	}
	printf("\n");
}

int mem_DebugHeapWalk(int show_leaked)
{
	long leaked = 0;
	mem_chunk *mi = mem_first_chunk;
	if(mi)
	{
		while(mi != mem_last_chunk)
		{
			if(show_leaked && !mi->is_freed )
			{
				printf("leaked ");
				mem_Dump(mi->ptr);
				leaked++;
			}
			mi = mi->next;
		}
		if(show_leaked)
			printf("leaked chunks:%d\n",leaked);
		if(leaked)
			return(0);
		else 
			return(1);
	}
	return(1);
}

int mem_Close(void)
{
	printf("mem chunks allocated:%d\n",mem_chunks_num);
	printf("mem allocated size:%d\n",mem_allocated_size);
	printf("mem max allocated size:%d\n",mem_max_allocated_size);
	printf("mem max chunk size:%d\n",mem_max_chunk_size);
	int r = mem_DebugHeapWalk(1);
	if(mem_first_chunk)
  	{
		mem_chunk *mi = mem_first_chunk;
		while(mi)
		{
			if(!mi->is_freed)
			{
				free_ptr(mi->ptr);
			}
			mem_chunk *next = mi->next;
			free_ptr(mi);
			mi = next;
		}
	}	
	return(r);
}

void *mem_malloc(unsigned long size)
{
	void *ptr = malloc_ptr(size);
	if(size > mem_max_chunk_size)
	{
		mem_max_chunk_size = size;
	}		
	mem_AddChunk(ptr,size);
	return(ptr);
}


void *mem_realloc(void *ptr, unsigned long size)
{
	if(size > mem_max_chunk_size)
	{
		mem_max_chunk_size = size;
	}		

	//void *tmp = (void*)malloc_ptr(size);
	void *tmp = (void*)realloc_ptr(ptr,size);
	
	mem_chunk *chunk = mem_get_chunk(ptr);
	size_t msize = chunk->size;
	if(msize > size) //make memory move fitting the new chunk size
		msize = size;
		
	//memcpy(tmp,ptr,msize);
	//free_ptr(ptr);
	mem_allocated_size = (mem_allocated_size - chunk->size) + size;

	chunk->ptr = tmp;
	chunk->size = size;	

	if(mem_allocated_size>mem_max_allocated_size)
		mem_max_allocated_size = mem_allocated_size;
	return(tmp);
}

void mem_free(void *ptr)
{	
	mem_chunk *chunk = mem_get_chunk(ptr);
	if(!chunk)
	{
		printf("chunk not found\ndouble freeing:%x\n",ptr);
		return;
	}
	if(!chunk->is_freed)
	{
		if(mem_verbosity>=1)
		{
			printf("freeing ptr:%x\n",ptr);
		}
		if(mem_verbosity>=2)
		{
			mem_Dump(ptr);
		}
		free_ptr(ptr);
		mem_allocated_size-=chunk->size;

		if(mem_last_chunk == chunk)
		{
			if(chunk->prev)
			{
				//printf("removing last chunk in chain\n");
				mem_last_chunk = chunk->prev;
			}
			else
			{
				//printf("removed all chunks setting last chunk to NULL\n");
				mem_last_chunk = NULL;
			}
		}

		if(mem_first_chunk == chunk)
		{
			if(chunk->next)
			{
				//printf("removing first chunk in chain\n");
				mem_first_chunk = chunk->next;
			}
			else
			{
				//printf("removed all chunks setting first chunk to NULL\n");
				mem_first_chunk = NULL;
			}
		}

		if(chunk->next)
		{
			//printf("setting next chunk prev\n");
			chunk->next->prev = chunk->prev;
		}
		if(chunk->prev)
		{
			//printf("setting prev chunk next\n");
			chunk->prev->next = chunk->next;
		}
	}
	else
	{
		printf("double freeing:%x\n",ptr);
		return;
		//int x = 1/0;//my breakpoint style :-)
	}
}

#endif

