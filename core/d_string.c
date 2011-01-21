/***********************************************************************
 *  Author by Jin Du
 ***********************************************************************/


#include "d_string.h"


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

