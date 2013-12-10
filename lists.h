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


#ifndef LISTS_H
#define LISTS_H

#include "stdio.h"
#include "stdarg.h"
#include "stdlib.h"
#include "string.h"


#ifdef __cplusplus
extern "C"  {
#endif

#define BOOL int
#define TRUE 1
#define FALSE 0
#define True 1
#define False 0


typedef struct
{
	void **items; 
	long num;
        long iteration_index;
	unsigned char flags;
} item_list;


#define LIST_STATIC 1		// TODO add support for static lists , so indices wont change

item_list *list_Create(long num, unsigned char flags); /* returns item_list*/

item_list *list_Copy(item_list *list);

void list_Close(item_list *list);

void list_Push(item_list *list, void *obj);

void *list_Pop(item_list *list);

BOOL list_Insert(item_list *list, long index, void *obj);

void *list_Remove(item_list *list, long index);

void list_RemoveItem(item_list *list, void *obj);

unsigned long list_GetIndex(item_list *list, void *obj);

void list_Clear(item_list *list);

unsigned long list_GetLen(item_list *list);

void *list_Get(item_list *list, long index);

void list_Set(item_list *list, long index, void *obj);

void list_Queue(item_list *list, void *obj);

void *list_Dequeue(item_list *list);

BOOL list_IsEmpty(item_list *list);

BOOL list_Contains(item_list *list,void *obj);

void list_MoveUp(item_list *list, long index);

void list_MoveDown(item_list *list, long index);

void *list_Iterate(item_list *list);

void list_IterationReset(item_list *list);

int list_IterationUnfinished(item_list *list);


#ifdef __cplusplus
} 
#endif


#endif
