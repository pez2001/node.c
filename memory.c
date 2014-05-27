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
mem_chunk **mem_chunk_items;


void mem_Init(void)
{
	mem_chunk_items = (mem_chunk**) malloc_ptr(1 * sizeof(mem_chunk*));
	mem_chunks_num = 0;
	mem_max_chunk_size = 0;
}

long mem_get_chunk(void *ptr)
{
  long index = -1;
  for(long i=0;i<mem_chunks_num;i++)
  	if(mem_chunk_items[i]->ptr==ptr)
  	{
  		index = i;
  		break;
    }
  return(index);
}

long mem_AddChunk(void *ptr,char *description,unsigned long size)
{
	mem_chunk *chunk = (mem_chunk*)malloc_ptr(sizeof(mem_chunk));

	chunk->ptr = ptr;
	chunk->description = description;
	chunk->size = size;
	chunk->is_freed = 0;
	mem_chunk_items = (mem_chunk**) realloc_ptr(mem_chunk_items,(mem_chunks_num+1)*sizeof(mem_chunk*));
	mem_chunk_items[mem_chunks_num] = chunk;
	mem_chunks_num++;
	return(mem_chunks_num-1);
}

void mem_Dump(void *mem)
{
	long index = mem_get_chunk(mem);
	printf("chunk address:%x\ndescription:%s\nsize:%d\nhex:\n",mem,mem_chunk_items[index]->description,mem_chunk_items[index]->size);
    for(unsigned long b = 0;b<mem_chunk_items[index]->size;b++)
	{
	  printf("%2x ",((unsigned char*)mem_chunk_items[index]->ptr)[b]);
	}
	printf("\nraw:\n");
	for(unsigned long b = 0;b<mem_chunk_items[index]->size;b++)
	{
	  printf("%c",((unsigned char*)mem_chunk_items[index]->ptr)[b]);
	}
	printf("\n");
}

int mem_DebugHeapWalk(int show_leaked)
{
	long leaked = 0;
	for(long i = 0;i<mem_chunks_num;i++)
	{
		if(show_leaked && !mem_chunk_items[i]->is_freed )
		{
			printf("leaked ");
			mem_Dump(mem_chunk_items[i]->ptr);
			leaked++;
		}
	}
	if(show_leaked)
		printf("leaked chunks:%d\n",leaked);
	if(leaked)
		return(0);
	else 
		return(1);
}

int mem_Close(void)
{
	printf("mem chunks allocated:%d\n",mem_chunks_num);
	printf("mem max chunk size:%d\n",mem_max_chunk_size);
	//printf("heap peaked @ %d\n",mem_max_allocated_size);
	int r = mem_DebugHeapWalk(1);
	for(long i = 0;i<mem_chunks_num;i++)
	{
		if(!mem_chunk_items[i]->is_freed)
		{
			free_ptr(mem_chunk_items[i]->ptr);
		}
		free_ptr(mem_chunk_items[i]);
	}
	free_ptr(mem_chunk_items);
	return(r);
}

void *mem_malloc_debug(unsigned long size, char *description)
{
	void *ptr = malloc_ptr(size);
	if(size > mem_max_chunk_size)
	{
		mem_max_chunk_size = size;
	}		
	mem_AddChunk(ptr,description,size);
	return(ptr);
}

void *mem_malloc(unsigned long size)
{
	void *ptr = malloc_ptr(size);
	if(size > mem_max_chunk_size)
	{
		mem_max_chunk_size = size;
	}		
	mem_AddChunk(ptr,"",size);
	return(ptr);
}


void *mem_realloc(void *ptr, unsigned long size)
{
	if(size > mem_max_chunk_size)
	{
		mem_max_chunk_size = size;
	}		
	void *tmp = (void*)malloc_ptr(size);
	long index = mem_get_chunk(ptr);
	size_t msize = mem_chunk_items[index]->size;
	if(msize > size) //make memory move fitting the new chunk size
		msize = size;
		
	memcpy(tmp,ptr,msize);
	free_ptr(ptr);
	mem_chunk_items[index]->ptr = tmp;
	mem_chunk_items[index]->size = size;	
	return(tmp);
}

void mem_free(void *ptr)
{	
	long index = mem_get_chunk(ptr);
	if(!mem_chunk_items[index]->is_freed)
	{
		unsigned long i =0;
		/*for(i=0;i<mem_chunk_items[index]->size;i++)
		{

		}*/
		memset(ptr,0,mem_chunk_items[index]->size);
		free_ptr(ptr);
		mem_chunk_items[index]->is_freed = 1;
		mem_chunk_items[index]->ptr = NULL;
	}
	else
	{
		printf("double freeing:%x\n",ptr);
		//int x = 1/0;//my breakpoint style :-)
	}

}

void *mem_copy(void *src)
{
	long index = mem_get_chunk(src);
	void *tmp = (void*)malloc_ptr(mem_chunk_items[index]->size);
	memcpy(tmp,src,mem_chunk_items[index]->size);
	mem_AddChunk(tmp,"memory copy",mem_chunk_items[index]->size);
	return(tmp);
}


#endif

