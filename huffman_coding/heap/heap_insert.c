#include "heap.h"

/**
 * push_bt_node_queue - program that pushes a binary tree node
 * into a node queue
 * @queue: a pointer to the queue
 * @node: a binary tree node to be pushed
 * Return: a pointer to the newly created node in the queue
 */

bt_node_queue_t *push_bt_node_queue(bt_node_queue_t **queue,
				    binary_tree_node_t *node)
{
	bt_node_queue_t *temp, *new_node = NULL;

	if (!queue || !node)
		return (NULL);

	new_node = malloc(sizeof(bt_node_queue_t));

	if (!new_node)
		return (NULL);

	new_node->node = node;
	new_node->next = NULL;

	if (!*queue)
		*queue = new_node;
	else
	{
		temp = *queue;

		while (temp && temp->next)
			temp = temp->next;

		temp->next = new_node;
	}

	return (new_node);
}


/**
 * advance_bt_node_queue - program that advances the node queue by removing
 * the front element
 * @queue: a pointer to the queue
 * Return: nothing (void)
 */

void advance_bt_node_queue(bt_node_queue_t **queue)
{
	bt_node_queue_t *temp;

	if (!queue || !*queue)
		return;

	temp = *queue;
	*queue = (*queue)->next;

	free(temp);
}



/**
 * clear_bt_node_queue - program that clears the entire node queue
 * by removing all elements
 * @queue: a pointer to the queue
 * Return: nothing (void)
 */

void clear_bt_node_queue(bt_node_queue_t **queue)
{
	if (!queue)
		return;

	while (*queue)
		advance_bt_node_queue(queue);
}


/**
 * complete_binary_tree_insert - program that inserts a new node
 * into a binary tree in a complete manner
 * @root: the root of the binary tree
 * @data: the data to be stored in the new node
 * Return: a pointer to the newly inserted node
 */

binary_tree_node_t *complete_binary_tree_insert(binary_tree_node_t *root,
						void *data)
{
	binary_tree_node_t *new_node = NULL;
	bt_node_queue_t *queue = NULL;

	if (!root)
		return (binary_tree_node(NULL, data));

	queue = push_bt_node_queue(&queue, root);

	if (!queue)
		return (NULL);

	while (queue)
	{
		if (queue->node->left && queue->node->right)
		{
			if (!push_bt_node_queue(&queue, queue->node->left) ||
			    !push_bt_node_queue(&queue, queue->node->right))
			{
				clear_bt_node_queue(&queue);
				return (NULL);
			}
			advance_bt_node_queue(&queue);
		}
		else
		{
			new_node = binary_tree_node(queue->node, data);

			if (!new_node)
				return (NULL);

			if (!queue->node->left)
				queue->node->left = new_node;
			else
				queue->node->right = new_node;

			clear_bt_node_queue(&queue);
		}
	}
	return (new_node);
}

/**
 * sift_up_min_heap - program that adjusts the position of
 * a newly inserted node in a min-heap by moving it up the heap
 * as long as necessary
 * @new_node: a pointer to the newly inserted node
 * @data_cmp: a function pointer for data comparison
 * Return: a pointer to the final position of the adjusted node
 */

binary_tree_node_t *sift_up_min_heap(binary_tree_node_t *new_node,
				     int (*data_cmp)(void *, void *))
{
	binary_tree_node_t *temp = NULL;
	void *swap;

	if (!data_cmp)
		return (new_node);

	temp = new_node;

	while (temp && temp->parent &&
	       data_cmp(temp->data, temp->parent->data) < 0)
	{
		swap = temp->data;
		temp->data = temp->parent->data;
		temp->parent->data = swap;
		temp = temp->parent;
	}
	return (temp);
}

/**
 * insert_into_heap - program that inserts a new node into a heap
 * and performs min-heap adjustments
 * @root: a pointer to the root of the heap
 * @data_cmp: a function pointer for data comparison
 * @data: the data to be stored in the new node
 * Return: a pointer to the newly inserted node
 */

binary_tree_node_t *insert_into_heap(binary_tree_node_t **root,
				     int (*data_cmp)(void *, void *),
				     void *data)
{
	binary_tree_node_t *new_node = NULL;

	if (!root || !data_cmp || !data)
		return (NULL);

	new_node = complete_binary_tree_insert(*root, data);

	if (!new_node)
		return (NULL);
	if (!*root)
		*root = new_node;

	new_node = sift_up_min_heap(new_node, data_cmp);

	return (new_node);
}

/**
 * heap_insert - program that inserts a new node with data into a heap
 * and updates heap properties
 * @heap: a pointer to the heap structure
 * @data: the data to be stored in the new node
 * Return: a pointer to the newly inserted node
 */

binary_tree_node_t *heap_insert(heap_t *heap, void *data)
{
	binary_tree_node_t *new_node = NULL;

	if (!heap || !data)
		return (NULL);

	new_node = insert_into_heap(&(heap->root), heap->data_cmp, data);

	if (new_node)
		heap->size++;

	return (new_node);
}
