#include "querycount.h"
#include "string.h"

QUERY_COUNT init_query_count()
{
  QUERY_COUNT qc=NULL;
  qc=(QUERY_COUNT)malloc(QUERY_COUNT_SIZE);
  memset(qc,0,QUERY_COUNT_SIZE);
  
  qc->query=init_d_string(10);
  qc->stream_id=-1;
  return qc;
}

void free_query_count(QUERY_COUNT qc){
  if(!qc)
    return;
  if(qc->query)
    free_d_string(qc->query);
  free(qc);
}

void clear_query_count(QUERY_COUNT qc){
  if(!qc)
    return;
  if(qc->query)
    clear_d_string(qc->query);
  qc->count=0;
  qc->stream_id=-1;
}

void cp_query_count(QUERY_COUNT qc1, QUERY_COUNT qc2){
  qc1->count=qc2->count;
  qc1->stream_id=qc2->stream_id;

  strcpy_d_string(qc1->query, get_buff_d_string(qc2->query));
}

/* parse the line "12 query" into query_count structure */
int parse_query_count(QUERY_COUNT qc, char* line, int stream_id){
  int pos=0;
  clear_query_count(qc);
  qc->stream_id=stream_id;
  while(*line != '\0' && *line != ' '){
    set_at_d_string(qc->query,pos++,*line);
    line++;
  }
  if(*line != ' '){
    clear_d_string(qc->query);
    return -1;
  }
  set_at_d_string(qc->query,pos, '\0');
  qc->count=atoi(get_buff_d_string(qc->query));
  clear_d_string(qc->query);
  strcpy_d_string(qc->query,line+1);
  return 0;
}

int compare_query(void* q1, void* q2){
  int status = 0;
  QUERY_COUNT qc1=(QUERY_COUNT)q1;
  QUERY_COUNT qc2=(QUERY_COUNT)q2;
  
  if(qc1->count==0 && qc2->count>0){
    return 1;
  }
  if (qc1->count> 0 && qc2->count==0){
    return -1;
  }
  if(qc1->count==0 && qc2->count==0){
    return 0;
  }

  status=strcmp(get_buff_d_string(qc1->query),get_buff_d_string(qc2->query));
  if(status > 0)
    status = 1;
  else if(status < 0)
    status = -1;
  else
    status=0;

  return status;
}

int compare_query_counts(void* q1, void* q2){
  int status = 0;
  QUERY_COUNT qc1=(QUERY_COUNT)q1;
  QUERY_COUNT qc2=(QUERY_COUNT)q2;

  if(qc1->count>qc2->count){
    return 1;
  }
  else if(qc1->count<qc2->count){
    return -1;
  }
  else{
    return 0;
  }
  
}

/* MIN_HEAP interface */

MIN_HEAP init_min_heap(DP_LIST list, int size, COMPAREFUNC func){
  MIN_HEAP heap=NULL;
  
  heap=(MIN_HEAP)malloc(MIN_HEAP_SIZE);
  memset(heap,0,MIN_HEAP_SIZE);
  heap->list=list;
  heap->size=size;
  heap->compare=func;
  return heap;
}

void free_min_heap(MIN_HEAP heap){
  if(heap)
    free(heap);
}

void heapify_min_heap(MIN_HEAP heap, int node){
  COMPAREFUNC func=NULL;
  void* leftp, *rightp, *nodep,*largestp;
  int left=2*node;
  int right=2*node+1;
  int min = 0;
  leftp=get_at_dp_list(heap->list,left);
  rightp=get_at_dp_list(heap->list,right);
  nodep=get_at_dp_list(heap->list,node);

  func=heap->compare;
  
  if(left <= heap->size && func(leftp,nodep) < 0){
    min=left;
    largestp=leftp;
  }
  else{
    min=node;
    largestp=nodep;
  }
  
  if(right <= heap->size && func(rightp,largestp) < 0){
    min=right;
  }
  

  if(min != node){
    /* exchange the node with the minimal value */
    set_at_dp_list(heap->list,node, get_at_dp_list(heap->list,min));
    set_at_dp_list(heap->list,min,nodep);
    heapify_min_heap(heap, min);
  }
}

void build_min_heap(MIN_HEAP heap){
  int i=0;
  
  for(i=heap->size/2; i>0;i--){
    heapify_min_heap(heap, i);
  }
}

void* get_min_item_min_heap(MIN_HEAP heap){

  return get_at_dp_list(heap->list,1);
}

