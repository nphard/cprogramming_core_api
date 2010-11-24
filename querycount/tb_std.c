#include "tb_std.h"


/* DYNAMIC STRING API implementation */

D_STRING init_d_string(int size){
  D_STRING ds=NULL;

  ds = (D_STRING)malloc(D_STRING_SIZE);
  if(ds == NULL)
    return NULL;
  memset(ds,0,D_STRING_SIZE);
  size=size+1; /* for the trailing \0 */
  
  ds->buff=(char*)malloc(size*sizeof(char));
  memset(ds->buff,0,sizeof(char)*size);
  ds->last_c=0;
  ds->size=size;
  return ds;
}


void free_d_string(D_STRING ds){
  
  if(ds){
    if(ds->buff)
      free(ds->buff);
    free(ds);
  }
}

void clear_d_string(D_STRING ds){
  if(ds && ds->buff){
    ds->buff[0]='\0';
  }
  ds->last_c=0;
}

char* get_buff_d_string(D_STRING ds){
  if(ds)
    return ds->buff;
  else 
    return NULL;
}


void resize_d_string(D_STRING ds, int size){
  char* newbuff=NULL;

  size=size+1;
  if(size <= ds->size)
    return;
  newbuff=(char*)malloc(sizeof(char)*size);
  memset(newbuff,0, sizeof(char)*size);
  if(ds->buff){
    memcpy(newbuff,ds->buff,sizeof(char)*ds->last_c);
    free(ds->buff);
  }
  ds->buff=newbuff;
  ds->size=size;
}

int set_at_d_string(D_STRING ds, int pos, char c){
  if(pos >= ds->size)
    resize_d_string(ds,pos+10);

  ds->buff[pos]=c;
  if(pos>ds->last_c)
    ds->last_c=pos;
  return pos;
}

int strcat_d_string(D_STRING ds, char*src){
  int len;
  if(ds == NULL || src==NULL)
    return -1;
  
  len=strlen(src);
  if((ds->last_c+len) >= ds->size)
    resize_d_string(ds, ds->last_c+len+10);
  
  strcpy(ds->buff+ds->last_c, src);
  ds->last_c += len;
  return 0;
}

int strcpy_d_string(D_STRING ds, char* src){
  
  if(src==NULL || ds==NULL)
    return -1;
  int len=strlen(src);
  if((len+1)>=ds->size)
    resize_d_string(ds,len+10);
  
  clear_d_string(ds);
  strcpy(ds->buff,src);
  ds->last_c=len;
  return 0;
}

char* fgets_d_string(D_STRING ds, FILE *f){
  char* str;
  int read_size=0,len;

  if(ds==NULL || f==NULL)
    return NULL;
  
  if(feof(f) || ferror(f)){
    return NULL;
  }
  clear_d_string(ds);
  resize_d_string(ds,LINE_MAX_BUFFER_SIZE);
  
  for(;;){
    str = ds->buff+ds->last_c;
    read_size = ds->size-ds->last_c;
    str[read_size-1]='\007';

    if(fgets(ds->buff,read_size,f) == NULL){
      break; /* read error or EOF */
    }
    
    if(str[read_size-1]=='\007')
      break;

    if(str[read_size-1]=='\0' && str[read_size-2]=='\n')
      break;

    ds->last_c=ds->size;
    resize_d_string(ds,ds->size+LINE_MAX_BUFFER_SIZE);
  }
  len =strlen((char*)get_buff_d_string(ds));
  ds->last_c=len;

  /* remove the trailing line break */
  while(ds->last_c >0 &&
	(ds->buff[ds->last_c-1]=='\n' || ds->buff[ds->last_c-1]== '\r')){
    ds->last_c--;
  }
  ds->buff[ds->last_c]='\0';
  return get_buff_d_string(ds);
}


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
