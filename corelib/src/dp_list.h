#ifndef _DP_LIST_H_
#define _DP_LIST_H_

#include <stdio.h>

/* Dynamic pointer array api */
typedef struct d_ptr_list{
  void **buff;
  int size;
  int last_idx;
}*DP_LIST;

#define DP_LIST_SIZE (sizeof(struct d_ptr_list))

DP_LIST init_dp_list(int size);
void free_dp_list(DP_LIST list);
void clear_dp_list(DP_LIST list);
void resize_dp_list(DP_LIST list, int size);
void set_at_dp_list(DP_LIST list, int pos, void* ptr);
void* get_at_dp_list(DP_LIST list, int pos);


#endif /* _DP_LIST_H_ */
