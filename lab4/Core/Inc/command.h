#define MAX_BUF 32

typedef struct buf_t {
  uint8_t buffer[MAX_BUF];
  uint8_t head, tail;
} buf_t;

typedef struct command {
  char * cmd_string;
  void (*cmd_function)(char * arg);
} command_t;

extern buf_t buf;
