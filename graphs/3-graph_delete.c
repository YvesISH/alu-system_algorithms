#include "graphs.h"

/**
 * graph_delete - program that frees the memory occupied by a graph
 * and its vertices
 * @graph: a pointer to the graph to be deleted
 */

void graph_delete(graph_t *graph)
{
	vertex_t *current_vertex = NULL, *next_vertex = NULL;
	edge_t *current_edge = NULL, *next_edge = NULL;

	if (!graph)
		return;

	current_vertex = graph->vertices;

	while (current_vertex)
	{
		next_vertex = current_vertex->next;

		if (current_vertex->content)
			free(current_vertex->content);

		current_edge = current_vertex->edges;

		while (current_edge)
		{
			next_edge = current_edge->next;
			free(current_edge);
			current_edge = next_edge;
		}

		free(current_vertex);
		current_vertex = next_vertex;
	}

	free(graph);
}
