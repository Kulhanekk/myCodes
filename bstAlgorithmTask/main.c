#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int function;
    int visited;
    int len;
    int x;
    int y;
    int z;
} node_t;

typedef struct graphs
{
    int w;
    int h;
    int n_colors;
    node_t ****graph;
} graphs_t;

typedef struct queue
{
    node_t **queue;
    int front;
    int end;
} queue_t;

int find_solution(graphs_t *graphs);
queue_t *init_queue(int size);
int is_empty(queue_t *q);
void enqueue(queue_t *q, node_t *node);
node_t *dequeue(queue_t *q);
graphs_t *init_struct();
graphs_t *init_from_stdin();
queue_t *init_queue(int len);
void print_graphs(graphs_t *graphs);

int main()
{
    graphs_t *graphs = init_from_stdin();
    print_graphs(graphs);
    printf("%d\n", find_solution(graphs));
}

int find_solution(graphs_t *graphs)
{
    int q_size = graphs->w * graphs->h * graphs->n_colors;
    queue_t *q = init_queue(q_size);
    node_t *tmp = graphs->graph[0][graphs->h - 1][0];
    tmp->visited = 1;
    tmp->len = 0;
    enqueue(q, tmp);
    //printf("visited: [%d:%d:%d]\n", tmp->x, tmp->y, tmp->z);
    print_graphs(graphs);
    while (!is_empty(q))
    {
        node_t *tmp = dequeue(q);
        //printf("dq: [%d:%d:%d] -> %d\n", tmp->x, tmp->y, tmp->z, tmp->len);
        if (tmp->x == graphs->w - 1 && tmp->y == 0)
        {
            //printf("here we are: [%d:%d:%d] -> %d\n", tmp->x, tmp->y, tmp->z, tmp->len);
            return tmp->len;
        }

        if (tmp->function < 0)
        {
            //printf("dddd: %d\n", abs(tmp->function));
            int new_pos = abs(tmp->function);
            if (!graphs->graph[new_pos][tmp->y][tmp->x]->visited)
            {
                graphs->graph[new_pos][tmp->y][tmp->x]->visited = 1;
                graphs->graph[new_pos][tmp->y][tmp->x]->len = tmp->len;
                enqueue(q, graphs->graph[new_pos][tmp->y][tmp->x]);
                //printf("visited: [%d:%d:%d]\n", tmp->x, tmp->y, new_pos);
                print_graphs(graphs);
            }
        }
        if (tmp->function && abs(tmp->function) != tmp->z)
        {
            //printf("NEROZPLIZUJ SE!!\n");
            continue;
        }
        if (tmp->x + 1 < graphs->w && graphs->graph[tmp->z][tmp->y][tmp->x + 1]->function <= 0)
        {
            if (!graphs->graph[tmp->z][tmp->y][tmp->x + 1]->visited)
            {
                graphs->graph[tmp->z][tmp->y][tmp->x + 1]->visited = 1;
                graphs->graph[tmp->z][tmp->y][tmp->x + 1]->len = tmp->len + 1;
                enqueue(q, graphs->graph[tmp->z][tmp->y][tmp->x + 1]);
                //printf("visited: [%d:%d:%d]\n", tmp->x + 1, tmp->y, tmp->z);
                print_graphs(graphs);
            }
        }
        if (tmp->x - 1 >= 0 && graphs->graph[tmp->z][tmp->y][tmp->x - 1]->function <= 0)
        {
            if (!graphs->graph[tmp->z][tmp->y][tmp->x - 1]->visited)
            {
                graphs->graph[tmp->z][tmp->y][tmp->x - 1]->visited = 1;
                graphs->graph[tmp->z][tmp->y][tmp->x - 1]->len = tmp->len + 1;
                enqueue(q, graphs->graph[tmp->z][tmp->y][tmp->x - 1]);
                //printf("visited: [%d:%d:%d]\n", tmp->x - 1, tmp->y, tmp->z);
                print_graphs(graphs);
            }
        }
        if (tmp->y + 1 < graphs->h && graphs->graph[tmp->z][tmp->y + 1][tmp->x]->function <= 0)
        {
            if (!graphs->graph[tmp->z][tmp->y + 1][tmp->x]->visited)
            {
                graphs->graph[tmp->z][tmp->y + 1][tmp->x]->visited = 1;
                graphs->graph[tmp->z][tmp->y + 1][tmp->x]->len = tmp->len + 1;
                enqueue(q, graphs->graph[tmp->z][tmp->y + 1][tmp->x]);
                //printf("visited: [%d:%d:%d]\n", tmp->x, tmp->y + 1, tmp->z);
                print_graphs(graphs);
            }
        }
        if (tmp->y - 1 >= 0 && graphs->graph[tmp->z][tmp->y - 1][tmp->x]->function <= 0)
        {
            if (!graphs->graph[tmp->z][tmp->y - 1][tmp->x]->visited)
            {
                graphs->graph[tmp->z][tmp->y - 1][tmp->x]->visited = 1;
                graphs->graph[tmp->z][tmp->y - 1][tmp->x]->len = tmp->len + 1;
                enqueue(q, graphs->graph[tmp->z][tmp->y - 1][tmp->x]);
                //printf("visited: [%d:%d:%d]\n", tmp->x, tmp->y - 1, tmp->z);
                print_graphs(graphs);
            }
        }
    }
    return -1;
}

queue_t *init_queue(int size)
{
    queue_t *q = malloc(sizeof(queue_t));
    q->queue = malloc(sizeof(node_t *) * size);
    q->front = -1;
    q->end = -1;
    return q;
}

int is_empty(queue_t *q)
{
    return q->front == -1;
}

void enqueue(queue_t *q, node_t *node)
{
    if (is_empty(q))
    {
        q->front = 0;
        q->end = 0;
    }
    else
    {
        q->end++;
    }
    q->queue[q->end] = node;
}

node_t *dequeue(queue_t *q)
{
    if (is_empty(q))
    {
        return NULL;
    }

    node_t *dequeuedNode = q->queue[q->front];

    if (q->front == q->end)
    {
        q->front = -1;
        q->end = -1;
    }
    else
    {
        q->front++;
    }

    return dequeuedNode;
}

graphs_t *init_struct()
{
    graphs_t *graphs = malloc(sizeof(graphs_t));
    graphs->graph = NULL;
    return graphs;
}

graphs_t *init_from_stdin()
{
    graphs_t *graphs = init_struct();
    if (scanf("%d %d %d", &graphs->h, &graphs->w, &graphs->n_colors))
    {
        graphs->n_colors += 1;
        node_t ****types = malloc(graphs->n_colors * sizeof(node_t ***));
        for (size_t k = 0; k < graphs->n_colors; k++)
        {
            types[k] = malloc(graphs->h * sizeof(node_t **));

            for (size_t i = 0; i < graphs->h; i++)
            {
                types[k][i] = malloc(graphs->w * sizeof(node_t *));
                for (size_t j = 0; j < graphs->w; j++)
                {
                    types[k][i][j] = malloc(sizeof(node_t));
                }
            }
        }

        for (size_t i = 0; i < graphs->h; i++)
        {
            for (size_t j = 0; j < graphs->w; j++)
            {
                int val;
                if (scanf("%d", &val))
                {
                    for (size_t k = 0; k < graphs->n_colors; k++)
                    {
                        if (k > 0 && val > 0 && val == k)
                        {
                            types[k][i][j]->function = 0;
                        }
                        else
                        {
                            types[k][i][j]->function = val;
                        }
                        types[k][i][j]->visited = 0;
                        types[k][i][j]->len = 0;
                        types[k][i][j]->x = j;
                        types[k][i][j]->y = i;
                        types[k][i][j]->z = k;
                    }
                    //printf("[%d %d] ", types[0][i][j]->y, types[0][i][j]->x);
                }
            }
            //printf("\n");
        }
        graphs->graph = types;
    }
    return graphs;
}

void print_graphs(graphs_t *graphs)
{
    for (size_t k = 0; k < graphs->n_colors; k++)
    {
        for (size_t i = 0; i < graphs->h; i++)
        {
            for (size_t j = 0; j < graphs->w; j++)
            {
                //printf("%2d", graphs->graph[k][i][j]->visited);
            }
            //printf("\n");
        }
        //printf("\n---------------------------\n");
    }
}
