#include <stdio.h>
#include <stdlib.h>

// Struct for a point in the map
typedef struct point {
    int x;
    int y;
} point_t;

// Node for the queue
typedef struct queue_node {
    point_t pt;
    struct queue_node *next;
} queue_node_t;

// Queue structure
typedef struct queue {
    queue_node_t *front;
    queue_node_t *rear;
} queue_t;

// Create a new queue
queue_t *create_queue() {
    queue_t *q = (queue_t *)malloc(sizeof(queue_t));
    q->front = q->rear = NULL;
    return q;
}

// Enqueue a point to the queue
void enqueue(queue_t *q, point_t pt) {
    queue_node_t *new_node = (queue_node_t *)malloc(sizeof(queue_node_t));
    new_node->pt = pt;
    new_node->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = new_node;
        return;
    }
    q->rear->next = new_node;
    q->rear = new_node;
}

// Check if the queue is empty
int is_empty(queue_t *q) {
    return q->front == NULL;
}

// Dequeue a point from the queue
point_t dequeue(queue_t *q) {
    if (is_empty(q)) {
        point_t pt = {-1, -1};
        return pt;
    }
    queue_node_t *temp = q->front;
    point_t pt = temp->pt;
    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;
    free(temp);
    return pt;
}

// Print the queue
void print_queue(queue_t *q) {
    queue_node_t *node = q->front;
    while (node != NULL) {
        printf("(%d, %d)\n", node->pt.x, node->pt.y);
        node = node->next;
    }
}

// Recursive backtracking function
int backtrack(char **map, int rows, int cols, point_t start, point_t target, queue_t *path, int **visited) {
    if (start.x < 0 || start.x >= rows || start.y < 0 || start.y >= cols || map[start.x][start.y] == '1' || visited[start.x][start.y])
        return 0;

    printf("Visiting cell: (%d, %d)\n", start.x, start.y);
    if (start.x == target.x && start.y == target.y) {
        enqueue(path, start);
        return 1;
    }

    visited[start.x][start.y] = 1;

    point_t directions[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    for (int i = 0; i < 4; i++) {
        point_t next = {start.x + directions[i].x, start.y + directions[i].y};
        if (backtrack(map, rows, cols, next, target, path, visited)) {
            enqueue(path, start);
            return 1;
        }
    }

    visited[start.x][start.y] = 0;
    return 0;
}

// Main function to find the path
queue_t *backtracking_array(char **map, int rows, int cols, point_t const *start, point_t const *target) {
    queue_t *path = create_queue();
    int **visited = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        visited[i] = (int *)calloc(cols, sizeof(int));
    }

    if (backtrack(map, rows, cols, *start, *target, path, visited)) {
        return path;
    } else {
        // Free the queue and visited if no path is found
        while (!is_empty(path)) {
            dequeue(path);
        }
        free(path);
        for (int i = 0; i < rows; i++) {
            free(visited[i]);
        }
        free(visited);
        return NULL;
    }
}

// Example usage
int main() {
    char *map[] = {
        "0010",
        "0000",
        "0100",
        "0001"
    };

    point_t start = {0, 0};
    point_t target = {3, 3};

    queue_t *path = backtracking_array(map, 4, 4, &start, &target);

    if (path != NULL) {
        printf("Path found:\n");
        print_queue(path);
    } else {
        printf("No path found\n");
    }

    return 0;
}
