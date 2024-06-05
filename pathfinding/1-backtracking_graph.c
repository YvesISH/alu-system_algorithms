#include "pathfinding.h"

/**
 * graph_fill - program that explores the graph recursively
 * @new_node: the queue to store the path from start to target vertex
 * @saw: the array indicating whether a vertex has been visited
 *       (1 if visited, 0 otherwise)
 * @current: the current vertex being explored
 * @target: the content of the target vertex we are searching for
 * Return: a node if the target is found and added to the queue,
 *         otherwise NULL
 */

queue_node_t *graph_fill(queue_t *new_node, int *saw, const vertex_t *current,
			 char *target)
{
	edge_t *e;

	if (current == NULL || saw[current->index] == 1)
		return (NULL);

	printf("Checking %s\n", current->content);

	if (!strcmp(current->content, target))
		return (queue_push_front(new_node,
					 strdup(current->content)));

	saw[current->index] = 1;

	for (e = current->edges; e != NULL; e = e->next)
		if (graph_fill(new_node, saw, e->dest, target))
			return (queue_push_front(new_node,
						 strdup(current->content)));
	return (NULL);
}

/**
 * backtracking_graph - program that performs backtracking
 * @graph: a pointer to the graph structure where the search is performed
 * @start: a pointer to the starting vertex for the pathfinding
 * @target: a pointer to the target vertex to find
 * Return: a queue containing the path from start to target,
 *         or NULL if no path exists
 */

queue_t *backtracking_graph(graph_t *graph, vertex_t const *start,
			    vertex_t const *target)
{
	queue_t *new_node;
	int *saw;

	if (!graph || !start || !target)
		return (NULL);

	new_node = queue_create();

	if (new_node == NULL)
		return (NULL);

	saw = calloc(graph->nb_vertices, sizeof(*saw));

	if (saw == NULL)
		return (NULL);

	if (graph_fill(new_node, saw, start, target->content) == NULL)
	{
		queue_delete(new_node);
		new_node = NULL;
	}
	free(saw);

	return (new_node);
}
