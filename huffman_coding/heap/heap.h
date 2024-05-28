#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/**
 * struct binary_tree_node_s - Binary tree node data structure
 * @data: the data stored in a node
 * @left: a pointer to the left child
 * @right: a pointer to the right child
 * @parent: a pointer to the parent node
 */

typedef struct binary_tree_node_s
{
	void *data;
	struct binary_tree_node_s *left;
	struct binary_tree_node_s *right;
	struct binary_tree_node_s *parent;
} binary_tree_node_t;


/**
 * struct heap_s - Heap data structure
 * @size: the size of the heap (number of nodes)
 * @data_cmp: the function to compare two nodes data
 * @root: a pointer to the root node of the heap
 */

typedef struct heap_s
{
	size_t size;
	int (*data_cmp)(void *, void *);
	binary_tree_node_t *root;
} heap_t;


/**
 * struct binary_tree_node_queue_s - Node structure in
 * a binary tree node queue
 * @node: a pointer to the binary tree node
 * @next: a pointer to the next node in the queue
 */

typedef struct binary_tree_node_queue_s
{
	binary_tree_node_t *node;
	struct binary_tree_node_queue_s *next;
} bt_node_queue_t;


/* task 0 */
heap_t *heap_create(int (*data_cmp)(void *, void *));

/* task 1 */
binary_tree_node_t *binary_tree_node(binary_tree_node_t *parent, void *data);

/* task 2 */
bt_node_queue_t *push_bt_node_queue(bt_node_queue_t **queue,
				    binary_tree_node_t *node);
void advance_bt_node_queue(bt_node_queue_t **queue);
void clear_bt_node_queue(bt_node_queue_t **queue);
binary_tree_node_t *complete_binary_tree_insert(binary_tree_node_t *root,
						void *data);
binary_tree_node_t *sift_up_min_heap(binary_tree_node_t *new_node,
				     int (*data_cmp)(void *, void *));
binary_tree_node_t *insert_into_heap(binary_tree_node_t **root,
				     int (*data_cmp)(void *, void *),
				     void *data);
binary_tree_node_t *heap_insert(heap_t *heap, void *data);

/* task 3 */
binary_tree_node_t *swap_nodes(binary_tree_node_t *a, binary_tree_node_t *b);
binary_tree_node_t *n_node(binary_tree_node_t *node, int n);
binary_tree_node_t *restore_heapify(int (*data_cmp)(void *, void *),
				    binary_tree_node_t *node);
void *extract_root(heap_t *heap);
void *heap_extract(heap_t *heap);

/* task 4 */
void recursive_node_free(binary_tree_node_t *node, void (*free_data)(void *));
void heap_delete(heap_t *heap, void (*free_data)(void *));

#endif /* HEAP_H */
