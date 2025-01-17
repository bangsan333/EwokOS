#ifndef SHELL_H
#define SHELL_H

#include <sys/mstr.h>

extern bool _stdio_inited;
extern bool _console_inited;
extern bool _initrd;
extern bool _terminated;

typedef struct st_old_cmd {
	str_t* cmd;
	struct st_old_cmd* next;
	struct st_old_cmd* prev;
} old_cmd_t;
extern old_cmd_t* _history;
extern old_cmd_t* _history_tail;

int32_t gets(int fd, str_t* buf);
int32_t handle_shell_cmd(const char* cmd);
void    add_history(const char* cmd);
void    free_history(void);

#endif