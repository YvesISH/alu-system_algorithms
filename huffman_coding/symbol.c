#include "huffman.h"

/**
 * symbol_create - program that creates a symbol_t data structure
 * @data: the data to be stored in the structure
 * @freq: the associated frequency
 * Return: a pointer to the created symbol_t structure,
 *         or NULL if allocation fails
 */

symbol_t *symbol_create(char data, size_t freq)
{
	symbol_t *symbol = (symbol_t *)malloc(sizeof(symbol_t));

	if (symbol == NULL)
	{
		return (NULL);
	}
	symbol->data = data;
	symbol->freq = freq;

	return (symbol);
}
