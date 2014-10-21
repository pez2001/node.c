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




node *setup_tests_suite(char *tests_json)
{
  node *suite = node_Create();
  node *tests_json_tree = json_LoadFile(tests_json);
  node *tests = node_GetItemByKey(tests_json_tree,"tests");
  node_RemoveItem(tests_json_tree,tests);
  node_AddItem(suite,tests);
  node_SetParent(tests,suite);
  node_FreeTree(tests_json_tree);
  return(suite);
}

long get_tests_num(node *suite)
{
  node *tests = node_GetItemByKey(suite,"tests");
  return(node_array_GetNum(tests));
}

node *get_test(node *suite,long index)
{
  node *tests = node_GetItemByKey(suite,"tests");
  return(node_array_Get(tests,index));
}

void close_tests_suite(node *suite)
{
  node_FreeTree(suite);
}

char *run_script(char *filename,char *interpreter_filename,int is_exitcode ,void *check_value)
{
  node *state = init_nyx();
  node *nyx_stream =  nyx_LoadFile(filename);
  nyx_set_output_mode(state,NYX_OUTPUT_STRING);
  long ret = -1;
  set_obj_string(state,"script_filename",filename);
  set_obj_string(state,"interpreter_filename",interpreter_filename);
  if(nyx_stream!=NULL)
  {
    node *nyx_block = node_GetItemByKey(nyx_stream,"nyx_block");
    if(nyx_block!=NULL)
    {
      add_obj_kv(state,nyx_block);
      node_RemoveItem(nyx_stream,nyx_block);
      node_FreeTree(nyx_stream);
      node *ret_obj = evaluate_block(state,nyx_block);  
      node *real_value = node_GetItemByKey(ret_obj,"value");
      if(node_GetType(real_value)==NODE_TYPE_SINT32)
        ret = node_GetSint32(real_value);
      else if(node_GetType(real_value)==NODE_TYPE_STRING)
        ret = atoi(node_GetString(real_value));
      //add_garbage(state,ret);
      free_garbage(state,0,NULL);
      node *block_flag=node_GetItemByKey(state,"block_flag");
      if(block_flag!=NULL) 
      {
        char *bf = node_GetString(block_flag);
        if(!strcmp(bf,"exit"))
        {
          node *nexit_code=node_GetItemByKey(state,"exit_code");
          ret = (int)node_GetSint32(nexit_code);
        }
      }
    }
    else
      node_FreeTree(nyx_stream);
  }
  if(is_exitcode)
  {
    printf("\t %d == %d ",ret,(int)check_value);
    ret=!(ret == (int)check_value);
    if(ret)
      ret = str_Copy(nyx_get_output(state));
  }
  else
  {
    ret = strcmp(nyx_get_output(state),(char*)check_value);
    if(strlen(nyx_get_output(state))>3)
    {
      char *a = str_Sub(nyx_get_output(state),0,3);
      //char *b = str_Sub((char*)check_value,0,3);
      //printf("\t\"%s..\" == \"%s..\" ",a,b);
      char *a_r = str_ReplaceAll(a,"\n","");
      printf("\t \"%s..\" " ,a_r);
      free(a);
      free(a_r);
      //free(b);
    }
    else
    {
      char *a_r = str_ReplaceAll(nyx_get_output(state),"\n","");
      char *b_r = str_ReplaceAll((char*)check_value,"\n","");
      printf("\t\"%s\" == \"%s\" ",a_r,b_r);
      free(a_r);
      free(b_r);
    }
    if(ret)
      ret = str_Copy(nyx_get_output(state));
  }
  close_nyx(state);
  return(ret);
}

char *get_test_expected_output(node *suite,int index)
{
  node *test = get_test(suite,index);
  node *expected_output = node_GetItemByKey(test,"expected_output");
  node *expected_exitcode = node_GetItemByKey(test,"expected_exitcode");
  if(expected_output)
    return(node_GetString(expected_output));
  return(NULL);
}

char *run_test(node *test,char *interpreter_filename)
{
  node *name = node_GetItemByKey(test,"name");
  printf("\t%-17s",node_GetString(name));
  node *file = node_GetItemByKey(test,"file");
  node *expected_output = node_GetItemByKey(test,"expected_output");
  node *expected_exitcode = node_GetItemByKey(test,"expected_exitcode");
  if(expected_exitcode)
    return(run_script(node_GetString(file),interpreter_filename,1,node_GetSint32(expected_exitcode)));  
  if(expected_output)
    return(run_script(node_GetString(file),interpreter_filename,0,node_GetString(expected_output)));  
}

int main(int argc, char *argv[])
{
  /*nyx_raw_tests();
  nyx_raw_tests2();*/
  node *suite = setup_tests_suite("tests/tests.json");
  long num_tests = get_tests_num(suite);
  printf("\t\t:: Running %d tests :: \n",num_tests);
  printf(" Test#\tTest Name\t\t\t\tTest Status\n");
  printf("------------------------------------------------------------\n");
  long tests_failed = 0 ;
  for(long i=0;i<num_tests;i++)
  {
    printf("#%-3d",i+1);
    void *r = run_test(get_test(suite,i),argv[0]);
    if(!r)
      printf("\t  Success");
    else
    {
      printf("\t  Failed");
      char *t_ex = get_test_expected_output(suite,i);
      if(t_ex)
      {
        char *t_exx = str_Cat(">>",t_ex);
        char *t_exx_r = str_ReplaceAll(t_exx,"\n","\n>>");
        char *t_r = str_Cat(">>",r);
        char *t_r_r = str_ReplaceAll(t_r,"\n","\n>>");

        printf("\n\t  Test #%d Output:\n\t ------------------\n%s\n",i+1,t_r_r);
        printf("\n\t  Expected Output:\n\t ------------------\n%s\n",t_exx_r);
        free(t_exx);
        free(t_exx_r);
        free(t_r);
        free(t_r_r);
      }
      else
      {
        if(strlen(r))
        {
          char *t_r = str_Cat(">>",r);
          char *t_r_r = str_ReplaceAll(t_r,"\n","\n>>");
          printf("\n\t  Test #%d Output:\n\t ------------------\n%s\n",i+1,t_r_r);
          free(t_r);
          free(t_r_r);
        }
      }
      free(r);
      tests_failed++;
    }

    printf("\n");
  }
  printf("------------------------------------------------------------\n");
  if(tests_failed)
    printf("\t\t%d Test(s) failed\n",tests_failed);
  else
    printf("\t\tAll Tests succeeded\n");
  close_tests_suite(suite);
}

#pragma GCC diagnostic pop
