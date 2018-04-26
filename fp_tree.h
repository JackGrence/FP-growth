#include <stdio.h>
#include <stdbool.h>
#include "heap_sort.h"

#ifndef FP_TREE
#define FP_TREE

#define NUM_OF_INIT_CANDIDATE 1
#define NODE_LEN(HASH_INDEX) (HASH_FUNC_MOD - 1 - HASH_INDEX)
#define NODE_INDEX(LEN, HASH_INDEX) (HASH_INDEX - (HASH_FUNC_MOD - LEN))

int HASH_FUNC_MOD;
int MAX_LEAF_SIZE;

typedef struct _fp_node fp_node;

struct _fp_node
{
  int index;
  int childs_len;
  int count;
  fp_node **childs;
  fp_node *parent;
};

typedef struct _fp_node_link_list fp_node_link_list;

struct _fp_node_link_list
{
  fp_node *node;
  fp_node_link_list *next;
};

typedef struct _item_set
{
  int count;
  int size;
  int *items;
} item_set;

typedef struct _fp_tree fp_tree;

struct _fp_tree
{
  fp_node *root;
  item_set *freq_item_set;
  fp_node_link_list **header_table;
  heap_ary *order_to_ID;
};


item_set *create_item_set (int *item, int item_size);
fp_node *create_fp_node (int index);
fp_tree *create_fp_tree (item_set *freq_item_set);
void free_item_set (item_set *ptr);
void free_fp_node (fp_node *ptr);
void free_fp_tree (fp_tree *ptr);
void fp_tree_insert (int *insert_item, int item_size, fp_tree *tree);
bool fp_is_empty (fp_node *node);
fp_node_link_list *create_fp_node_link_list (void);
void insert_fp_node_link_list (fp_node_link_list *list, fp_node *node);
void free_fp_node_link_list (fp_node_link_list *list);

#endif /* ifndef HASH_TREE */
