#include <stdio.h>
#include <stdlib.h>
#include "version.h"
#include "heap_sort.h"
#include "linklist.h"
#include "display.h"

int get_transactions (FILE *f, int **ary);
heap_ary *get_sorted_frequent_item_ary (FILE *f);

static char *FILE_NAME;
static int MIN_SUP;
static int item_num;

int
main (int argc, char *argv[])
{
  FILE *f;
  if (argc < 3)
    {
      printf ("Usage: ./fp-growth FILE MIN_SUP\n");
      exit (1);
    }

  MIN_SUP = atoi(argv[2]);
  FILE_NAME = argv[1];

  f = fopen (FILE_NAME, "rb");
  if (f == NULL)
    {
      printf ("Open file error.\nexit...\n");
      exit (1);
    }

  heap_ary *sorted_heap_node;

  sorted_heap_node = get_sorted_frequent_item_ary (f);
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
  fread (&len, sizeof (len), 1, f);	// read len
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
get_sorted_frequent_item_ary (FILE *f)
{
  int *t_ary;
  int i;
  int len = 0;
  int max_item = 0;
  int sorted_heap_ary_len;
  table_list *C1;
  heap_ary *min_heap;
  heap_node *tmp_node;
  
  /* generate C1 */
  C1 = create_tableList ();
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
  print_min_heap (min_heap);
  printf ("%d\n", sorted_heap_ary_len);

  return NULL;
}
