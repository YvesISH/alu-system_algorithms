#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pathfinding.h"

// Vertex structure representing a node in the graph
typedef struct vertex {
    char *name;
    int x, y;
} vertex_t;

// Graph structure containing vertices and edges
typedef struct graph {
    // Add your graph implementation details here
} graph_t;

// Node structure for the priority queue
typedef struct pq_node {
    vertex_t *vertex;
    double priority;
    struct pq_node *next;
} pq_node_t;

// Priority queue structure
typedef struct {
    pq_node_t *head;
} priority_queue_t;

// Create a new priority queue
priority_queue_t *create_priority_queue() {
    priority_queue_t *pq = (priority_queue_t *)malloc(sizeof(priority_queue_t));
    pq->head = NULL;
    return pq;
}

// Insert a vertex with a given priority into the priority queue
void pq_insert(priority_queue_t *pq, vertex_t *vertex, double priority) {
    pq_node_t *new_node = (pq_node_t *)malloc(sizeof(pq_node_t));
    new_node->vertex = vertex;
    new_node->priority = priority;
    new_node->next = NULL;

    if (!pq->head || pq->head->priority > priority) {
        new_node->next = pq->head;
        pq->head = new_node;
    } else {
        pq_node_t *current = pq->head;
        while (current->next && current->next->priority <= priority) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

// Remove and return the vertex with the highest priority (lowest priority value)
vertex_t *pq_extract_min(priority_queue_t *pq) {
    if (!pq->head) return NULL;
    pq_node_t *min_node = pq->head;
    vertex_t *vertex = min_node->vertex;
    pq->head = pq->head->next;
    free(min_node);
    return vertex;
}

// Check if the priority queue is empty
int pq_is_empty(priority_queue_t *pq) {
    return pq->head == NULL;
}

// Calculate the Euclidean distance between two vertices
double heuristic(vertex_t *a, vertex_t *b) {
    return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2));
}

// Reconstruct the path from start to target
queue_t *reconstruct_path(vertex_t *start, vertex_t *target, vertex_t **came_from) {
    queue_t *path = create_queue();
    vertex_t *current = target;
    while (current != start) {
        enqueue(path, strdup(current->name));
        current = came_from[current - start];
    }
    enqueue(path, strdup(start->name));
    return path;
}

// A* algorithm to find the shortest path in the graph
queue_t *a_star_graph(graph_t *graph, vertex_t const *start, vertex_t const *target) {
    priority_queue_t *open_set = create_priority_queue();
    pq_insert(open_set, (vertex_t *)start, 0);

    // Maps to store the cost from start and the estimated cost to the target
    double *g_score = (double *)malloc(sizeof(double) * graph->vertex_count);
    double *f_score = (double *)malloc(sizeof(double) * graph->vertex_count);
    vertex_t **came_from = (vertex_t **)malloc(sizeof(vertex_t *) * graph->vertex_count);

    for (int i = 0; i < graph->vertex_count; i++) {
        g_score[i] = INFINITY;
        f_score[i] = INFINITY;
        came_from[i] = NULL;
    }

    g_score[start - graph->vertices] = 0;
    f_score[start - graph->vertices] = heuristic((vertex_t *)start, (vertex_t *)target);

    while (!pq_is_empty(open_set)) {
        vertex_t *current = pq_extract_min(open_set);
        printf("Checking %s, distance to %s is %.0f\n", current->name, target->name, heuristic(current, (vertex_t *)target));

        if (current == target) {
            queue_t *path = reconstruct_path((vertex_t *)start, (vertex_t *)target, came_from);
            free(g_score);
            free(f_score);
            free(came_from);
            return path;
        }

        // Iterate over neighbors of the current vertex
        // for each neighbor of current:
        // {
        //     double tentative_g_score = g_score[current] + distance(current, neighbor);
        //     if (tentative_g_score < g_score[neighbor]) {
        //         came_from[neighbor] = current;
        //         g_score[neighbor] = tentative_g_score;
        //         f_score[neighbor] = g_score[neighbor] + heuristic(neighbor, target);
        //         pq_insert(open_set, neighbor, f_score[neighbor]);
        //     }
        // }
    }

    free(g_score);
    free(f_score);
    free(came_from);
    return NULL;
}

// Example usage
int main(void) {
    // Initialize the graph and vertices here
    graph_t *graph = graph_create();
    // Add vertices and edges to the graph

    vertex_t *start = // set start vertex;
    vertex_t *target = // set target vertex;

    queue_t *path = a_star_graph(graph, start, target);
    if (path) {
        printf("Path found:\n");
        print_free_path(path);
    } else {
        printf("No path found\n");
    }

    graph_delete(graph);
    return 0;
}

