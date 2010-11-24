#ifndef _QUERYCOUNT_H_
#define _QUERYCOUNT_H_

#include "tb_std.h"

typedef struct str_query_count{
  D_STRING query;
  int count;
  int stream_id;
}*QUERY_COUNT;

#define QUERY_COUNT_SIZE (sizeof(struct str_query_count))

QUERY_COUNT init_query_count();
void free_query_count(QUERY_COUNT qc);
void clear_query_count(QUERY_COUNT qc);
void cp_query_count(QUERY_COUNT qc1, QUERY_COUNT qc2);

int parse_query_count(QUERY_COUNT qc, char* line, int stream_id);

int compare_query(void* q1, void* q2);
int compare_query_counts(void* q1, void* q2);


#endif /* _QUERYCOUNT_H_ */
