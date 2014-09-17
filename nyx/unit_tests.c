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

void nyx_raw_tests(void)
{
  printf("nyx raw tests\n");
  #ifdef USE_MEMORY_DEBUGGING
  mem_Init();
  #endif
  
  printf("loading test_block2.yeti\n");
  node *root = nyx_LoadFile("tests/test_block2.yeti");
  if(root!=NULL)
  {
    printf("test_block2.yeti node tree:\n");
    node_PrintTree(root);
    node_FreeTree(root);
  }


  printf("loading test.yeti\n");
  root = nyx_LoadFile("tests/test.yeti");
  if(root!=NULL)
  {
    printf("test.yeti node tree:\n");
    node_PrintTree(root);
    node_FreeTree(root);
  }
  
  printf("loading test_comment.yeti\n");
  root = nyx_LoadFile("tests/test_comment.yeti");
  if(root!=NULL)
  {
    printf("test_comment.yeti node tree:\n");
    node_PrintTree(root);
    node_FreeTree(root);
  }

  printf("loading test_print.yeti\n");
  root = nyx_LoadFile("tests/test_print.yeti");
  if(root!=NULL)
  {
    printf("test_print.yeti node tree:\n");
    node_PrintTree(root);
    node_FreeTree(root);
  }

  printf("loading test_block.yeti\n");
  root = nyx_LoadFile("tests/test_block.yeti");
  if(root!=NULL)
  {
    printf("test_block.yeti node tree:\n");
    node_PrintTree(root);
    node_FreeTree(root);
  }

  printf("loading test_empty.yeti\n");
  root = nyx_LoadFile("tests/test_empty.yeti");
  if(root!=NULL)
  {
    printf("test_empty.yeti node tree:\n");
    node_PrintTree(root);
    node_FreeTree(root);
  }

  printf("loading test_empty2.yeti\n");
  root = nyx_LoadFile("tests/test_empty2.yeti");
  if(root!=NULL)
  {
    printf("test_empty2.yeti node tree:\n");
    node_PrintTree(root);
    node_FreeTree(root);
  }

  printf("loading test_empty3.yeti\n");
  root = nyx_LoadFile("tests/test_empty3.yeti");
  if(root!=NULL)
  {
    printf("test_empty3.yeti node tree:\n");
    node_PrintTree(root);
    node_FreeTree(root);
  }

  printf("loading test_empty4.yeti\n");
  root = nyx_LoadFile("tests/test_empty4.yeti");
  if(root!=NULL)
  {
    printf("test_empty4.yeti node tree:\n");
    node_PrintTree(root);
    node_FreeTree(root);
  }

  printf("loading test_broken.yeti\n");
  root = nyx_LoadFile("tests/test_broken.yeti");
  if(root!=NULL)
  {
    printf("test_broken.yeti node tree:\n");
    node_PrintTree(root);
    node_FreeTree(root);
  }

  printf("loading test_block2.yeti\n");
  root = nyx_LoadFile("tests/test_block2.yeti");
  if(root!=NULL)
  {
    printf("test_block2.yeti node tree:\n");
    node_PrintTree(root);
    node_FreeTree(root);
  }

  printf("loading test_nesting.yeti\n");
  root = nyx_LoadFile("tests/test_nesting.yeti");
  if(root!=NULL)
  {
    printf("test_nesting.yeti node tree:\n");
    node_PrintTree(root);
    node_FreeTree(root);
  }

  /*root = nyx_LoadFile("tests/test_comment_strings.yeti");
  printf("node tree:\n");
  if(root!=NULL)
  {
    node_PrintTree(root);
    node_FreeTree(root);
  }*/


  #ifdef USE_MEMORY_DEBUGGING
  mem_Close();
  #endif
  printf("nyx tests thru\n");
}

void nyx_raw_tests2(void)
{
  printf("nyx raw tests 2\n");
  #ifdef USE_MEMORY_DEBUGGING
  mem_Init();
  #endif
  

  printf("loading test_nesting.yeti\n");
  node *root = nyx_LoadFile("tests/test_nesting.yeti");
  if(root!=NULL)
  {
    printf("test_nesting.yeti node tree:\n");
    node_PrintTree(root);
    node_FreeTree(root);
  }

  printf("loading test_last_statement.yeti\n");
  root = nyx_LoadFile("tests/test_last2.yeti");
  if(root!=NULL)
  {
    printf("test_nesting.yeti node tree:\n");
    node_PrintTree(root);
    node_FreeTree(root);
  }


  #ifdef USE_MEMORY_DEBUGGING
  mem_Close();
  #endif
  printf("nyx tests 2 thru\n");
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

int main(int argc, char *argv[])
{
  nyx_raw_tests();
  nyx_raw_tests2();
}

#pragma GCC diagnostic pop
