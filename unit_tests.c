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

#include "unit_tests.h"


#ifdef USE_DEBUGGING
int debug_level = 0;
#endif

void ptr_tests(void)
{
	printf("testing ptr list functions\n");
	list *p = list_Create(0, 0);
	
	list_Insert(p,0,(void*)2);
	list_Insert(p,0,(void*)1);
	for(unsigned int i = 0; i < list_GetLen(p); i++)
		printf("%d ", (int)list_Get(p,i));
	list_Pop(p);
	list_Pop(p);
	printf("\n");
		
	list_Push(p, (void*)1);
	list_Push(p, (void*)2);
	list_Push(p, (void*)3);
	list_Push(p, (void*)4);
	printf("push(1,2,3,4):\n");
	for(unsigned int i = 0; i < list_GetLen(p); i++)
		printf("%d ", (int)list_Get(p,i));
	printf("\n");
	int x =(int)list_Pop(p);

	printf("pop(): %d\n", x);
	list_Push(p,  (void*)5);
	printf("push(5):\n");
	for(unsigned int i = 0; i < list_GetLen(p); i++)
		printf("%d ", list_Get(p,i));
	printf("\n");
	int r = (int)list_Remove(p, 2);

	printf("remove(2): %d\n", r);
	for(unsigned int i = 0; i < list_GetLen(p); i++)
		printf("%d ",(int)list_Get(p,i));
	printf("\ninserting\n");
	list_Insert(p, 3, (void*)66);
	printf("insert 3,66:\n");
	for(unsigned int i = 0; i < list_GetLen(p); i++)
		printf("%d ",(int)list_Get(p,i));
	printf("\ninserting\n");
	list_Insert(p, 0, (void*)99);
	printf("insert 0,99:\n");
	for(unsigned int i = 0; i < list_GetLen(p); i++)
		printf("%d ",(int)list_Get(p,i));
	printf("\n");
	list_Set(p, 1,(void*)22);
	printf("set 1,22:\n");
	for(unsigned int i = 0; i < list_GetLen(p); i++)
		printf("%d ",(int)list_Get(p,i));
	printf("\n");
	list_Queue(p, (void*)88);
	printf("queue(88):\n");
	for(unsigned int i = 0; i < list_GetLen(p); i++)
		printf("%d ",(int)list_Get(p,i));
	printf("\n");

	list_Clear(p);
	list_Push(p, 0);
	for(unsigned int i = 0; i < 20000; i++)
		list_Push(p, (void*)1);
	for(unsigned int i = 0; i < 20000; i++)
		list_Pop(p);
	printf("first:%d \n", (int)list_Pop(p));
	printf("top:%d\n", list_GetLen(p));

	for(unsigned int i = 0; i < 20000; i++)
	{
		list_Push(p, (void*)0);
		list_Pop(p);
	}

	list_Close(p);
	printf("ptr tests thru\n");
}

void ptr_iteration_tests(void)
{


}




void node_tests(void)
{
  printf("node tests\n");
  node *n = node_Create();
  node_Free(n,True);
  
  n = node_Create();
  node_SetKey(n,"root");
  node_SetSint8(n,127);
  printf("node key:%s\n",node_GetKey(n));
  printf("node value: %d\n",node_GetSint8(n));
  node_SetUint8(n,227);
  printf("node value: %d\n",node_GetUint8(n));
  
  node *n2 = node_Create();
  node_SetKey(n2,"version");
  node_SetString(n2,"basic");
  node_AddItem(n,n2);

  node *n3 = node_Create();
  node_SetKey(n3,"format");
  node_SetString(n3,"memory");
  node_AddItem(n,n3);



  node *query = node_GetItemByKey(n,"version");
  if(query != NULL && node_IsType(query,NODE_TYPE_STRING))
    printf("query:%s = %s\n",node_GetKey(query),node_GetString(query));
  if(query!=NULL)
  	printf("query key:%s,type:%d\n",node_GetKey(query),node_GetType(query));

  query = node_GetItemByKey(n,"format");
  if(query != NULL && node_IsType(query,NODE_TYPE_STRING))
    printf("query2:%s = %s\n",node_GetKey(query),node_GetString(query));


  node_Free(n,True);
  node_Free(n2,True);
  node_Free(n3,True);


  printf("node tests thru\n");
}


void json_speed_tests(void)
{
  printf("json speed tests\n");
  #ifdef USE_DEBUGGING
  mem_Init();
  for(int i=0;i<10;i++)
  {	
  	printf("test round:%d\n",i);
  #else
  for(int i=0;i<10000;i++)
  {	
  #endif
  node *root = json_LoadFile("test_wiki.json");
  node_FreeTree(root);
  root = json_LoadFile("test7_export.json");
  node_FreeTree(root);
  root = json_LoadFile("test7_export2nicer.json");
  node_FreeTree(root);
  root = json_LoadFile("test_a.json");
  node_FreeTree(root);
  root = json_LoadFile("test2.json");
  node_FreeTree(root);
  root = json_LoadFile("test3.json");
  node_FreeTree(root);
  root = json_LoadFile("test4.json");
  node_FreeTree(root);
  root = json_LoadFile("test5.json");
  node_FreeTree(root);
  root = json_LoadFile("test6.json");
  node_FreeTree(root);
  root = json_LoadFile("test_broken2.json");
  node_FreeTree(root);
  }
  #ifdef USE_DEBUGGING
  mem_Close();
  #endif
  printf("json speed tests thru\n");

}

void json_tests(void)
{
  printf("json tests\n");
  #ifdef USE_DEBUGGING
  mem_Init();
  #endif
  //node *root = json_LoadFile("test_wiki.json");
  //node *root = json_LoadFile("test7_export.json");
  node *root = json_LoadFile("tests/test7_export2nicer.json");
  //node *root = json_LoadFile("test_a.json");
  //node *root = json_LoadFile("test2.json");
  //node *root = json_LoadFile("test3.json");
  //node *root = json_LoadFile("test4.json");
  //node *root = json_LoadFile("test5.json");
  //node *root = json_LoadFile("test6.json");
  node *query = NULL;
  if(root!=NULL)
     query = node_GetItemByKey(root,"address");
  //if(query != NULL && node_IsType(query,NODE_TYPE_STRING))
  //  printf("query:%s = %s\n",node_GetKey(query),node_GetString(query));
  if(query!= NULL)
  	printf("found address object\n");
  query = node_GetItemByKey(root,"connection");
  if(query!= NULL)
    printf("query:%d\n",query->type);
  query = node_GetItemByKey(root,"customer");
  if(query!= NULL)
    printf("query customer:%d,%d\n",query->type,*(unsigned char*)query->value);
  query = node_GetItemByKey(root,"callback");
  if(query!= NULL)
    printf("query callback:%d,%d\n",query->type,*(unsigned char*)query->value);
  query = node_GetItemByKey(root,"maker");
  if(query!= NULL)
    printf("query maker:%d,%d\n",query->type,*(unsigned char*)query->value);
  query = node_GetItemByKey(root,"phone");
  //if(query != NULL && node_IsType(query,NODE_TYPE_STRING))
  //  printf("query:%s = %s\n",node_GetKey(query),node_GetString(query));
  if(query!= NULL)
  {
  	printf("found phone object\n");
    query = node_GetItemByKey(query,"home");
    if(query != NULL && node_IsType(query,NODE_TYPE_STRING))
      printf("found:%s = %s\n",node_GetKey(query),node_GetString(query));
    
  }
  printf("node tree:\n");
  node_PrintTree(root);
  node_FreeTree(root);
  
  printf("malformed json tests\n");
  //root = json_LoadFile("test_broken1.json");
  root = json_LoadFile("tests/test_broken2.json");
  node_PrintTree(root);
  node_FreeTree(root);

  printf("malformed #2 (total garbage)\n");
  //root = json_LoadFile("test_broken1.json");
  root = json_LoadFile("tests/test_total_broken.json");
  if(root!=NULL)
  {
    node_PrintTree(root);
    node_FreeTree(root);
  }
  printf("malformed #3 (binary file garbage)\n");
  //root = json_LoadFile("test_broken1.json");
  root = json_LoadFile("unit_tests_debug.exe");
  if(root!=NULL)
  {
  	node_PrintTree(root);
    node_FreeTree(root);
  }

  
  printf("malformed #4 (binary .o file garbage)\n");
  //root = json_LoadFile("test_broken1.json");
  root = json_LoadFile("unit_tests.do");
  if(root!=NULL)
  {
  	printf("found some structure\n");
  	node_PrintTree(root);
    node_FreeTree(root);
  }
}  

void fbx_tests(void)
{
  printf("fbx tests\n");
  #ifdef USE_DEBUGGING
  mem_Init();
  #endif
  
  //node *root = fbx_LoadFile("tests/box.fbx");
  //node *root = fbx_LoadFile("tests/ranger.fbx");
  node *root = fbx_LoadFile("tests/fbx_object.fbx");
  printf("node tree:\n");
  if(root!=NULL)
  {
    node_PrintTree(root);
    node_FreeTree(root);
  }
  #ifdef USE_DEBUGGING
  mem_Close();
  #endif
  printf("fbx tests thru\n");
}

void mem_tests(void)
{
  #ifdef USE_DEBUGGING
  printf("memory test\n");
  mem_Init();
  char *test = malloc(66);
  free(test);
  node *n = node_Create();
  node_SetKey(n,"root");
  node_SetString(n,"Hello World!");
  node_SetString(n,"Hello World2!");
  node_SetArray(n,0);
  node *sub = node_Create();
  node_SetKey(sub,"sub_node");
  node_SetString(sub,"sub");
  node *sub2 = node_Create();
  node_SetString(sub2,"sub");
  node_SetKey(sub2,"sub2_node");
  node_array_Add(n,sub);
  node_AddItem(n,sub2);
  node_FreeTree(n);
  mem_Close();
  printf("memory test thru\n");
  #endif
}


int main(int argc, char *argv[])
{
	//ptr_tests();
    //node_tests();
    //mem_tests();
    //json_tests();
    fbx_tests();
    //json_speed_tests();
}
