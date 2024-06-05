#include "pathfinding.h"

/**
 * get_min_g_score - finds the vertex with lowest g_score from source
 *
 * @graph: pointer to graph
 * @g_score: array of g_scores from start vertex
 * @visited: array of if vertex has been visited
 * @index: current index
 *
 * Return: index with minimum g_score or NULL
 */
vertex_t *get_min_g_score(graph_t *graph, size_t *g_score, size_t *visited,
		size_t *index)
{
	size_t min = ULONG_MAX;
	size_t i;
	vertex_t *vertex;

	vertex = graph->vertices;
	if (vertex == NULL)
		return (NULL);

	*index = ULONG_MAX;

	for (i = 0; i < graph->nb_vertices; i++)
	{
		if (visited[i] == 0 && min > g_score[i])
		{
			min = g_score[i];
			*index = i;
		}
	}
	if (*index == ULONG_MAX)
		return (NULL);

	while (vertex->next)
	{
		if (vertex->index == *index)
			return (vertex);
		vertex = vertex->next;
	}
	return (vertex);
}


/**
 * reconstruct_path - inserts vertices into queue
 *
 * @graph: pointer to graph with vertices
 * @path: pointer to path array
 * @came_from: pointer to came_from node
 * @start: start vertex
 * @target: target vertex
 *
 * Return: void
 */
void reconstruct_path(graph_t *graph, queue_t *path, vertex_t **came_from,
			vertex_t const *start, vertex_t const *target)
{
	size_t i = target->index;

	if (!came_from[i])
		return;

	/* Push into queue starting at end (target node) */
	if (!queue_push_front(path, strdup(target->content)))
		queue_delete(path);

	while (came_from[i] && i < graph->nb_vertices)
	{
		if (!queue_push_front(path, strdup(came_from[i]->content)))
			queue_delete(path);
		i = came_from[i]->index;
		if (i == start->index)
			return;
	}
}


/**
 * recursive_a_star - recursive utility to find shortest path using A-star
 *
 * @graph: pointer to graph to go through
 * @g_score: array of g_scores from start vertex
 * @visited: keeps track of which vertices have been visited
 * @came_from: keeps track of came_from nodes for each vertex
 * @start: pointer to starting vertex
 * @target: pointer to target vertex
 * @f_score: array of distance from start + euclidean to target
 * @idx: pointer to current index
 *
 * Return: queue of shortest path or NULL
 */
void recursive_a_star(graph_t *graph, size_t *visited, vertex_t **came_from,
	vertex_t const *start, vertex_t const *target,
	size_t *f_score, size_t *g_score, size_t idx)
{
	vertex_t *current, *neighbor;
	edge_t *edge;
	size_t i = 0, tentative_g_score;

	current = get_min_g_score(graph, f_score, visited, &idx);
	if (!current)
		return;

	f_score[current->index] = h(current->x, current->y, target->x, target->y);
	printf("Checking %s, distance to %s is %ld\n", current->content,
			target->content, f_score[current->index]);
	i = current->index;
	edge = current->edges;
	while (edge && visited[i] == 0)
	{
		neighbor = edge->dest;
		tentative_g_score = g_score[i] + edge->weight;
		if (g_score[neighbor->index] > tentative_g_score)
		{
			came_from[neighbor->index] = current;
			g_score[neighbor->index] = tentative_g_score;
			f_score[neighbor->index] = g_score[neighbor->index] +
				h(neighbor->x, neighbor->y, target->x, target->y);
		}
		edge = edge->next;
	}
	visited[i] = 1;
	if (visited[target->index] == 1)
		return;

	recursive_a_star(graph, visited, came_from, start, target,
		f_score, g_score, idx);
}


/**
 * initialize_a_star - initializes all the arrays for A* search
 *
 * @graph: graph that we are examining
 * @visited: has this vertex been visited?
 * @came_from: array of parents of visited nodes
 * @f_score: array of g_score + h(current, target)
 * @g_score: distance from start to current
 * @start: starting vertex
 * @target: target vertex
 *
 * Return: 1 on success, -1 on failure
 */
int initialize_a_star(graph_t *graph, size_t **visited, vertex_t ***came_from,
	size_t **f_score, size_t **g_score,
	const vertex_t *start, const vertex_t *target)
{
	size_t i;

	*visited = calloc(graph->nb_vertices, sizeof(**visited));
	if (!*visited)
		return (-1);
	*came_from = calloc(graph->nb_vertices, sizeof(***came_from));
	if (!*came_from)
	{
		free(*visited);
		return (-1);
	}
	*f_score = malloc(graph->nb_vertices * sizeof(**f_score));
	if (!*f_score)
	{
		free(*visited), free(*came_from);
		return (-1);
	}
	*g_score = malloc(graph->nb_vertices * sizeof(**g_score));
	if (!*g_score)
	{
		free(*visited), free(*came_from), free(*f_score);
		return (-1);
	}
	for (i = 0; i < graph->nb_vertices; i++)
	{
		(*f_score)[i] = ULONG_MAX;
		(*g_score)[i] = ULONG_MAX;
	}
	(*g_score)[start->index] = 0;
	(*f_score)[start->index] = h(start->x, start->y,
							target->x, target->y);
	return (1);
}

/**
 * a_star_graph - searches for the shortest path from a starting point to a
 *	target point in a graph using A* algorithm using Euclidean g_score as
 *  the heuristic
 *
 * @graph: pointer to graph to go through
 * @start: pointer to starting vertex
 * @target: pointer to target vertex
 *
 * Return: queue in which each node is a char * corresponding to a vertex
 *	forming a path from start to target
 */
queue_t *a_star_graph(graph_t *graph, vertex_t const *start,
					vertex_t const *target)
{
	size_t *f_score, *g_score, *visited = 0;
	queue_t *queue = NULL;
	vertex_t **came_from = NULL;

	if (!graph || !start || !target)
		return (NULL);

	if (!initialize_a_star(graph, &visited, &came_from, &f_score, &g_score,
			start, target))
		return (NULL);

	queue = queue_create();

	recursive_a_star(graph, visited, came_from, start, target,
		f_score, g_score, 0);
	reconstruct_path(graph, queue, came_from, start, target);

	free(visited), free(f_score), free(g_score), free(came_from);

	if (!queue->front)
	{
		free(queue);
		return (NULL);
	}
	return (queue);
}
