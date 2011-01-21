#ifndef _MIN_HEAP_H_
#define _MIN_HEAP_H_

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

#endif /* _MIN_HEAP_H_ */
