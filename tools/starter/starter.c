#include "starter.h"

const char *starter_helpmsg = "Usage: starter [OPTION]... [LIST OF COMMANDS+ARGUMENTS]\n\
Create Starter Executables.\n\
\n\
  -r    immediately rename a file\n\
  -d    immediately delete a file\n\
  -R    remove tag from itself\n\
  -o    output temp filename\n\
  -s    interpret INPUT FILE as script string\n\
  -h    display this help and exit\n\
  -v    output version information and exit\n\
  -p    print the return value\n\
\n\
  Use  leave empty to use STDIN\n\
\n\
  code.\n\
\n\
      Report bugs to <pez2001@voyagerproject.de>.\n";



/*
  Features

    add multiple executables for running each with its own set of arguments
    add payload extract function
    open url
    load payload with curl statically linked
    remove file function
    open console window/or disable it

    works in linux and windows environments

    static executable

    add nyx for scripting of actions
*/


void write_data(void *data,unsigned long len,unsigned long offset,FILE *out)
{
  fseek(out,offset,SEEK_SET);
  fwrite(data,len,1,out);
}

void append_data(void *data,unsigned long len,FILE *out)
{
  fwrite(data,len,1,out);
}

int copy_itself_to_file(FILE *in,unsigned long len,FILE *out)
{
  fseek(in,0,SEEK_SET);
  char *data = (char*)malloc(len);
  fread(data,len,1,in);
  append_data(data,len,out);
  free(data);
}

int add_entry(char *command,char *payload,unsigned long payload_len,char *second_payload,unsigned long second_payload_len,FILE *out)
{
  entry e;
  e.command_len = strlen(command)+1;
  e.payload_len = payload_len;
  e.second_payload_len = second_payload_len;
  append_data(command,strlen(command)+1,out);
  //printf("entry command:[%s]:%d\n",command,strlen(command)+1);
  if(payload_len)
  {
    //printf("entry payload:[%s]:%d\n",payload,payload_len);
    append_data(payload,payload_len,out);
  }
  if(second_payload_len)
  {
    //printf("entry second payload:[%s]:%d\n",second_payload,second_payload_len);
    append_data(second_payload,second_payload_len,out);
  }
  append_data(&e,sizeof(entry),out);
}

int add_tag(unsigned long num_entries,FILE *out)
{
  tag t;
  t.magic=123456;
  t.num_entries = num_entries;
  t.get_options = 1;
  append_data(&t,sizeof(tag),out);
}

int check_for_mz(FILE *in)
{
  fseek(in,0,SEEK_SET);
  unsigned short magic;
  fread(&magic,2,1,in);
  if((magic&255)=='M' && ((magic>>8)&255)=='Z')
    return(1);
  return(0);
}

int check_for_tag(FILE *in,unsigned long len)
{
  fseek(in,len-sizeof(tag),SEEK_SET);
  tag ti;
  fread(&ti,sizeof(tag),1,in);
  if(ti.magic == 123456)
    return(1);
  return(0);
}

tag *read_tag(FILE *in,unsigned long len)
{
  fseek(in,len-sizeof(tag),SEEK_SET);
  tag *ti = (tag*)malloc(sizeof(tag));
  fread(ti,sizeof(tag),1,in);
  if(ti->magic == 123456)
    return(ti);
  free(ti);
  return(NULL);
}

int update_tag(FILE *in,unsigned long len,tag *t)
{
  fseek(in,len-sizeof(tag),SEEK_SET);
  fwrite(t,sizeof(tag),1,in);
}


int execute_popen(char *command)
{
  //printf("executing popen:[%s]\n",command);
  FILE *f = (FILE*)popen(command,"rb");
  if(f!=NULL)
  {
    char *buf = (char*)malloc(100);
    memset(buf,0,100);
    char *ret= str_CreateEmpty();
    int fr=0;
    while((fr=fread(buf,1,99,f))!=0)
    {
      ret=str_CatFree(ret,buf);
      memset(buf,0,100);
    }
    fclose(f);
    //printf("popen output:\n",ret);
    free(ret);
    free(buf);
    return(1);
  }
 return(0);
}

//"open"
int execute_shell(char *executable,char *arguments)
{
  //printf("shell execute:[%s],[%s]\n",executable,arguments);
  ShellExecute(NULL,executable,arguments,NULL,NULL,SW_SHOWNORMAL); 
}

int execute_process(char *executable,char *arguments)
{
  //printf("executing process:[%s],[%s]\n",executable,arguments);
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  //memset(&si,0,sizeof(si));
  memset(&pi,0,sizeof(pi));
  //si.dwFlags = STARTF_USESHOWWINDOW;
  //si.wShowWindow = SW_SHOWNORMAL;
  si.cb = sizeof(si);
  GetStartupInfo(&si);
  //int r = CreateProcess(executable,arguments,0,0,FALSE,CREATE_NEW_CONSOLE,0,0,&si,&pi);//CREATE_DEFAULT_ERROR_MODE
  int r = CreateProcess(executable,arguments,0,0,FALSE,0,0,0,&si,&pi);//
  printf("pid:%d ,r:%d\n",pi.hProcess,r);
  int err = GetLastError();
  printf("err:%d\n",err);
}

int execute_process2(char *executable,char *arguments)
{
  printf("executing process:[%s],[%s]\n",executable,arguments);
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  //memset(&si,0,sizeof(si));
  memset(&pi,0,sizeof(pi));
  //si.dwFlags = STARTF_USESHOWWINDOW;
  //si.wShowWindow = SW_SHOWNORMAL;
  si.cb = sizeof(si);
  GetStartupInfo(&si);
  //int r = CreateProcess(executable,arguments,0,0,FALSE,CREATE_NEW_CONSOLE,0,0,&si,&pi);//CREATE_DEFAULT_ERROR_MODE
  int r = CreateProcess(executable,arguments,0,0,FALSE,CREATE_NEW_CONSOLE,0,0,&si,&pi);//
  printf("pid:%d ,r:%d\n",pi.hProcess,r);
  int err = GetLastError();
  printf("err:%d\n",err);
}





int open_url(char *url)
{
  execute_shell("open",url);
}

int extract_payload(FILE *in,char *filename,char *payload,unsigned long payload_len)
{
  FILE *out = fopen(filename,"ab+");
  append_data(payload,payload_len,out);
  fclose(out);
}


int rename_file()
{


}

int copy_file()
{

}


char *option_temp_output_filename="starter_output.exe";  
char *option_filename = NULL;

int option_rename_file = 0;
int option_delete_file = 0;
int option_copy_file = 0;
int option_patch_file = 0;
int option_execute_file = 0;
int option_check_tag = 0;

int check_options(int argc, char **argv)
{
  int c = 0;
  int ret = 0;
  while ((c = getopt (argc, argv, "trdxcPo:hp")) != -1)
  {
    switch (c)
    {
      /*case 'i':
        interactive_mode = 1;
        break;*/
      case 'r':
        option_rename_file = 1;
        break;
      case 'd':
        option_delete_file = 1;
        break;
      case 'c':
        option_copy_file = 1;
        break;
      case 'P':
        option_patch_file = 1;
        break;
      case 'x':
        option_execute_file = 1;
        break;
      case 't':
        option_check_tag = 1;
        break;
      case 'o':
        option_temp_output_filename = optarg;
        break;
      case 'h':
        printf(starter_helpmsg);
        return(0);
      case 'v':
        printf("starter\n");
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
  option_filename = argv[optind++];

}

int check_tagged_options(int argc, char **argv)
{
  int c = 0;
  int ret = 0;
  while ((c = getopt (argc, argv, "trdxmcPo:hp")) != -1)
  {
    switch (c)
    {
      /*case 'i':
        interactive_mode = 1;
        break;*/
      case 'r':
        option_rename_file = 1;
        break;
      case 'd':
        option_delete_file = 1;
        break;
      case 'c':
        option_copy_file = 1;
        break;
      case 'P':
        option_patch_file = 1;
        break;
      case 'x':
        option_execute_file = 1;
        break;
      case 't':
        option_check_tag = 1;
        break;
      case 'o':
        option_temp_output_filename = optarg;
        break;
      case 'h':
        printf(starter_helpmsg);
        return(0);
      case 'v':
        printf("starter\n");
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
  option_filename = argv[optind++];

}



int main(int argc, char** argv)
{
  unsigned long exe_len = 0;
 
  //printf("exe name:%s\n",argv[0]);
  FILE *in = fopen(argv[0],"rb");
  fseek(in,0,SEEK_END);
  exe_len = ftell(in);
  //printf("file size: %d\n",exe_len);
  tag *t = read_tag(in,exe_len);
  if(t!=NULL)
  {
    //printf("found tag(main)\n");
    int is_mz = check_for_mz(in);
    //if(is_mz)
    //  printf("is mz (main)\n");
    if(t->get_options)
    {
      check_tagged_options(argc,argv);
      if(option_check_tag)
      {
        printf("found tag\n");
      }
      /*if(option_rename_file)
      {
        rename(argv[0])
        return(0);
      }*/
      if(option_copy_file)
      {
        printf("copy to :%s\n",option_filename);
        remove(option_filename);
        FILE *out = fopen(option_filename,"ab+");
        copy_itself_to_file(in,exe_len,out);
        fclose(out);
        fclose(in);
        return(0);
      }
      if(option_delete_file)
      {
        printf("delete :%s\n",option_filename);
        remove(option_filename);
        return(0);
      }
      if(option_patch_file)
      {
        printf("patch to :%s\n",option_filename);
        remove(option_filename);
        FILE *out = fopen(option_filename,"ab+");
        copy_itself_to_file(in,exe_len,out);
        fclose(out);
        fclose(in);
        out = fopen(option_filename,"rb+");
        fseek(out,0,SEEK_END);
        exe_len = ftell(out);
        t->get_options = 0;
        update_tag(out,exe_len,t);
        fclose(out);
        //remove(argv[0]);
        spawnl(P_OVERLAY,"c:\\windows\\system32\\cmd.exe","cmd.exe","/c","del",argv[0],NULL);
        return(0);
      }
    }

    unsigned long index=0;
    unsigned long tag_offset = exe_len - sizeof(tag);
    //printf("to:%d\n",tag_offset);
    while(index<t->num_entries)
    {
      entry ei;
      //printf("found entry:%d\n",index);
      fseek(in,tag_offset-sizeof(entry),SEEK_SET);
      fread(&ei,sizeof(entry),1,in);
      tag_offset = tag_offset - sizeof(entry);
      //printf("st:%d,se:%d\n",sizeof(tag),sizeof(entry));
      //printf("to:%d ,cl:%d,pl:%d,2pl:%d\n",tag_offset,ei.command_len,ei.payload_len,ei.second_payload_len);
      char *command = (char*)malloc(ei.command_len);  
      char *payload = NULL;
      char *second_payload = NULL;
      if(ei.second_payload_len)
      {
        second_payload = (char*)malloc(ei.second_payload_len);  
        fseek(in,tag_offset-ei.second_payload_len,SEEK_SET);
        fread(second_payload,ei.second_payload_len,1,in);
        tag_offset = tag_offset - ei.second_payload_len;
        //printf("to:%d\n",tag_offset);
      }
      if(ei.payload_len)
      {
        payload = (char*)malloc(ei.payload_len);  
        fseek(in,tag_offset-ei.payload_len,SEEK_SET);
        fread(payload,ei.payload_len,1,in);
        tag_offset = tag_offset - ei.payload_len;
        //printf("to:%d\n",tag_offset);
      }
      fseek(in,tag_offset-ei.command_len,SEEK_SET);
      fread(command,ei.command_len,1,in);
      tag_offset = tag_offset - ei.command_len;
      //printf("cto:%d\n",tag_offset-ei.command_len);
      printf("executing:[%s]\n",command);  
      //printf("payload:[%s]\n",payload);  
      //printf("2nd payload:[%s]\n",second_payload);  
      if(!strcmp(command,"url"))
        open_url(payload);
      else if(!strcmp(command,"remove"))
        remove(payload);
      else if(!strcmp(command,"extract"))
      {
        remove(payload);
        extract_payload(in,payload,second_payload,ei.second_payload_len);
      }
      else if(!strcmp(command,"execute"))
      {
        //int pid = spawnl(P_WAIT,payload,payload,second_payload,NULL);
        //wait(pid);
        //execute_process2(payload,second_payload);
        char *cmd = str_Cat(payload," ");
        cmd = str_CatFree(cmd,second_payload);
        system(cmd);
        free(cmd);
      }

      index++;
    }


    free(t);
    fclose(in);
  }
  else
  {
    check_options(argc,argv);
    if(option_check_tag)
    {
      printf("no starter tag found\n");
    }
    if(option_copy_file)
    {
      printf("copy to :%s\n",option_filename);
      remove(option_filename);
      FILE *out = fopen(option_filename,"ab+");
      copy_itself_to_file(in,exe_len,out);
      fclose(out);
      fclose(in);
      return(0);
    }

    if(option_patch_file)
    {
      printf("patch to :%s\n",option_filename);
      remove(option_filename);
      FILE *out = fopen(option_filename,"ab+");
      copy_itself_to_file(in,exe_len,out);
      fclose(out);
      fclose(in);
      spawnl(P_NOWAIT,option_filename,option_filename,"-d",argv[0],NULL);
      return(0);
    }

    if(option_delete_file)
    {
      printf("delete :%s\n",option_filename);
      remove(option_filename);
      return(0);
    }

    //char *command = "url";
    char *command = option_filename;
    //char *payload = "http://www.works.org/";
    char *payload = argv[optind++];
    char *second_payload = argv[optind++];
    unsigned long payload_len = 0;
    if(payload)
      payload_len = strlen(payload)+1;
    unsigned long second_payload_len = 0;
    if(second_payload)
      second_payload_len = strlen(second_payload)+1;
    remove(option_temp_output_filename);
    FILE *out = fopen(option_temp_output_filename,"ab+");
    copy_itself_to_file(in,exe_len,out);

    char *nyx_command = "extract";
    char *nyx_rm_command = "remove";
    char *nyx_file = "temp.nyx";

    add_entry(nyx_rm_command,nyx_file,strlen(nyx_file)+1,NULL,0,out);

    add_entry(command,payload,payload_len,second_payload,second_payload_len,out);

    FILE *in_nyx = fopen("tests/nyx/websock_http.nyx","rb+");
    fseek(in_nyx,0,SEEK_END);
    unsigned long nyx_len = ftell(in_nyx);
    fseek(in_nyx,0,SEEK_SET);
    char *nyx = (char*)malloc(nyx_len);
    fread(nyx,nyx_len,1,in_nyx);
    fclose(in_nyx);
    add_entry(nyx_command,nyx_file,strlen(nyx_file)+1,nyx,nyx_len,out);
    free(nyx);

    add_tag(3,out);
    fclose(out);
    fclose(in);

    spawnl(P_NOWAIT,option_temp_output_filename,option_temp_output_filename,"-P",argv[0],NULL);
  }
}