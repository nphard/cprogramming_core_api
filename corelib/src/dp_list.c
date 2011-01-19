/***********************************************************************
 *  Author by Jin
 ***********************************************************************/

#include "dp_list.h"

/* Dynamic pointer array */

DP_LIST init_dp_list(int size){
  DP_LIST list=NULL;
  list=(DP_LIST)malloc(DP_LIST_SIZE);
  if(list==NULL)
    return NULL;

  memset(list,0, DP_LIST_SIZE);
  list->buff=malloc(sizeof(void*)*size);
  memset(list->buff,0, sizeof(void*)*size);
  list->size=size;
  return list;
}
void free_dp_list(DP_LIST list){
  if(!list)
    return;

  if(list->buff)
    free(list->buff);
  free(list);
}

void clear_dp_list(DP_LIST list){
  if(list && list->buff){
    memset(list->buff,0, sizeof(void*) * list->size);
  }
  list->last_idx=0;
}

void resize_dp_list(DP_LIST list, int size){
  void** buff;
  
  if(size<=list->size){
    return;
  }
  buff=malloc(sizeof(void*)*size);

  if(list->buff){
    memcpy(buff,list->buff,sizeof(void*)*list->size);
    free(list->buff);
  }
  list->buff=buff;
  list->size=size;
}

void set_at_dp_list(DP_LIST list, int pos, void* ptr){
  if(!list || !list->buff)
    return;

  if(list->size <= pos){
    resize_dp_list(list,pos+10);
  }
  if(pos > list->last_idx){
    list->last_idx=pos;
  }
  list->buff[pos]=ptr;

}

void* get_at_dp_list(DP_LIST list, int pos){
  if(!list || !list->buff){
    return NULL;
  }
  if(pos < 0 || pos>list->last_idx){
    return NULL;
  }
  return (void*)list->buff[pos];
}
