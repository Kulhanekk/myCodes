#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int function;

    struct node *up;
    struct node *down;
    struct node *left;
    struct node *right;
} node_t;

typedef struct coord
{
    int x;
    int y;
} coord_t;

typedef coord_t player_t;

typedef struct graph
{
    int phase;
    node_t *top_left;
} graph_t;

typedef struct everyg
{
    int w;
    int h;
    player_t player;
    graph_t **graphs;
} everyg_t;

everyg_t *init_graph_from_stdin(int h, int w, int n_colors);
everyg_t *init_everyg(int h, int w, int n_colors);
graph_t *init_graph(int phase);
node_t *init_node(int function);
void *my_malloc(size_t size);
void *my_realloc(void *ptr, size_t size);

int main()
{
    int h, w, n_colors;
    everyg_t *every_graph;
    if (scanf("%d %d %d", &h, &w, &n_colors) == 3)
    {
        every_graph = init_graph_from_stdin(h, w, n_colors);
    }

    return 0;
}

everyg_t *init_graph_from_stdin(int h, int w, int n_colors)
{
    everyg_t *every_graph = init_everyg(h, w, n_colors);
    node_t *tmp_nodes[n_colors + 1];
    for (size_t k = 0; k < n_colors + 1; k++)
    {
        tmp_nodes[k] = every_graph->graphs[k]->top_left;
        tmp_nodes[k] = init_node(0);
    }
    for (size_t i = 0; i < h; i++)
    {
        node_t *tmp_rows[n_colors + 1];
        for (size_t k = 0; k < n_colors + 1; k++)
        {
            tmp_rows[k] = tmp_nodes[k];
        }
        for (size_t j = 0; j < w; j++)
        {
            int node_function;
            if (scanf("%d ", &node_function))
            {
                for (size_t k = 0; k < n_colors + 1; k++)
                {
                    tmp_rows[k]->function = node_function;
                    if (i == 0 && j < w - 1)
                    {
                        tmp_rows[k]->right = init_node(0);
                        tmp_rows[k]->right->left = tmp_rows[k];
                    }
                    else if (j < w - 1)
                    {
                        tmp_rows[k]->right = tmp_rows[k]->up->right->down;
                        tmp_rows[k]->right->left = tmp_rows[k];
                    }
                    if (i < h - 1)
                    {
                        tmp_rows[k]->down = init_node(0);
                        tmp_rows[k]->down->up = tmp_rows[k];
                    }
                }
            }
            for (size_t k = 0; k < n_colors + 1; k++)
            {
                tmp_rows[k] = tmp_rows[k]->right;
            }
        }
        for (size_t k = 0; k < n_colors + 1; k++)
        {
            tmp_nodes[k] = tmp_nodes[k]->down;
        }
    }
    return every_graph;
}

everyg_t *init_everyg(int h, int w, int n_colors)
{
    everyg_t *all_graphs = my_malloc(sizeof(everyg_t));
    all_graphs->graphs = my_malloc(sizeof(graph_t *) * (n_colors + 1));
    all_graphs->player.x = 0;
    all_graphs->player.y = h - 1;
    all_graphs->h = h;
    all_graphs->w = w;

    for (size_t i = 0; i < n_colors + 1; i++)
    {
        all_graphs->graphs[i] = init_graph(i);
    }
    return all_graphs;
}

graph_t *init_graph(int phase)
{
    graph_t *graph = my_malloc(sizeof(graph_t));
    graph->phase = phase;
    graph->top_left = NULL;
    return graph;
}

node_t *init_node(int function)
{
    node_t *new = my_malloc(sizeof(node_t));
    new->function = function;
    new->down = NULL;
    new->up = NULL;
    new->left = NULL;
    new->right = NULL;
    return new;
}

void *my_malloc(size_t size)
{
    void *ret = malloc(size);
    if (!ret)
    {
        fprintf(stderr, "ERROR: malloc!\n");
        exit(100);
    }
    return ret;
}

void *my_realloc(void *ptr, size_t size)
{
    void *ret = realloc(ptr, size);
    if (!ret)
    {
        fprintf(stderr, "ERROR: realloc!\n");
        exit(100);
    }
    return ret;
}
