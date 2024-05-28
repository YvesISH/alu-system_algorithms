#include "huffman.h"

/**
 * compare_frequencies - program that compares the frequencies
 * of two symbol nodes
 * the purpose of this comparison is to determine the order in which symbols
 * should be arranged or sorted based on their frequencies
 * @p1: a pointer to the first symbol node
 * @p2: a pointer to the second symbol node
 * Return: the difference between the frequencies of the symbols
 */

int compare_frequencies(void *p1, void *p2)
{
	binary_tree_node_t *node1, *node2;
	symbol_t *symbol1, *symbol2;

	node1 = (binary_tree_node_t *)p1;
	node2 = (binary_tree_node_t *)p2;
	symbol1 = (symbol_t *)node1->data;
	symbol2 = (symbol_t *)node2->data;

	return (symbol1->freq - symbol2->freq);
}

/**
 * huffman_priority_queue - program that creates a min-heap priority queue
 * of symbols
 * @data: an array of characters
 * @freq: an array of frequencies associated with characters
 * @size: the size of the arrays
 * Return: a min-heapified version of the arrays
 */

heap_t *huffman_priority_queue(char *data, size_t *freq, size_t size)
{
	heap_t *heap;
	symbol_t *symbol;
	binary_tree_node_t *node;
	size_t i;

	heap = heap_create(compare_frequencies);

	for (i = 0; i < size; i++)
	{
		symbol = symbol_create(data[i], freq[i]);
		node = binary_tree_node(NULL, symbol);
		node = heap_insert(heap, node);
	}
	return (heap);
}
