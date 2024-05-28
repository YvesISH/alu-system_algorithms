#include "heap.h"

/**
 * binary_tree_node - program that creates a new binary tree node
 * @parent: a pointer to the parent node of the node to be created
 * @data: the data to be stored in the node
 * Return: a pointer to the created node, or NULL if it fails
 */

binary_tree_node_t *binary_tree_node(binary_tree_node_t *parent, void *data)
{
	binary_tree_node_t *new_node = NULL;

	if (data == NULL)
		return (NULL);

	new_node = malloc(sizeof(binary_tree_node_t));

	if (new_node == NULL)
		return (NULL);

	new_node->data = data;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->parent = parent;

	return (new_node);
}
