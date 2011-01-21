#ifndef _D_STRING_H_
#define _D_STRING_H_


#include <stdio.h>


/* DYNAMIC STRING API */
#define LINE_MAX_BUFFER_SIZE 1024

typedef struct str_d_str{
  char* buff;
  int size;
  int last_c;
}*D_STRING;

#define D_STRING_SIZE (sizeof(struct str_d_str))


D_STRING init_d_string(int size);
void free_d_string(D_STRING ds);
char* get_buff_d_string(D_STRING ds);
void clear_d_string(D_STRING ds);

void resize_d_string(D_STRING ds, int size);
int set_at_d_string(D_STRING DS, int pos, char c);
int strcpy_d_string(D_STRING ds, char* src);
int strcat_d_string(D_STRING ds, char* src);

char* fgets_d_string(D_STRING ds, FILE* f);


#endif  /* _D_STRING_H_ */
