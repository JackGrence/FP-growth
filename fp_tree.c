#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "fp_tree.h"
#include "heap_sort.h"

void
fp_tree_insert (int *insert_item, int item_size, fp_tree *tree)
{
  int hash_index;
  int i;
  fp_node *node;
  node = tree->root;
  for (i = 0; i < item_size; i++)
    {
      hash_index = insert_item[i] - node->index - 1;
      if (node->childs[hash_index] == NULL)
	{
	  node->childs[hash_index] = create_fp_node (insert_item[i]);
	  insert_fp_node_link_list (tree->header_table[insert_item[i]],
				    node->childs[hash_index]);
	}
      node->childs[hash_index]->count++;
      node->childs[hash_index]->parent = node;
      node = node->childs[hash_index];
    }
}

fp_node *
create_fp_node (int index)
{
  if (index >= HASH_FUNC_MOD)
    {
      printf ("index too large\n");
      return NULL;
    }
  fp_node *new_node;
  new_node = (fp_node *) calloc (1, sizeof (fp_node));
  new_node->index = index;
  new_node->childs_len = HASH_FUNC_MOD - index - 1;
  new_node->childs =
    (fp_node **) calloc (new_node->childs_len, sizeof (fp_node *));
  return new_node;
}

item_set *
create_item_set (int *item, int item_size)
{
  item_set *new_set;
  new_set = (item_set *) calloc (1, sizeof (item_set));
  new_set->size = item_size;
  new_set->items = (int *) calloc (item_size, sizeof (int));
  memcpy (new_set->items, item, sizeof (int) * item_size);
  return new_set;
}

bool
fp_is_empty (fp_node *node)
{
  int match;
  int cmp_size;
  cmp_size = sizeof (fp_node *) * node->childs_len;
  fp_node **null_str;
  null_str = (fp_node **) alloca (cmp_size);
  memset (null_str, 0, cmp_size);
  match = !memcmp (null_str, node->childs, cmp_size);
  return match;
}

fp_tree *
create_fp_tree (item_set *freq_item_set, int header_table_len)
{
  int i;

  fp_tree *new_fp_tree;
  new_fp_tree = (fp_tree *) calloc (1, sizeof (fp_tree));
  new_fp_tree->root = create_fp_node (-1);
  new_fp_tree->freq_item_set =
    create_item_set (freq_item_set->items, freq_item_set->size);

  new_fp_tree->header_table_len = header_table_len;
  new_fp_tree->header_table =
    (fp_node_link_list **) calloc (header_table_len,
				   sizeof (fp_node_link_list *));
  for (i = 0; i < HASH_FUNC_MOD; i++)
    {
      new_fp_tree->header_table[i] = create_fp_node_link_list ();
    }
  return new_fp_tree;
}

void
free_item_set (item_set *ptr)
{
  free (ptr->items);
  free (ptr);
}

void
free_fp_node (fp_node *ptr)
{
  int i;
  if (ptr == NULL)
    return;
  for (i = 0; i < ptr->childs_len; i++)
    {
      if (ptr->childs[i] != NULL)
	free_fp_node (ptr->childs[i]);
    }
  free (ptr->childs);
  free (ptr);
}

void
free_fp_tree (fp_tree *ptr)
{
  int i;
  free_fp_node (ptr->root);
  free_item_set (ptr->freq_item_set);

  for (i = 0; i < HASH_FUNC_MOD; i++)
    free_fp_node_link_list (ptr->header_table[i]);
  free (ptr->header_table);

  if (ptr->order_to_ID != NULL)
    free_heap_ary (ptr->order_to_ID);

  free (ptr);
}

fp_node_link_list *
create_fp_node_link_list (void)
{
  return (fp_node_link_list *) calloc (1, sizeof (fp_node_link_list));
}

void
insert_fp_node_link_list (fp_node_link_list *list, fp_node *node)
{
  fp_node_link_list *new_list;
  new_list = create_fp_node_link_list ();
  new_list->next = list->next;
  new_list->node = node;
  list->next = new_list;
}

void
free_fp_node_link_list (fp_node_link_list *list)
{
  fp_node_link_list *freed_list;
  while (list != NULL)
    {
      freed_list = list;
      list = list->next;
      free (freed_list);
    }
}
