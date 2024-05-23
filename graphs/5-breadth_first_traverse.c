#include "graphs.h"

/**
 * freeQueue - program that frees all the memory allocated for the queue
 * and its nodes
 * this function iterates through each node in the queue, freeing them
 * individually, and finally frees the queue itself;
 * it ensures that all the memory allocated for queue management is
 * properly released
 * @queue: a pointer to the 'GraphQueue' structure to be freed
 * Return: nothing (void)
 */

void freeQueue(GraphQueue *queue)
{
	GraphQueueNode *currentNode, *tempNode;

	currentNode = queue->head;

	while (currentNode)
	{
		tempNode = currentNode;
		currentNode = currentNode->next;
		free(tempNode);
	}
	free(queue);
}

/**
 * pushToQueue - program that adds a new vertex to the queue
 * this function creates a new 'GraphQueueNode', assigns the provided vertex
 * and depth to it, and adds it to the end of the queue;
 * it handles memory allocation for the new node and updates the queue's tail;
 * in case of memory allocation failure, it ensures that the queue is properly
 * freed
 * @queue: a pointer to the GraphQueue where the vertex will be added
 * @vertex: a pointer to the vertex to be added to the queue
 * @depth: the depth level of the vertex in the graph traversal
 * Return: 1 if the vertex is successfully added, 0 if an error occurs
 */

int pushToQueue(GraphQueue *queue, vertex_t *vertex, size_t depth)
{
	GraphQueueNode *newNode;

	newNode = calloc(1, sizeof(GraphQueueNode));

	if (!newNode)
	{
		freeQueue(queue);
		return (0);
	}
	newNode->vertex = vertex;
	newNode->depth = depth;

	if (!queue->tail)
	{
		queue->tail = queue->head = newNode;
	}
	else
	{
		queue->tail->next = newNode;
		queue->tail = newNode;
	}
	queue->size++;
	return (1);
}

/**
 * popFromQueue - program that removes and retrieves a vertex
 * from the front of the queue
 * this function removes the node at the front of the queue, retrieves
 * the vertex and its depth, and then frees the memory of the node;
 * it adjusts the head of the queue and ensures the correct management
 * of the queue's size
 * @queue: a pointer to the 'GraphQueue' from which the vertex will be removed
 * @vertex: a pointer to store the removed vertex
 * @depth: a pointer to store the depth of the removed vertex
 * Return: 1 if the vertex is successfully removed, 0 if the queue is empty
 */

int popFromQueue(GraphQueue *queue, vertex_t **vertex, size_t *depth)
{
	GraphQueueNode *frontNode;

	if (!queue || !queue->head)
		return (0);

	frontNode = queue->head;
	queue->head = queue->head->next;

	if (!queue->head)
		queue->tail = NULL;

	*vertex = frontNode->vertex;
	*depth = frontNode->depth;
	queue->size--;
	free(frontNode);

	return (1);
}

/**
 * processEdges - program that processes all edges connected to
 * a given vertex in a graph
 * this function iterates through all edges of a vertex,
 * checks if the destination vertex of each edge has been visited,
 * and if not, adds it to the queue for further processing;
 * this ensures all connected vertices are traversed
 * @queue: the queue used in the breadth-first traversal
 * @vertex: the current vertex being processed
 * @currentDepth: the depth of the current vertex in the traversal
 * @visited: an array indicating whether a vertex has been visited
 * Return: nothing (void)
 */

void processEdges(GraphQueue *queue, const vertex_t *vertex,
		  size_t currentDepth, size_t *visited)
{
	edge_t *currentEdge = vertex->edges;

	while (currentEdge)
	{
		if (!visited[currentEdge->dest->index])
		{
			visited[currentEdge->dest->index] = 1;
			pushToQueue(queue, currentEdge->dest,
				    currentDepth + 1);
		}
		currentEdge = currentEdge->next;
	}
}

/**
 * breadth_first_traverse - program that performs a breadth-first traversal
 * of a graph
 * this function traverses a graph starting from a given vertex and explores
 * all the neighboring vertices at the present depth before moving on to
 * the vertices at the next depth level;
 * it uses a queue to keep track of vertices to be processed and an array
 * to mark visited vertices;
 * the function applies a provided action to each vertex and tracks the maximum
 * depth reached during traversal
 * @graph: the graph to be traversed
 * @action: a function to be called on each vertex during traversal
 *
 * Return: the maximum depth reached during the traversal
 */

size_t breadth_first_traverse(const graph_t *graph,
			      void (*action)(const vertex_t *v, size_t depth))
{
	GraphQueue *queue;
	vertex_t *currentVertex;
	size_t maxDepth = 0, currentDepth, i, queueSize;
	size_t *visited;

	if (!graph || !action || !graph->nb_vertices)
		return (0);

	visited = calloc(graph->nb_vertices, sizeof(size_t));

	if (!visited)
		return (0);

	queue = calloc(1, sizeof(GraphQueue));

	if (!queue)
	{
		free(visited);
		return (0);
	}

	visited[graph->vertices->index] = 1;
	pushToQueue(queue, graph->vertices, 0);

	while (queue->size)
	{
		queueSize = queue->size;

		for (i = 0; i < queueSize; i++)
		{
			popFromQueue(queue, &currentVertex, &currentDepth);
			action(currentVertex, currentDepth);
			maxDepth = (maxDepth > currentDepth) ? maxDepth : currentDepth;
			processEdges(queue, currentVertex, currentDepth, visited);
		}
	}
	free(visited);
	freeQueue(queue);
	return (maxDepth);
}
