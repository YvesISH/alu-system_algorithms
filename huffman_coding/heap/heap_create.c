#include "heap.h"

/**
 * heap_create - program that creates a new heap data structure
 * @data_cmp: a pointer to the comparison function for node data
 * Return: a pointer to the created heap_t structure, or NULL if it fails
 */

heap_t *heap_create(int (*data_cmp)(void *, void *))
{
	heap_t *heap = NULL;

	if (data_cmp == NULL)
		return (NULL);

	heap = malloc(sizeof(heap_t));

	if (heap == NULL)
		return (NULL);

	heap->size = 0;
	heap->data_cmp = data_cmp;
	heap->root = NULL;

	return (heap);
}
