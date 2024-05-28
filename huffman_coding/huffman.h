#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdlib.h>
#include <stddef.h>
#include "heap/heap.h"

/**
 * struct symbol_s - Structure that stores a char
 * and its associated frequency
 * @data: The character
 * @freq: The associated frequency
 */

typedef struct symbol_s
{
	char data;
	size_t freq;
} symbol_t;



/* task 5 */
symbol_t *symbol_create(char data, size_t freq);


/* task 6 */
int compare_frequencies(void *p1, void *p2);
heap_t *huffman_priority_queue(char *data, size_t *freq, size_t size);


/* task 7 */
int huffman_extract_and_insert(heap_t *priority_queue);


/* task 8 */
void freeNestedNode(void *data);
binary_tree_node_t *huffman_tree(char *data, size_t *freq, size_t size);

/* task 9 */
int huffman_codes(char *data, size_t *freq, size_t size);
void free_huffman_tree(binary_tree_node_t *root);
size_t calculate_tree_depth(binary_tree_node_t *root, size_t depth);
void print_huffman_codes_recursive(binary_tree_node_t *root, char *code,
				   size_t depth);

#endif /* HUFFMAN_H */
