#ifndef HEAP_SORT
#define HEAP_SORT

typedef struct _heap_node heap_node;

struct _heap_node
{
  int itemID;
  int frequent;
};

typedef struct _heap_ary heap_ary;

struct _heap_ary
{
  int ary_len;
  int latest;
  heap_node *ary;
};

// you can change compare member by this macro
#define CMP_MEMBER_OFFSET offsetof(heap_node, frequent)
#define CMP_MEMBER_TYPE typeof(((heap_node *)0)->frequent)

void pSort (void);
void pRest (int indx, char str[]);
int pop (int ary[]);
void insert (int item);
int heap_test (void);

heap_ary *create_heap_ary (int ary_size);
void free_heap_ary (heap_ary *heap_ary_ptr);
void insert_min_heap (heap_ary *heap, heap_node *node);
int sort_min_heap (heap_ary *heap);
void pop_min_heap (heap_ary *heap);

#endif /* ifndef HEAP_SORT */
