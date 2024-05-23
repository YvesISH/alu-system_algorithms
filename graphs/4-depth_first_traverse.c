#include "graphs.h"

/**
 * traverseVerticesRecursively - recursive function that travels through
 * vertices in a graph
 * @vertex: the starting vertex to traverse from
 * @visited_flags: an array indicating whether vertices have been visited
 * @current_depth: the current depth level in traversal
 * @vertex_action: a function to perform an action on each vertex
 * Return: the maximum depth reached during traversal
 */

size_t traverseVerticesRecursively(vertex_t *vertex, char *visited_flags,
				   size_t current_depth, void (*vertex_action)
				   (const vertex_t *v, size_t depth))
{
	size_t current_depth_new = 0, max_depth = current_depth, flag = 0;
	edge_t *edge_header = NULL;

	if (!vertex)
		return (current_depth - 1);

	edge_header = vertex->edges;

	while (edge_header)
	{
		if (visited_flags[edge_header->dest->index] == 0)
		{
			vertex_action(edge_header->dest, current_depth);
			visited_flags[edge_header->dest->index] = 1;
			current_depth_new = traverseVerticesRecursively
				(edge_header->dest, visited_flags,
				 current_depth + 1, vertex_action);

			if (current_depth_new > max_depth)
				max_depth = current_depth_new;
			flag = 1;
		}
		edge_header = edge_header->next;
	}
	if (flag)
		return (max_depth);

	return (max_depth - 1);
}



/**
 * depth_first_traverse - program that traverses a graph using
 * depth-first search
 * @graph: the graph to traverse
 * @action: a function to perform an action on each vertex along with its depth
 * Return: the maximum depth reached during traversal
 */

size_t depth_first_traverse(const graph_t *graph,
			    void (*action)(const vertex_t *v, size_t depth))
{
	vertex_t *current_vertex = NULL;
	char visited_flags[2048] = {0};
	size_t current_depth = 0, max_depth = 0;

	if (!graph || !action)
		return (0);

	current_vertex = graph->vertices;

	while (current_vertex)
	{
		if (visited_flags[current_vertex->index] == 0)
		{
			action(current_vertex, 0);
			visited_flags[current_vertex->index] = 1;
			current_depth = traverseVerticesRecursively
				(current_vertex, visited_flags, 1, action);

			if (current_depth > max_depth)
				max_depth = current_depth;
		}
		current_vertex = current_vertex->next;
		break;
	}
	return (max_depth);
}
