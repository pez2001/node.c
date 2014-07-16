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

#include "nyxi.h"

const char *nyxi_helpmsg = "Usage: nyxi [OPTION]... [INPUT FILE]\n\
Execute nyx script Files.\n\
\n\
  -i    interactive mode\n\
  -a    print script ast\n\
  -s    interpret INPUT FILE as script string\n\
  -h    display this help and exit\n\
  -v    output version information and exit\n\
  -p    print the return value\n\
\n\
  Use - as input file or leave empty to use STDIN\n\
  CTRL-D to exit\n\
\n\
  Return value of script will be used as the exit code.\n\
\n\
      Report bugs to <pez2001@voyagerproject.de>.\n";



/*TODO
add binary typing of strings (intepreted as byte stream)(to read c structs etc)
dump/load state via control object
catch ctrl-D
x interpret each received line via stdin seperately

man page
deb packages

better handling of preops regarding ++ and like (check if last token in statement)
better string handling(single char setting)
better json export (tuples dont work now)

*/


volatile int done=0;

void sig_handler(int sig)//,siginfo_t *siginfo,void *context)
{
  //printf("signo:%d\n",sig);
  //printf("signo:%d\n",siginfo->si_signo);
  //printf("sigcode:%d\n",siginfo->si_code);
  done=1;
}

//struct termios *setup_terminal()
//#define _O_U16TEXT 0x20000
void setup_terminal(void)
{
    //struct termios old;
    //struct termios new;
    //sigaction(sig_handler,0);
    //signal(SIGABRT,sig_handler);
    //signal(SIGTERM,sig_handler);
    signal(SIGINT,sig_handler);
    //_setmode(_fileno(stdout), _O_U16TEXT);
    //signal(1,sig_handler);
    //signal(3,sig_handler);
    //signal(20,sig_handler);
    //signal(sig_handler,0);
    //setvbuf(stdout,NULL,_IONBUF,0);
    //tcgetattr(0,&old);
    //new = old;
    //new.c_cc[VEOF] = 3;
    //new.c_cc[VINTR] = 4;
    //tcsetattr(0,TCSANOW,&new);
    //return(old);
}

//void close_terminal(struct termios *old)
void close_terminal(void)
{
  //tcsettr(0,TCSANOW,old);
}


int main(int argc, char** argv)
{
  #ifdef USE_MEMORY_DEBUGGING
  mem_Init();
  #endif
  #ifdef WIN32
  WSADATA wsaData;
  WSAStartup(MAKEWORD(2, 2), &wsaData);
  #endif


  int c = 0;
  int print_return_value=0;
  //int interactive_mode=0;
  int print_ast=0;
  int use_input_as_script_string=0;

  int ret = 0;
  while ((c = getopt (argc, argv, "iashvp")) != -1)
  {
    switch (c)
    {
      case 's':
        use_input_as_script_string = 1;
        break;
      case 'p':
        print_return_value = 1;
        break;
      /*case 'i':
        interactive_mode = 1;
        break;*/
      case 'a':
        print_ast = 1;
        break;
      case 'h':
        printf(nyxi_helpmsg);
        return(0);
      case 'v':
        printf("nyx interpreter %d.%d (build %d)\n",MAJOR_VERSION,MINOR_VERSION,BUILD+1);
        return(0);
      case '?':
        if (optopt == 'c')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint(optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
        return(1);
      default:
        abort();
    }
  }
  char *filename = argv[optind];
  node *nyx_stream = NULL;
  if(filename != NULL && strlen(filename)>0 && !use_input_as_script_string && strcmp(filename,"-")) //use filename to load a script file
  {
    nyx_stream = nyx_LoadFile(filename);
  }
  else if(filename != NULL && strcmp(filename,"-") && use_input_as_script_string)//) //string input //!(strlen(filename)==1 &&
  {
    nyx_stream = nyx_LoadString(filename);
  }
  /*else if(interactive_mode) //interactive 
  { 
    printf("interactive\n");
  }*/
  else //read from stdin
  {
    setup_terminal();
    node *state = init_nyx();
    node *blocks=node_GetItemByKey(state,"blocks");
    char *buf =(char*)malloc(100);
    char *tmp_get=NULL;
    node *ret_obj=NULL;
    set_obj_string(state,"script_filename","STDIN");
    set_obj_string(state,"interpreter_filename",argv[0]);
    while((tmp_get=fgets(buf,100,stdin))!=NULL && !done && ((long)(tmp_get)!=EOF))
    {
      nyx_stream = nyx_LoadString(buf);
      if(nyx_stream!=NULL)
      {
        node *nyx_block = node_GetItemByKey(nyx_stream,"nyx_block");
        if(nyx_block!=NULL)
        {
          node *master_block = node_GetItem(blocks,0);
          node_RemoveItem(nyx_stream,nyx_block);
          node_FreeTree(nyx_stream);
          if(master_block!=NULL)
            ret_obj = evaluate_block_in(state,nyx_block,master_block);  
          else
            ret_obj = evaluate_block(state,nyx_block);  
          node *real_value = node_GetItemByKey(ret_obj,"value");
          if(print_return_value)
          {
            node_Print(real_value,False,False);
            printf("\n");
          }
          if(node_GetType(real_value)==NODE_TYPE_SINT32)
            ret = node_GetSint32(real_value);
          else if(node_GetType(real_value)==NODE_TYPE_STRING)
            ret = atoi(node_GetString(real_value));
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
    }
    close_terminal();
    close_nyx(state);
    return(0);
  }
  if(print_ast)
  {
    if(nyx_stream!=NULL)
    {
      node_PrintTree(nyx_stream);
      node_FreeTree(nyx_stream);
    }
    return(0);
  }
  else // interpret stream
  { 
    node *state = init_nyx();
    set_obj_string(state,"script_filename",filename);
    set_obj_string(state,"interpreter_filename",argv[0]);
    if(nyx_stream!=NULL)
    {
      node *nyx_block = node_GetItemByKey(nyx_stream,"nyx_block");
      if(nyx_block!=NULL)
      {
        add_obj_kv(state,nyx_block);
        node_RemoveItem(nyx_stream,nyx_block);
        node_FreeTree(nyx_stream);
        node *ret_obj = evaluate_block(state,nyx_block);  
        //test call into script 
        //node *parameters = create_obj("parameters");
        //printf("calling defined script function\n");
        //node *ret_call_obj = call_function(nyx_state,"external_call",parameters);//,nyx_block);
        node *real_value = node_GetItemByKey(ret_obj,"value");
        if(print_return_value)
        {
          node_Print(real_value,False,False);
          printf("\n");
        }
        if(node_GetType(real_value)==NODE_TYPE_SINT32)
          ret = node_GetSint32(real_value);
        else if(node_GetType(real_value)==NODE_TYPE_STRING)
          ret = atoi(node_GetString(real_value));
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
    close_nyx(state);
  }
  #ifdef WIN32
  WSACleanup();
  #endif
  #ifdef USE_MEMORY_DEBUGGING
  mem_Close();
  #endif
  return(ret%256);
}