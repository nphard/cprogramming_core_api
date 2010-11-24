#include <stdio.h>
#include <stdlib.h>

#include "tb_std.h"
#include "querycount.h"


/* write the queries and count in the chunk buffer to the temp disk file*/
void write_queries_chunk_buffer(FILE* f,D_STRING_LIST dsl){
  char* pre_str=NULL, *str;
  int count=0,i;
  if(!f || !dsl)
    return;
  for(i=0;i<dsl->last_idx+1;i++){
    str=get_at_d_string_list(NULL, dsl,i);
    if(!str){
      continue;
    }
    if(!pre_str || !strcmp(pre_str,str)){
      pre_str=str;
      count++;
    }
    else{
      fprintf(f,"%d %s\n",count, pre_str);
      pre_str=str;
      count = 1;
    }
  }
  fprintf(f,"%d %s\n",count,pre_str);
}


void min_heap_merge(DP_LIST files,
		    int nb_files,
		    char* merge_name){
  DP_LIST queries=NULL;
  QUERY_COUNT qc=NULL, pre_qc;
  D_STRING line=NULL;
  FILE* fout=NULL,*fin=NULL;
  MIN_HEAP heap=NULL;
  int i, pos=1,finished=0,stream_id=0,end_query=0;;
  
  /* initialize the reading query buffer */
  fout=fopen(merge_name,"wb");
  queries = init_dp_list(nb_files+1);
  line=init_d_string(20);
  pre_qc = init_query_count();

  for(i=0;i<nb_files+1;i++){
    qc=init_query_count();
    set_at_dp_list(queries,i,(void*)qc);
  }

  for(i=0;i<nb_files;i++){
    fin=(FILE*)get_at_dp_list(files,i);
    if(fin){
      if(fgets_d_string(line,fin)){
	qc=(QUERY_COUNT)get_at_dp_list(queries,pos++);
	parse_query_count(qc,get_buff_d_string(line),i);
      }
      else{
	fclose(fin);
	set_at_dp_list(files,i,NULL);
      }
    }
  }

  heap=init_min_heap(queries,pos-1, &compare_query);
  build_min_heap(heap);

  qc=get_min_item_min_heap(heap);
  pos=2;
  while(qc->count!= 0){
    
    if(pre_qc->count == 0){
      cp_query_count(pre_qc,qc);
    }
    else if(!strcmp(get_buff_d_string(pre_qc->query),
		    get_buff_d_string(qc->query))){
      /* the same queries with last reading, wait until final writing */
      pre_qc->count += qc->count;
      pre_qc->stream_id = qc->stream_id;
    }
    else{
      /* write the queries to temp chunk file*/
      fprintf(fout,"%d %s\n",pre_qc->count,get_buff_d_string(pre_qc->query));
      clear_query_count(pre_qc);
      cp_query_count(pre_qc, qc);
    }
    clear_query_count(qc);

    /* read a new record from the same chunk stream */
    stream_id=pre_qc->stream_id;
    fin=(FILE*)get_at_dp_list(files,stream_id);
    i=0;
    while(qc->count==0 && end_query==0){
      if(fin!=NULL){
	if(fgets_d_string(line,fin)){
	  parse_query_count(qc,get_buff_d_string(line),stream_id);
	}
	else{
	  /* finished with this file */
	  fclose(fin);
	  fin=NULL;
	  set_at_dp_list(files,stream_id,NULL);
	}
      }
      else{
	stream_id++;
	stream_id=stream_id%(nb_files);
	fin=(FILE*)get_at_dp_list(files,stream_id);   
	i++;
	/* all the queries have been read from stream */
	if(i>nb_files){
	  end_query=1;
	}
      }
    }
    /* replace the min node with another one, need heapify */
    heapify_min_heap(heap, 1);
    qc=get_min_item_min_heap(heap);
  }

  if(pre_qc->count > 0){
    fprintf(fout,"%d %s\n",pre_qc->count,get_buff_d_string(pre_qc->query));
    clear_query_count(pre_qc);
  }

  /* free resources */
  for(i=0;i<queries->size;i++){
    qc=(QUERY_COUNT)get_at_dp_list(queries,i);
    free_query_count(qc);
  }
  free_dp_list(queries);
  free_min_heap(heap);
  free_query_count(pre_qc);
  free_d_string(line);
  fclose(fout);
}


int multiway_merge(char* prefix, 
		   int chunk_idx, 
		   int multi_k, 
		   int level,
		   int clean){
  DP_LIST files, loser_tree;
  int i=0,merge_idx=0,nb_files=0,output_level,j=0;
  FILE* f=NULL;
  char chunk_name[15];
  char merge_name[15];
  
  files=init_dp_list(multi_k);
  output_level=level+1;

  /* iterate the chunk files and merge*/
  for(i=0;i<chunk_idx;i++){
    if(multi_k==nb_files){
      sprintf(merge_name,"%s_%d_%d",prefix,output_level,merge_idx++);
      min_heap_merge(files, nb_files, merge_name);
      if(clean){
	/* clean the temp files */
	for(j=0;j<nb_files;j++){
	  sprintf(chunk_name,"%s_%d_%d",prefix,level,i-j-1);
	  remove(chunk_name);
	}
      }
      clear_dp_list(files);
      nb_files=0;
    }
    sprintf(chunk_name,"%s_%d_%d",prefix,level,i);
    f=fopen(chunk_name,"rb");
    if(f){
      set_at_dp_list(files,nb_files++,f);
    }
    else{
      break;
    }
  }
  if(nb_files > 0){
    sprintf(merge_name,"%s_%d_%d",prefix,output_level,merge_idx++);
    min_heap_merge(files, nb_files, merge_name);
    if(clean){
      /* clean the temp files */
      for(j=0;j<nb_files;j++){
	  sprintf(chunk_name,"%s_%d_%d",prefix,level,i-j-1);
	  remove(chunk_name);
      }
    }
    clear_dp_list(files);
  }
  free_dp_list(files);
  
  if(merge_idx > 1)
    output_level=multiway_merge(prefix, merge_idx, multi_k,output_level,clean);
  return output_level;
}

void write_query_counts(DP_LIST queries, int start, int end,FILE* fout){
  
  int i;
  QUERY_COUNT qc;

  for(i=start;i<end+1;i++){
    qc=(QUERY_COUNT)get_at_dp_list(queries,i);
    if(qc->count>0){
      fprintf(fout, "%d %s\n",qc->count,get_buff_d_string(qc->query));
    }
  }
}

int select_top_queries(FILE* fin, int top, FILE* fout){
  
  int count=0,i=0;
  QUERY_COUNT qc,min_qc;
  DP_LIST top_queries;
  D_STRING ds;
  MIN_HEAP heap;
  
  ds= init_d_string(20);
  top_queries=init_dp_list(top+1);
  
  for(i=0;i<top_queries->size;i++){
    qc=init_query_count();
    set_at_dp_list(top_queries,i, qc);
  }
    
  while(count< top && fgets_d_string(ds,fin) != NULL){
    qc=(QUERY_COUNT)get_at_dp_list(top_queries,count+1);
    parse_query_count(qc,get_buff_d_string(ds),0);
    count++;
  }
  
  if(count < top){
    write_query_counts(top_queries,1,count,fout);
  }
  else{
    heap=init_min_heap(top_queries,count, compare_query_counts);
    build_min_heap(heap); 
    qc=init_query_count();
    while(fgets_d_string(ds,fin)!=NULL){
      parse_query_count(qc,get_buff_d_string(ds),0);
      min_qc=get_min_item_min_heap(heap);
      if(compare_query_counts(min_qc,qc) <0){
	cp_query_count(min_qc,qc);
	heapify_min_heap(heap,1);
      }
    }
    free_query_count(qc);
    free_min_heap(heap);
    write_query_counts(top_queries,1,top,fout);
  }
  
  for(i=0;i<top+1;i++){
    qc=(QUERY_COUNT)get_at_dp_list(top_queries,i);
    free_query_count(qc);
  }
  free_dp_list(top_queries);
  free_d_string(ds);
  return count;
}

void print_help(char* msg){
  fprintf(stderr,"ERROR! %s\n", msg);
  fprintf(stderr,"_querycount --log log.txt --top 8 --multiway 4 --chunk 10 --clean\n");
  exit(1);
}


int main(int argc, char** argv){
  int i=0,chunk_idx=0, clean=0;
  int topn=0, chunk_size=1000000, multi_k=6, output_level;
  char *logfile=NULL;
  char chunk_name[10];
  char *outfile=NULL;
  char *str=NULL;
  D_STRING ds_line=NULL;
  D_STRING_LIST ds_list=NULL;
  
  FILE* logf=NULL, *cf=NULL, *fout=stdout;;

  for(i=1; i<argc; i++){
    if(!strcmp("--log",argv[i])){
      logfile = argv[++i];
    }
    else if(!strcmp("--top", argv[i])){
      topn = atoi(argv[++i]);
      if(topn < 1)
	print_help("top arguments should be bigger than 0.");
    }
    else if(!strcmp("--multiway", argv[i])){
      multi_k=atoi(argv[++i]);
      if(multi_k < 2){
	print_help("top arguments should be bigger than 1.");
      }
    }
    else if(!strcmp("--chunk", argv[i])){
      chunk_size = atoi(argv[++i]);
      if(chunk_size < 1)
	print_help("chunk_size should be bigger than 0.");
    }
    else if(!strcmp("--out" ,argv[i])){
      outfile=argv[++i];
      fout=fopen(outfile,"wb");
      if(!fout){
	print_help("can't open output file.");
      }
    }
    else if(!strcmp("--clean",argv[i])){
      clean=1;
    }
    else{
      /* unknown arguments */
      print_help("unknown arguments!");
    }
  }
  
  ds_line=init_d_string(20);
  sprintf(chunk_name,"chunk_%d",0);

  logf=fopen(logfile,"rb");
  if(logf==NULL){
    print_help("Can't read the query log file!");
  }

  /* read the stream of chunk_size buffer, sort and store it in the temp chunk file */
  chunk_idx=0;
  ds_list=init_d_string_list(chunk_size);
  i=0;
  while(fgets_d_string(ds_line, logf) != NULL){
    str=get_buff_d_string(ds_line);
    if(!str || strlen(str)==0){
      continue;
    }
    set_at_d_string_list(ds_list,i++,str);

    if((i+1)>chunk_size){
      /* sort the queries in the buffer */
      sort_d_string_list(ds_list);

      /* begin to output the temp chunk */ 
      sprintf(chunk_name,"chunk_0_%d",chunk_idx);
      cf=fopen(chunk_name,"wb");
      if(cf==NULL){
	fprintf(stderr,"error open chunk temp file %s to write.", chunk_name);
	exit(1);
      }
      write_queries_chunk_buffer(cf,ds_list);
      fclose(cf);

      /* clear a new chunk temp buffer */
      i=0;
      chunk_idx++;
      clear_d_string_list(ds_list);
    }
  }
  fclose(logf);
  /* process the left queries in buffer */
  if(i> 0){
    /* sort the queries in the buffer */
    sort_d_string_list(ds_list);
    
    /* begin to output the temp chunk */ 
    sprintf(chunk_name,"chunk_0_%d",chunk_idx);
    cf=fopen(chunk_name,"wb");
    if(cf==NULL){
      fprintf(stderr,"error open chunk temp file %s to write.", chunk_name);
      exit(1);
    }
    write_queries_chunk_buffer(cf,ds_list);
      fclose(cf);
  }
  
  /* multiway merge */
  output_level=multiway_merge("chunk",chunk_idx, multi_k,0,clean);
  
  sprintf(chunk_name,"chunk_%d_0",output_level);
  logf=fopen(chunk_name,"rb");
  
  /* output the result */
  select_top_queries(logf,topn,fout);

  /* release the resources */
  if(clean){
    remove(chunk_name);
  }
  fclose(logf);
  fclose(fout);
  free_d_string(ds_line);
  free_d_string_list(ds_list);
}

