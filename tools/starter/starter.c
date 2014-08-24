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

int check_for_mz(FILE *in)
{
  fseek(in,0,SEEK_SET);
  unsigned short magic;
  fread(&magic,2,1,in);
  printf("MAGIC:%c%c\n",magic&255,(magic>>8)&255);
  if((magic&255)=='M' && ((magic>>8)&255)=='Z')
    return(1);
  return(0);
}

int check_for_tag(FILE *in,unsigned long len)
{
  fseek(in,len-sizeof(tag),SEEK_SET);
  tag ti;
  fread(&ti,sizeof(tag),1,in);
  printf("magic:%d\n",ti.magic);
  if(ti.magic == 123456)
    return(1);
  return(0);
}

tag *read_tag(FILE *in,unsigned long len)
{
  fseek(in,len-sizeof(tag),SEEK_SET);
  tag *ti = (tag*)malloc(sizeof(tag));
  fread(ti,sizeof(tag),1,in);
  printf("magic:%d\n",ti->magic);
  if(ti->magic == 123456)
    return(ti);
  free(ti);
  return(NULL);
}



int execute_popen(char *command)
{
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
    free(ret);
    free(buf);
    return(1);
  }
 return(0);
}

int execute_shell()
{

}

int execute_process()
{


}

int open_url()
{


}

int rename_file()
{


}

int delete_file()
{


}

int copy_file()
{

}

int copy_itself_to_file(FILE *in,unsigned long len,FILE *out)
{
  fseek(in,0,SEEK_SET);
  char *data = (char*)malloc(len);
  fread(data,len,1,in);
  write_data(data,len,0,out);
  free(data);
  
}

char *option_temp_output_filename="starter_output.exe";  

int option_rename_file = 0;
int option_delete_file = 0;
int option_copy_file = 0;
int option_execute_file = 0;
int option_check_tag = 0;


int check_options(int argc, char **argv)
{
  int c = 0;
  int ret = 0;
  while ((c = getopt (argc, argv, "trdxco:hp")) != -1)
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
  char *filename = argv[optind];

}

int check_tagged_options(int argc, char **argv)
{
  int c = 0;
  int ret = 0;
  while ((c = getopt (argc, argv, "trdxoc:hp")) != -1)
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
  char *filename = argv[optind];

}



int main(int argc, char** argv)
{
  unsigned long exe_len = 0;
 
  printf("exe name:%s\n",argv[0]);
  FILE *in = fopen(argv[0],"rb");
  fseek(in,0,SEEK_END);
  exe_len = ftell(in);
  printf("file size: %d\n",exe_len);
  tag *t = read_tag(in,exe_len);
  if(t!=NULL)
  {
    printf("found tag(main)\n");
    check_tagged_options(argc,argv);
    if(option_check_tag)
    {
      printf("found tag\n");
    }
    free(t);
  }
  else
  {
    check_options(argc,argv);
    if(option_check_tag)
    {
      printf("no starter tag found\n");
    }

  }

  fclose(in);
/*
  FILE *out = fopen(output_filename,"wb+");
  copy_itself_to_file(in,exe_len,out);
  tag t;
  t.magic=123456;
  t.num_entries = 1;
  entry e;
  char *exe = "url";
  char *args = "\"http://www.works.org/\"";
  e.exe_string_len = strlen(exe)+1;
  e.args_string_len = strlen(args)+1;
  write_data(exe,strlen(exe)+1,len,out);
  write_data(args,strlen(args)+1,len+strlen(exe)+1,out);
  write_data(&e,sizeof(entry),len+strlen(exe)+1+strlen(args)+1,out);
  write_data(&t,sizeof(tag),len+strlen(exe)+1+strlen(args)+1+sizeof(entry),out);

fseek(in,len-sizeof(tag),SEEK_SET);
tag ti;
fread(&ti,sizeof(tag),1,in);
printf("magic:%d\n",ti.magic);
if(ti.magic == 123456)
{
  entry ei;
  printf("found tag\n");
  fseek(in,len-sizeof(tag)-sizeof(entry),SEEK_SET);
  fread(&ei,sizeof(entry),1,in);
  char *exei = (char*)malloc(ei.exe_string_len);  
  char *argsi = (char*)malloc(ei.args_string_len);  
  fseek(in,len-sizeof(tag)-sizeof(entry)-ei.exe_string_len-ei.args_string_len,SEEK_SET);
  fread(exei,ei.exe_string_len,1,in);
  fseek(in,len-sizeof(tag)-sizeof(entry)-ei.args_string_len,SEEK_SET);
  fread(argsi,ei.args_string_len,1,in);
  printf("executing:[%s]\n",exei);  
  printf("args:[%s]\n",argsi);  
  STARTUPINFO	si;
  PROCESS_INFORMATION pi;
  //memset(&si,0,sizeof(si));
  memset(&pi,0,sizeof(pi));
  //si.dwFlags = STARTF_USESHOWWINDOW;
  //si.wShowWindow = SW_SHOWNORMAL;
  si.cb = sizeof(si);
  GetStartupInfo(&si);
  //CreateProcess(exei,argsi,0,0,TRUE,CREATE_NO_WINDOW,0,0,&si,&pi);
  //CreateProcess(NULL,argsi,0,0,TRUE,CREATE_NO_WINDOW,0,0,&si,&pi);
  //CreateProcess(exei,argsi,0,0,TRUE,NORMAL_PRIORITY_CLASS,0,0,&si,&pi);//CREATE_NEW_CONSOLE

  //int r = CreateProcess(exe,args,0,0,TRUE,CREATE_NO_WINDOW,0,0,&si,&pi);//CREATE_NEW_CONSOLE

  if(!strcmp("url",exei))
   ShellExecute(NULL, "open", argsi , NULL, NULL, SW_SHOWNORMAL); 
  else
  {
    int r = CreateProcess(exei,argsi,0,0,FALSE,CREATE_NEW_CONSOLE,0,0,&si,&pi);//CREATE_DEFAULT_ERROR_MODE
    printf("pid:%d ,r:%d\n",pi.hProcess,r);
    int err = GetLastError();
    printf("err:%d\n",err);
  }
}
fclose(in);
fclose(out);
*/
}