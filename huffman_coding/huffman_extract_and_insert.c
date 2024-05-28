#include "huffman.h"

/**
 * huffman_extract_and_insert - program that extracts the two nodes
 * with the lowest frequency from the priority queue, combines them
 * into a new node, and inserts this new node back into the priority queue
 * This function is typically used in building Huffman trees, where two nodes
 * with the lowest frequency are combined to create a new parent node with
 * a frequency equal to the sum of the two, reducing the total number of nodes
 * and thus progressing the construction of the tree
 * @priority_queue: a pointer to the priority queue used to manage the nodes
 *                  in the Huffman tree
 * Return: 1 if the operation is successful, 0 otherwise
 */

int huffman_extract_and_insert(heap_t *priority_queue)
{
	void *ptr1, *ptr2;
	binary_tree_node_t *node, *node1, *node2;
	symbol_t *combined_symbol, *symbol1, *symbol2;

	ptr1 = heap_extract(priority_queue);
	ptr2 = heap_extract(priority_queue);

	if (ptr1 == NULL || ptr2 == NULL)
		return (0);

	node1 = (binary_tree_node_t *)ptr1;
	node2 = (binary_tree_node_t *)ptr2;
	symbol1 = (symbol_t *)node1->data;
	symbol2 = (symbol_t *)node2->data;
	combined_symbol = symbol_create(-1, symbol1->freq + symbol2->freq);

	if (combined_symbol == NULL)
		return (0);

	node = binary_tree_node(NULL, combined_symbol);

	if (node == NULL)
	{
		free(combined_symbol);
		return (0);
	}
	node->left = node1;
	node->right = node2;
	node1->parent = node;
	node2->parent = node;

	if (heap_insert(priority_queue, node) == NULL)
	{
		return (0);
	}

	return (1);
}
