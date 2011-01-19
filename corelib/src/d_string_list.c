/***********************************************************************
 *  Author by Jin
 *
 ***********************************************************************/

#include "d_string_list.h"

/* DYNAMIC STRING LIST implementation */

D_STRING_LIST init_d_string_list(int size){
  D_STRING_LIST list=NULL;
  
  list = (D_STRING_LIST)malloc(D_STRING_LIST_SIZE);
  if(!list)
    return NULL;
  memset(list,0,D_STRING_LIST_SIZE);
  
  list->buff=(D_STRING*)malloc(size*sizeof(D_STRING));
  memset(list->buff,0,size*sizeof(D_STRING));
  list->size=size;

  return list;
}

void free_d_string_list(D_STRING_LIST list){
  int i;
  D_STRING ds=NULL;
  if(!list)
    return;
  for(i=0;i<list->size;i++){
    ds=(D_STRING)list->buff[i];
    if(ds)
      free_d_string(ds);
  }
  free(list->buff);
  free(list);
}

void resize_d_string_list(D_STRING_LIST list, int size){

  D_STRING *newbuff=NULL;

  if(size <= list->size){
    return;
  }
  newbuff=(D_STRING*)malloc(sizeof(D_STRING)*size);
  memset(newbuff,0,sizeof(D_STRING)*size);
  if(list->buff){
    memcpy(newbuff,list->buff,sizeof(D_STRING)*list->size);
    free(list->buff);
  }
  list->buff=newbuff;
  list->size=size;
}

int calc_mem_d_string_list(D_STRING_LIST list){
  int i;
  int total=0;
  D_STRING ds;
  
  for(i=0;i<list->size;i++){
    ds=(D_STRING)list->buff[i];
    if(ds){
      total+=ds->size+D_STRING_SIZE;
    }
  }
  total+= sizeof(D_STRING)*list->size;
  return total;
}

char* get_at_d_string_list(D_STRING ds,D_STRING_LIST list, int pos){
  char* strp=NULL;
  D_STRING dsp=NULL;
  if(pos >list->last_idx || pos < 0)
    return strp;
  dsp=(D_STRING)list->buff[pos];
  if(!dsp)
    return NULL;
  strp=get_buff_d_string(dsp);
  if(ds)
    strcpy_d_string(ds,strp);
  return strp;
}

int set_at_d_string_list(D_STRING_LIST list, int pos, char* str){
  D_STRING ds=NULL;

  if(!list || pos < 0)
    return -1;
  if((pos+1) > list->size){
    resize_d_string_list(list,pos+10);
  }
  ds=(D_STRING)list->buff[pos];
  if(!ds){
    ds=init_d_string(10);
    list->buff[pos]=ds;
  }
  strcpy_d_string(ds,str);
  if(pos > list->last_idx)
    list->last_idx=pos;
  return pos;
}


void clear_d_string_list(D_STRING_LIST list){
  int i;
  D_STRING ds=NULL;
  if(!list)
    return;
  for(i=0; i<list->last_idx;i++){
    ds=(D_STRING)list->buff[i];
    if(ds){
      /* we just clean the ds in case we need use it again */
      clear_d_string(ds);
    }
  }
  list->last_idx=0;
}

/* write the d_string_list to the disk, separated with \n" */
void write_d_string_list(FILE* f, D_STRING_LIST dsl){
  int i;
  char* str=NULL;
  if(!f || !dsl)
    return;
  for(i=0;i<dsl->last_idx;i++){
    str=get_at_d_string_list(NULL, dsl,i);
    if(!str){
      continue;
    }
    fprintf(f, "%s\n", str);
  }
}

int compare_d_string(const void* a, const void* b){

  D_STRING ds1,ds2;
  ds1=*(D_STRING*)a;
  ds2=*(D_STRING*)b;
  return strcmp(get_buff_d_string(ds1),get_buff_d_string(ds2));
}



void sort_d_string_list(D_STRING_LIST dsl){
  /* use quick sort */
  qsort(dsl->buff,dsl->last_idx+1,sizeof(D_STRING),compare_d_string);
}
