#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "version.h"
#include "heap_sort.h"
#include "linklist.h"
#include "display.h"
#include "fp_tree.h"

int get_transactions (FILE *f, int **ary);
heap_ary *get_sorted_frequent_item_ary (table_list *C1);
fp_tree *get_fp_tree (table_list *id_to_order, int order_ary_len);
void fp_growth (fp_tree *tree);

static char *FILE_NAME;
static FILE *OUTPUT_FILE;
static int MIN_SUP;
static int item_num;

int
main (int argc, char *argv[])
{
  int i;
  table_list *id_to_order;
  heap_ary *sorted_heap_ary;
  fp_tree *first_tree;

  begin = clock();

  if (argc < 4)
    {
      printf ("Usage: ./fp-growth FILE.data MIN_SUP OUTPUT_FILE_NAME\n");
      exit (1);
    }

  OUTPUT_FILE = fopen (argv[3], "w");
  MIN_SUP = atoi(argv[2]);
  FILE_NAME = argv[1];

  // generate itemID to sorted order table List
  id_to_order = create_tableList ();
  sorted_heap_ary = get_sorted_frequent_item_ary (id_to_order);
  clear_tableL_val (id_to_order, -1);
  for (i = 0; i < sorted_heap_ary->ary_len; i++)
    set_tableL_val (sorted_heap_ary->ary[i + 1].itemID, id_to_order, i);

  // set fp tree's max child len
  HASH_FUNC_MOD = sorted_heap_ary->ary_len;

  printf ("%d\n", sorted_heap_ary->ary_len);

  first_tree = get_fp_tree (id_to_order, sorted_heap_ary->ary_len);
  first_tree->order_to_ID = sorted_heap_ary;

  item_num = 0;
  fp_growth (first_tree);
  fclose (OUTPUT_FILE);

  calc_time ("finish");
  printf ("frq: %d\n", item_num);

  free_tableList (id_to_order);
  free_fp_tree (first_tree);
  printf ("free fp_tree\n");
  getchar();
  return 0;
}

int
get_transactions (FILE *f, int **ary)
{
  long long int tid;
  int len;
  int read_size;
  if ((read_size = fread (&tid, sizeof (tid), 1, f)) == 0)	// read tid
    if (feof (f))
      return -1;
  if (fread (&len, sizeof (len), 1, f) != 1)	// read len
    {
      printf ("file format error\n");
      exit (1);
    }
  *ary = (int *) malloc (len * sizeof (int));
  read_size = fread (*ary, sizeof (int), len, f);
  if (len != read_size)
    {
      printf ("read error\n");
      exit (1);
    }
  return len;
}

heap_ary *
get_sorted_frequent_item_ary (table_list *C1)
{
  int *t_ary;
  int i;
  int len = 0;
  int max_item = 0;
  int sorted_heap_ary_len;
  heap_ary *min_heap;
  heap_node *tmp_node;

  FILE *f;
  f = fopen (FILE_NAME, "rb");
  if (f == NULL)
    {
      printf ("Open file error.\nexit...\n");
      exit (1);
    }
  
  /* generate C1 */
  while (len != -1)
    {
      len = get_transactions (f, &t_ary);
      for (i = 0; i < len; i++)
	{
	  tableL_insert (t_ary[i], C1, true);
	  if (max_item < t_ary[i])
	    max_item = t_ary[i];
	}
      if (t_ary != NULL && len != -1)
	free (t_ary);
    }

  tmp_node = (heap_node *) alloca (sizeof (heap_node));
  min_heap = create_heap_ary (max_item + 1);

  for (i = 0; i <= max_item; i++)
    {
      tmp_node->itemID = i;
      tmp_node->frequent = get_tableL_val (i, C1);
      if (tmp_node->frequent >= MIN_SUP)
	insert_min_heap (min_heap, tmp_node);
    }
  sorted_heap_ary_len = sort_min_heap (min_heap);
  min_heap->ary_len = sorted_heap_ary_len;

  fclose (f);
  return min_heap;
}

fp_tree *
get_fp_tree (table_list *id_to_order, int order_ary_len)
{
  int *t_ary;
  int len;
  int i;
  int insert_ary_len;
  int order;
  int *insert_ary;
  fp_tree *tree;

  FILE *f;
  f = fopen (FILE_NAME, "rb");
  if (f == NULL)
    {
      printf ("Open file error.\nexit...\n");
      exit (1);
    }

  tree = create_fp_tree (create_item_set (NULL, 0), order_ary_len);
  insert_ary = (int *) alloca (sizeof (int) * order_ary_len);

  len = 0;
  while (len != -1)
    {
      len = get_transactions (f, &t_ary);
      insert_ary_len = 0;
      memset (insert_ary, 0, sizeof (int) * order_ary_len);
      // get used itemID's order
      for (i = 0; i < len; i++)
	{
	  order = get_tableL_val (t_ary[i], id_to_order);
	  if (order != -1)
	    insert_ary[order] = 1;
	}

      // generate insert array
      for (i = 0; i < order_ary_len; i++)
	{
	  if (insert_ary[i])
	    {
	      insert_ary[insert_ary_len] = i;
	      insert_ary_len++;
	    }
	}
      if (insert_ary_len > 0)
	{
	  //print_ary (insert_ary, insert_ary_len);
	  fp_tree_insert (insert_ary, insert_ary_len, tree, 1);
	}

      if (t_ary != NULL && len != -1)
	free (t_ary);
    }
  fclose (f);
  return tree;
}

void
fp_growth (fp_tree *tree)
{
  if (fp_is_empty (tree->root))
    return;
  int i;
  int ary_ind;
  int *id_count_ary;
  int *old_to_new_order;
  int *item_ary;
  int new_item_size;
  fp_node_link_list *cur_list;
  fp_node *cur_node;
  fp_tree *new_tree;
  heap_ary *new_order;
  heap_node *tmp_node;

  new_item_size = tree->freq_item_set->size + 1;
  item_ary = (int *) alloca (new_item_size * sizeof (int));
  id_count_ary = (int *) alloca (sizeof (int) * tree->header_table_len);
  old_to_new_order = (int *) alloca (sizeof (int) * tree->header_table_len);
  tmp_node = (heap_node *) alloca (sizeof (heap_node));
  // bottom up header table of FP tree
  for (i = tree->header_table_len - 1; i >= 0; i--)
    {
      if (tree->header_table[i]->next == NULL)
	continue;

      // calculate new freq
      // scan header table's list
      memset (id_count_ary, 0, sizeof (int) * tree->header_table_len);
      cur_list = tree->header_table[i]->next;
      while (cur_list != NULL)
	{
	  //scan node and count
	  cur_node = cur_list->node;
	  while (cur_node != tree->root)
	    {
	      id_count_ary[cur_node->index] += cur_list->node->count;
	      cur_node = cur_node->parent;
	    }
	  cur_list = cur_list->next;
	}
      // id_count_ary, index is this tree's freq order, value is new freq
      // sort new freq and old order pair
      cur_node = tree->header_table[i]->next->node;
      new_order = create_heap_ary (i);
      for (ary_ind = 0; ary_ind < i; ary_ind++)
	{
	  if (id_count_ary[ary_ind] < MIN_SUP || ary_ind == cur_node->index)
	    continue;
	  //tmp_node->itemID = tree->order_to_ID->ary[ary_ind + 1].itemID;
	  tmp_node->itemID = ary_ind;
	  tmp_node->frequent = id_count_ary[ary_ind];
	  insert_min_heap (new_order, tmp_node);
	}

      // record item: frq
      for (ary_ind = 0; ary_ind < tree->freq_item_set->size; ary_ind++)
	{
	  fprintf (OUTPUT_FILE, "%d, ", tree->freq_item_set->items[ary_ind]);
	}
      fprintf (OUTPUT_FILE, "%d: %d\n",
	       tree->order_to_ID->ary[cur_node->index + 1].itemID,
	       id_count_ary[cur_node->index]);

      item_num++;

      new_order->ary_len = sort_min_heap (new_order);
      if (new_order->ary_len == 0)
	{
	  // no frequent item >= MIN_SUP
	  free_heap_ary (new_order);
	  continue;
	}

      // get old_order to new_order array
      heap_node *tmp;
      memset (old_to_new_order, -1, sizeof (int) * tree->header_table_len);
      for (ary_ind = 0; ary_ind < new_order->ary_len; ary_ind++)
	{
	  tmp = &new_order->ary[ary_ind + 1];
	  old_to_new_order[tmp->itemID] = ary_ind;
	  // parse new_order->ary's itemID to real itemID
	  tmp->itemID = tree->order_to_ID->ary[tmp->itemID + 1].itemID;
	}

      // generate new tree
      // get new tree's frequent item set
      memcpy (item_ary, tree->freq_item_set->items, (new_item_size - 1) * sizeof (int));
      item_ary[new_item_size - 1] = tree->order_to_ID->ary[cur_node->index + 1].itemID;

      new_tree = create_fp_tree (create_item_set (item_ary, new_item_size),
				 new_order->ary_len);
      new_tree->order_to_ID = new_order;

      // insert to new tree
      int *transaction_ary;
      int transaction_len;
      int count;
      transaction_ary = id_count_ary;
      cur_list = tree->header_table[i]->next;
      while (cur_list != NULL)
	{
	  // get trasaction
	  memset (transaction_ary, 0, i * sizeof (int));
	  cur_node = cur_list->node;
	  count = cur_node->count;
	  cur_node = cur_node->parent;
	  while (cur_node != tree->root)
	    {
	      ary_ind = old_to_new_order[cur_node->index];
	      if (ary_ind != -1)
		{
		  // record used item
		  transaction_ary[ary_ind] = 1;
		}
	      cur_node = cur_node->parent;
	    }
	  transaction_len = 0;
	  for (ary_ind = 0; ary_ind < i; ary_ind++)
	    {
	      // set transaction array
	      if (transaction_ary[ary_ind])
		{
		  transaction_ary[transaction_len] = ary_ind;
		  transaction_len++;
		}
	    }
	  if (transaction_len)
	    {
	      fp_tree_insert (transaction_ary, transaction_len,
			      new_tree, count);
	    }
	  cur_list = cur_list->next;
	}
      fp_growth (new_tree);
      free_fp_tree (new_tree);
    }
}
