#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void help_command(char *args);
void lof_command();
void lon_command();
void test_command(char **args);
int parse_command (char *line, char **command, char **args);
int execute_command(u_int8_t * line);

typedef struct command {
  char * cmd_string;
  void (*cmd_function)(char * arg);
} command_t;

typedef struct buf_t {
  u_int8_t buffer[MAX_BUF];
  u_int8_t head, tail;
} buf_t;

command_t commands[] = {
  {"help",help_command},
  {"lof",lof_command},
  {"lon",lon_command},
  {"test",test_command},
  {0,0}
};

extern buf_t buf;
buf->head = buf->tail = 0;

int main(){
  /*
  u_int8_t *str = "test,arg1,arg2,arg3";
  u_int8_t *cmd;
  u_int8_t *arg;
  parse_command (str, &cmd, &arg);
  free(cmd);
  free(arg);*/
  
  char buf[20];
  char ch;
  int n = 0;
  printf("\r\nSystem is up and running!\n\r");
  printf("\r\nSTM$");
  while(1){
    if(buf->head != buf->tail && n<MAX_BUF){
      ch = buf->buffer[buf->tail];
      putchar(ch);
      if(buf->tail == (MAX_BUF-1)){
	buf->tail = 0;
      }else{
	buf->tail++;
      }
      if(ch == '\r' || ch == '\n'){
	if(execute_command(buf)){
	  print("OK\r\n");
	}
	n = 0;
	printf("\r\nSTM$");
      }
      n++;
    }else{
      printf("\r\nNOK\r\n");
      n=0;
      printf("\r\nSTM$");
    }
    }
  /* while(1){
    c = getchar();
    //putchar(c); for STM32, not cmd line run
    if(c == '\r' || c == '\n'){
      execute_command(buf);
      n = 0;
      printf("\r\nSTM$");
    } else{
      buf[n] = c;
      n++;
      }
      }*/
}

int execute_command(u_int8_t * line) {
  u_int8_t *cmd;
  u_int8_t *arg;
  command_t *p = commands;
  int success = 0;

  if (!line) {
    return (-1); // Passed a bad pointer 
  }
  if (parse_command(line,&cmd,&arg) == -1) {
    printf("Error with parse command\n\r");
    return (-1);
  }
  while (p->cmd_string) {
    if (!strcmp(p->cmd_string,(char *) cmd)) {
      if (!p->cmd_function) {
        return (-1);
      }
      (*p->cmd_function)((char *)arg);            // Run the command with the passed arguments
      success = 1;
      break;
    }
    p++;
  }
  if (success) {
    return (0);
  }
  else {
    return (-1);
  }
  }
int parse_command (char *line, char **command, char **args){

  char *line_temp;
  if((!line) || (!command) || (!args)){ //check for bad pointer
    return(-1); 
  }
  line_temp= line;
  *command = line;
  //strcpy(line_left, line);
  while(*line_temp != ','){ //increment through input to find end of command
    if(!*line_temp){
      *args = '\0'; //no arguments
      return(0);
    }
    line_temp++;
  }

  *line_temp = '\0'; //replace comma with null for end of command string
  args = line_temp +1; //one char past null is beginning of arguments
  return 0;
}
void help_command(char *args){
  printf("Available Commands:\n");
  printf("lon\n");
  printf("lof\n");
  printf("test\n");
}
void lof_command(){
  printf("LED off\n");
}
void lon_command(){
  printf("LED on\n");
}
void test_command(char **args){
  printf("test\n");
  //int i;
  //i = strlen(args);
  //printf("%d\n", i);
  printf("%s\n", args[0]);
  printf("%s\n", args[1]);
  
  
}
