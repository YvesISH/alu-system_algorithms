#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pathfinding.h"
#include "graphs.h"
#include "queues.h"

/* Node structure for the priority queue */
typedef struct pq_node {
    vertex_t *vertex;
    double priority;
    struct pq_node *next;
} pq_node_t;

/* Priority queue structure */
typedef struct {
    pq_node_t *head;
} priority_queue_t;

/* Create a new priority queue */
priority_queue_t *create_priority_queue() {
    priority_queue_t *pq = (priority_queue_t *)malloc(sizeof(priority_queue_t));
    pq->head = NULL;
    return pq;
}

/* Insert a vertex with a given priority into the priority queue */
void pq_insert(priority_queue_t *pq, vertex_t *vertex, double priority) {
    pq_node_t *new_node = (pq_node_t *)malloc(sizeof(pq_node_t));
    pq_node_t *current;

    new_node->vertex = vertex;
    new_node->priority = priority;
    new_node->next = NULL;

    if (!pq->head || pq->head->priority > priority) {
        new_node->next = pq->head;
        pq->head = new_node;
    } else {
        current = pq->head;
        while (current->next && current->next->priority <= priority) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

/* Remove and return the vertex with the highest priority (lowest priority value) */
vertex_t *pq_extract_min(priority_queue_t *pq) {
    pq_node_t *min_node;
    vertex_t *vertex;

    if (!pq->head) return NULL;
    min_node = pq->head;
    vertex = min_node->vertex;
    pq->head = pq->head->next;
    free(min_node);
    return vertex;
}

/* Check if the priority queue is empty */
int pq_is_empty(priority_queue_t *pq) {
    return pq->head == NULL;
}

/* Calculate the Euclidean distance between two vertices */
double heuristic(vertex_t *a, vertex_t *b) {
    return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2));
}

/* Reconstruct the path from start to target */
queue_t *reconstruct_path(vertex_t *start, vertex_t *target, vertex_t **came_from) {
    queue_t *path = create_queue();
    vertex_t *current = (vertex_t *)target;

    while (current != start) {
        enqueue(path, strdup(current->content));
        current = came_from[current->index];
    }
    enqueue(path, strdup(start->content));
    return path;
}

/* A* algorithm to find the shortest path in the graph */
queue_t *a_star_graph(graph_t *graph, vertex_t const *start, vertex_t const *target) {
    priority_queue_t *open_set = create_priority_queue();
    double *g_score;
    double *f_score;
    vertex_t **came_from;
    vertex_t *current;
    int i;

    g_score = (double *)malloc(sizeof(double) * graph->nb_vertices);
    f_score = (double *)malloc(sizeof(double) * graph->nb_vertices);
    came_from = (vertex_t **)malloc(sizeof(vertex_t *) * graph->nb_vertices);

    for (i = 0; i < graph->nb_vertices; i++) {
        g_score[i] = INFINITY;
        f_score[i] = INFINITY;
        came_from[i] = NULL;
    }

    g_score[start->index] = 0;
    f_score[start->index] = heuristic((vertex_t *)start, (vertex_t *)target);
    pq_insert(open_set, (vertex_t *)start, f_score[start->index]);

    while (!pq_is_empty(open_set)) {
        current = pq_extract_min(open_set);
        printf("Checking %s, distance to %s is %.0f\n", current->content, target->content, heuristic(current, (vertex_t *)target));

        if (current == target) {
            queue_t *path = reconstruct_path((vertex_t *)start, (vertex_t *)target, came_from);
            free(g_score);
            free(f_score);
            free(came_from);
            return path;
        }

        edge_t *edge = current->edges;
        while (edge) {
            vertex_t *neighbor = edge->dest;
            double tentative_g_score = g_score[current->index] + edge->weight;
            if (tentative_g_score < g_score[neighbor->index]) {
                came_from[neighbor->index] = current;
                g_score[neighbor->index] = tentative_g_score;
                f_score[neighbor->index] = g_score[neighbor->index] + heuristic(neighbor, (vertex_t *)target);
                pq_insert(open_set, neighbor, f_score[neighbor->index]);
            }
            edge = edge->next;
        }
    }

    free(g_score);
    free(f_score);
    free(came_from);
    return NULL;
}

/* Example usage */
int main(void) {
    graph_t *graph;
    vertex_t *start, *target;
    queue_t *path;

    graph = graph_create();
    graph_add_vertex(graph, "San Francisco", 37, -122);
    graph_add_vertex(graph, "Los Angeles", 34, -118);
    graph_add_vertex(graph, "Las Vegas", 36, -115);
    start = graph_add_vertex(graph, "Seattle", 47, -122);
    graph_add_vertex(graph, "Chicago", 41, -87);
    graph_add_vertex(graph, "Washington", 38, -77);
    graph_add_vertex(graph, "New York", 40, -74);
    graph_add_vertex(graph, "Houston", 29, -95);
    graph_add_vertex(graph, "Nashville", 36, -86);
    target = graph_add_vertex(graph, "Miami", 25, -80);
    graph_add_edge(graph, "San Francisco", "Los Angeles", 347, BIDIRECTIONAL);
    graph_add_edge(graph, "San Francisco", "Las Vegas", 417, BIDIRECTIONAL);
    graph_add_edge(graph, "Los Angeles", "Las Vegas", 228, BIDIRECTIONAL);
    graph_add_edge(graph, "San Francisco", "Seattle", 680, BIDIRECTIONAL);
    graph_add_edge(graph, "Seattle", "Chicago", 1734, BIDIRECTIONAL);
    graph_add_edge(graph, "Chicago", "Washington", 594, BIDIRECTIONAL);
    graph_add_edge(graph, "Washington", "New York", 203, BIDIRECTIONAL);
    graph_add_edge(graph, "Las Vegas", "Houston", 1227, BIDIRECTIONAL);
    graph_add_edge(graph, "Houston", "Nashville", 666, BIDIRECTIONAL);
    graph_add_edge(graph, "Nashville", "Washington", 566, BIDIRECTIONAL);
    graph_add_edge(graph, "Nashville", "Miami", 818, BIDIRECTIONAL);

    path = a_star_graph(graph, start, target);
    if (path) {
        printf("Path found:\n");
        print_free_path(path);
    } else {
        printf("No path found\n");
    }

    graph_delete(graph);
    return 0;
}
