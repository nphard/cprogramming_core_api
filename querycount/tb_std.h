#ifndef _TB_STD_H_
#define _TB_STD_H_


#include "stdio.h"
#include "stdlib.h"

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

/* MIN HEAP interface */ 

typedef int(*COMPAREFUNC)(void*a, void*b);

typedef struct str_min_heap{
  DP_LIST list;
  int size;
  COMPAREFUNC compare;
}*MIN_HEAP;

#define MIN_HEAP_SIZE (sizeof(struct str_min_heap))

/* the list index for the element should be started with 1,
   list[0] is a backup temp buffer */
MIN_HEAP init_min_heap(DP_LIST list, int size, COMPAREFUNC func);
void free_min_heap(MIN_HEAP heap);

void heapify_min_heap(MIN_HEAP heap, int node);

void build_min_heap(MIN_HEAP heap);

void* get_min_item_min_heap(MIN_HEAP heap);


     
#endif  /* _TB_STD_H_ */
