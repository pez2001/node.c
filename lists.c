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

#include "lists.h"


item_list *list_Create(long num, unsigned char flags)
{
	item_list *l = (item_list*)malloc(sizeof(item_list));
	if(num)
	{
		l->items = malloc(num * sizeof(void*));
		memset(l->items,0,num*sizeof(void*));
	}
	else
		l->items = NULL;
	l->num = num;
        l->iteration_index = 0;
	l->flags = flags;
	return(l);
}

void list_Close(item_list *list)
{
	if(list->num > 0)
		free(list->items);
	free(list);
}

item_list *list_Copy(item_list *list)
{
       item_list *r = list_Create(list->num,list->flags);
       memcpy(r->items,list->items,sizeof(void*)*list->num);
       return(r);
}

long list_Push(item_list *list, void *obj)
{
	if(!list->num)
	{
		list->num = 1;
		list->items = malloc(list->num * sizeof(void*));
		list->items[0] = obj;
		return(0);
	}
	else
	{
		list->items = realloc(list->items,(list->num + 1) * sizeof(void*));
		list->items[list->num] = obj;
		list->num++;
		return(list->num-1);
	}
}

void *list_Pop(item_list *list)
{
	if(list->num)
	{
		void *tmp = list_Remove(list, list->num - 1);
		return(tmp);
	}
	return(0);
}

BOOL list_Insert(item_list *list, long index, void *obj)
{
	if(index == 0 && !list->num)
	{
		list_Push(list, obj);
	}
	else if(index <= list->num - 1)
	{
		list->num++;
		list->items = realloc(list->items, (list->num) * sizeof(void*));
		for(long i = list->num - 2; i >= index; i--)
		{
			list_MoveDown(list, i);
		}
		list->items[index] = obj;
		return(1);
	}
	else if(index > list->num - 1)
		list_Push(list, obj);
	return(0);
}

void list_MoveUp(item_list *list, long index)
{
	if(!list->num)
	{
		return;
	}
	if(index < list->num && index > 0)
	{
		list->items[index - 1] = list->items[index];
	}
}

void list_MoveDown(item_list *list, long index)
{
	if(!list->num)
	{
		return;
	}
	if(index < list->num - 1 && index >= 0)
	{
		list->items[index + 1] = list->items[index];
	}
}

void *list_Remove(item_list *list, long index)
{
	if(!list->num)
	{
		return(0);
	}
	if(index < list->num)
	{
		void *tmp = list->items[index];
		for(long i = list->num - 1; i > index; i--)
		{
			list_MoveUp(list, i);
		}
		if((list->num - 1) == 0)
		{
			free(list->items);
			list->items = 0;
			list->num = 0;
		}
		else
		{
			list->items = realloc(list->items, (list->num - 1) * sizeof(void*));
			list->num--;
		}
		return(tmp);
	}
	return(0);
}

void list_Clear(item_list *list)
{
	if(list->num)
	{
		free(list->items);
		list->items = NULL;
	}
	list->num = 0;
}

unsigned long list_GetLen(item_list *list)
{
	unsigned long r = list->num;
	return(r);
}

void *list_Get(item_list *list,long index)
{
	void *r = NULL;
	if(index < list->num)
	{
		r = list->items[index];
	}
	return(r);
}

void list_Set(item_list *list,long index, void *obj)
{
	if(index < list->num)
	{
		list->items[index] = obj;
	}
}

void list_Queue(item_list *list, void *obj)
{
	list_Insert(list, 0, obj);
}

void *list_Dequeue(item_list *list)
{
	return(list_Remove(list, 0));
}

BOOL list_Contains(item_list *list,void *obj)
{
	for(int i=0;i<list->num;i++)
	{	
		if(list->items[i] == obj)
		{
			return(1);
		}
	}
	return(0);
}

unsigned long list_GetIndex(item_list *list, void *obj)
{
	for(int i=0;i<list->num;i++)
	{	
		if(list->items[i] == obj)
		{
			return(i);
		}
	}
	return(-1);
}

long list_RemoveItem(item_list *list, void *obj)
{
	unsigned long index = list_GetIndex(list,obj);
	if(index != -1)
		list_Remove(list,index);
	return(index);
}

BOOL list_IsEmpty(item_list *list)
{
	BOOL r = !list->num;
	return(r);
}

//TODO iteration and removal/insertion handling

void *list_Iterate(item_list *list)
{
  void *r = list->items[list->iteration_index];
  list->iteration_index++;
  return(r);
}

int list_IterationUnfinished(item_list *list)
{
  return(list->iteration_index < list->num);
}

void list_IterationReset(item_list *list)
{
  list->iteration_index = 0;
}

long list_GetIterationIndex(item_list *list)
{
  return(list->iteration_index);
}

void list_SetIterationIndex(item_list *list,long iteration_index)
{
  list->iteration_index = iteration_index;
}

