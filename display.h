#include <time.h>
#include "linklist.h"
#include "heap_sort.h"
#include "fp_tree.h"

#ifndef DISPLAY
#define DISPLAY
#define DEBUG_PRINT 1

clock_t begin;
clock_t end;
double spent;
void print_ary (int *ary, int len);
void print_tableList (table_list *tableL);
void calc_time (const char *msg);
void print_intList (int_list *intList);
void print_rgbList (rgb_list *rgbList);
void print_min_heap (heap_ary *heap);
void print_fp_tree (fp_tree *tree);
void print_fp_node (fp_node *node);

#endif /* ifndef DISPLAY */
