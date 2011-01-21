#ifndef _D_STRING_LIST_H_
#define _D_STRING_LIST_H_

#include "d_string.h"

/* DYNAMIC STRING ARRAY API */

typedef struct d_str_list{
  D_STRING *buff;
  int size;
  int last_idx;
}*D_STRING_LIST;

D_STRING_LIST init_d_string_list(int size);
void free_d_string_list(D_STRING_LIST list);
char* get_at_d_string_list(D_STRING ds,D_STRING_LIST list, int pos);
int set_at_d_string_list(D_STRING_LIST list, int pos, char* str);
void clear_d_string_list(D_STRING_LIST list);
int calc_mem_d_string_list(D_STRING_LIST list);

int compare_d_string(const void* a, const void* b);

#define D_STRING_LIST_SIZE  (sizeof(struct d_str_list))


#endif /* _D_STRING_LIST_H_ */
