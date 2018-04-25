#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "heap_sort.h"

static void swap_heap_node (heap_node *a, heap_node *b);

heap_ary *
create_heap_ary (int ary_size)
{
  heap_ary *new_heap_ary;
  new_heap_ary = (heap_ary *) calloc (1, sizeof (heap_ary));
  new_heap_ary->ary_len = ary_size;
  new_heap_ary->ary = (heap_node *) calloc (ary_size + 1, sizeof (heap_node));
  return new_heap_ary;
}

void
free_heap_ary (heap_ary *heap_ary_ptr)
{
  free (heap_ary_ptr->ary);
  free (heap_ary_ptr);
}

void
insert_min_heap (heap_ary *heap, heap_node *node)
{
  int index;
  if (heap->latest >= heap->ary_len)
    {
      printf ("array is full\n");
      return;
    }
  heap->latest++;
  memcpy (&heap->ary[heap->latest], node, sizeof (*node));
  index = heap->latest;
  while (index != 1)
    {
      if (heap->ary[index].frequent >= heap->ary[index / 2].frequent)
	break;
      swap_heap_node (&heap->ary[index], &heap->ary[index / 2]);
      index /= 2;
    }

}

static void
swap_heap_node (heap_node *a, heap_node *b)
{
  heap_node tmp;
  memcpy (&tmp, a, sizeof (heap_node));
  tmp = *a;
  memcpy (a, b, sizeof (heap_node));
  memcpy (b, &tmp, sizeof (heap_node));
}

void
pop_min_heap (heap_ary *heap)
{
  if (heap->latest < 1)
    {
      printf ("no any items\n");
      return;
    }
  if (heap->latest > 1)
    swap_heap_node (&heap->ary[heap->latest], &heap->ary[1]);
  int tmp;
  tmp = heap->ary[1].frequent;
  heap->latest--;
  int child = 2;
  int parent = 1;
  while (child <= heap->latest)
    {
      if (child < heap->latest && heap->ary[child].frequent > heap->ary[child + 1].frequent)
	child++;
      if (heap->ary[child].frequent > tmp)
	break;
      swap_heap_node (&heap->ary[parent], &heap->ary[child]);
      parent = child;
      child *= 2;
    }
  heap->ary[parent].frequent = tmp;
}

int
sort_min_heap (heap_ary *heap)
{
  int ary_len;
  int i;
  ary_len = heap->latest;
  for (i = 0; i < ary_len; i++)
    pop_min_heap (heap);
  return ary_len;
}
