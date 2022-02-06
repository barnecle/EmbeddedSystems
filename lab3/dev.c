#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void help_command(char *args);
void lof_command();
void lon_command();
void test_command(char *args);
int parse_command (char *line, char **command, char **args);
int execute_command(u_int8_t * line);

typedef struct command {
  char * cmd_string;
  void (*cmd_function)(char * arg);
} command_t;

command_t commands[] = {
  {"help",help_command},
  {"lof",lof_command},
  {"lon",lon_command},
  {"test",test_command},
  {0,0}
};


int main(){
  /*
  u_int8_t *str = "test,arg1,arg2,arg3";
  u_int8_t *cmd;
  u_int8_t *arg;
  parse_command (str, &cmd, &arg);
  free(cmd);
  free(arg);*/
  
  char buf[20];
  char c;
  int n = 0;
  printf("\r\nSystem is up and running!\n\r");
  printf("\r\nSTM$");
  while(1){
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
  }
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
  int i = 0;
  //char *line_left;
  char *line_temp = line;
  //strcpy(line_left, line);
  if(strchr(line, ',')==NULL){
    *command = malloc(sizeof(line));
    memcpy(*command, line, strlen(line)+1);
    return 0;
  }
  i = strlen(line) - strlen(line_temp); //find index comma is at
  *command = malloc(sizeof(char)*i);
  memcpy(*command, line, i); //copy chars from line until the comma.
    //printf("%c\n", command);
  line = line+i+1;
  *args = malloc(sizeof(line));
  memcpy(*args, line, strlen(line)+1);
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
void test_command(char *args){
  printf("test\n");
}
