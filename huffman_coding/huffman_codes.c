#include "huffman.h"

/**
 * huffman_codes - program that generates Huffman codes for characters based on
 * their frequencies and prints them
 * @data: an array of characters
 * @freq: an array of frequencies associated with characters
 * @size: the size of the arrays
 * Return: 1 on success, 0 on failure
 */

int huffman_codes(char *data, size_t *freq, size_t size)
{
	binary_tree_node_t *huffman_root;
	char *code;
	size_t depth, i;

	huffman_root = huffman_tree(data, freq, size);

	if (huffman_root == NULL)
		return (0);

	depth = calculate_tree_depth(huffman_root, 0);
	code = malloc(sizeof(char) * (depth + 1));

	for (i = 0; i < depth + 1; i++)
		code[i] = '\0';

	print_huffman_codes_recursive(huffman_root, code, 0);

	free(code);
	free_huffman_tree(huffman_root);

	return (1);
}

/**
 * free_huffman_tree - program that recursively frees the memory allocated
 * for the Huffman tree
 * @root: the root node of the Huffman tree
 * Return: nothing (void)
 */

void free_huffman_tree(binary_tree_node_t *root)
{
	if (root->left)
		free_huffman_tree(root->left);

	if (root->right)
		free_huffman_tree(root->right);

	free(root->data);
	free(root);
}

/**
 * calculate_tree_depth - program that calculates the depth of a binary tree
 * @root: the root node of the binary tree
 * @depth: the current depth of the node
 * Return: the depth of the tree
 */

size_t calculate_tree_depth(binary_tree_node_t *root, size_t depth)
{
	int left_depth, right_depth;

	left_depth = right_depth = -1;

	if (root->left)
		left_depth = calculate_tree_depth(root->left, depth + 1);
	if (root->right)
		right_depth = calculate_tree_depth(root->right, depth + 1);
	else
		return (depth);

	if (left_depth > right_depth)
		return (left_depth);

	return (right_depth);
}

/**
 * print_huffman_codes_recursive - program that recursively generates
 * and prints Huffman codes for characters
 * @root: the current node in the Huffman tree
 * @code: the current code generated during traversal
 * @depth: the current depth of the node
 * Return: nothing (void)
 */

void print_huffman_codes_recursive(binary_tree_node_t *root, char *code,
				   size_t depth)
{
	symbol_t *symbol = (symbol_t *)root->data;

	if (root->left)
	{
		code[depth] = '0';
		print_huffman_codes_recursive(root->left, code, depth + 1);
	}
	if (root->right)
	{
		code[depth] = '1';
		print_huffman_codes_recursive(root->right, code, depth + 1);
	}
	else
	{
		printf("%c: %s\n", symbol->data, code);
	}
}
