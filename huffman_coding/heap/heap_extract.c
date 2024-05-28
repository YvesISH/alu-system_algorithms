#include "heap.h"

/**
 * swap_nodes - program that swaps the data and pointers
 * of two binary tree nodes
 * @a: the first node
 * @b: the second node
 * Return: the node that was originally 'b' after swapping
 */

binary_tree_node_t *swap_nodes(binary_tree_node_t *a, binary_tree_node_t *b)
{
	void *tmp;

	tmp = b->data;
	b->data = a->data;
	a->data = tmp;
	a = b;

	return (a);
}

/**
 * n_node - program that finds the n-th node in a binary tree,
 * following a specific path
 * @node: the starting node
 * @n: the index of the desired node
 * Return: the n-th node, or NULL if not found
 */

binary_tree_node_t *n_node(binary_tree_node_t *node, int n)
{
	int bit_idx, mask;

	if (!node || n < 0)
		return (NULL);

	for (bit_idx = 0; 1 << (bit_idx + 1) <=  n; ++bit_idx)
		;
	for (--bit_idx; bit_idx >= 0; --bit_idx)
	{
		mask = 1 << bit_idx;
		if (n & mask)
		{
			if (node->right)
				node = node->right;
			else
				break;
		}
		else
		{
			if (node->left)
				node = node->left;
			else
				break;
		}
	}
	return (node);
}

/**
 * restore_heapify - program that restores the heap property
 * by swapping nodes as needed
 * @data_cmp: the function for comparing data
 * @node: the starting node to restore the heap property from
 * Return: the node after restoring the heap property
 */

binary_tree_node_t *restore_heapify(int (*data_cmp)(void *, void *),
				    binary_tree_node_t *node)
{
	binary_tree_node_t *lg;

	if (!data_cmp || !node)
		return (NULL);
	do {
		lg = NULL;

		if (node->left && data_cmp(node->data, node->left->data) >= 0)
			lg = node->left;

		if (node->right && data_cmp(node->data, node->right->data) >= 0 &&
		    (!lg || data_cmp(lg->data, node->right->data) > 0))
			lg = node->right;

		if (lg)
			node = swap_nodes(node, lg);
	} while (lg);

	return (node);
}

/**
 * extract_root - program that extracts the root element from the heap,
 * rearranges the heap to maintain the heap property,
 * and reduces the heap size
 * @heap: the heap structure from which to extract the root
 * Return: the data of the extracted root element
 */

void *extract_root(heap_t *heap)
{
	binary_tree_node_t *node;
	void *data;

	node = n_node(heap->root, heap->size);

	if (node->parent)
	{
		data = swap_nodes(heap->root, node)->data;

		if (node->parent->left == node)
			node->parent->left = NULL;
		else
			node->parent->right = NULL;
	}
	else
	{
		data = heap->root->data;
		heap->root = NULL;
	}
	free(node);

	if (--heap->size == 0)
		heap->root = NULL;

	return (data);
}

/**
 * heap_extract - program that extracts the root element from the heap
 * and restores heap property using the provided comparison function
 * @heap: the heap structure from which to extract and restore the heap
 * Return: the data of the extracted root element
 */

void *heap_extract(heap_t *heap)
{
	void *data;

	if (!heap)
		return (NULL);

	data = extract_root(heap);
	restore_heapify(heap->data_cmp, heap->root);

	return (data);
}
