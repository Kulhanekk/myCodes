#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct hallway
{
    size_t time;
    size_t balance;
} HALLW_T;

typedef struct room
{
    size_t number;
    size_t *boxes;
    size_t n_boxes;
    int weight;

    struct room *parent;

    HALLW_T h_left;
    struct room *left;
    HALLW_T h_right;
    struct room *right;
} ROOM_T;

typedef struct whouse
{
    size_t n_rooms;
    size_t n_boxes;
    ROOM_T *first_room;
    size_t *boxes;

    size_t balance;
    size_t time;
} WHOUSE_T;

WHOUSE_T *innit_warehouse(int n, int b);
void print_tree(ROOM_T *room);
void calc_balance(ROOM_T *room, size_t *balancer);
void fill_rooms(WHOUSE_T *whouse);
void find_solution(WHOUSE_T *whouse, size_t *room, size_t *boxes_moved, size_t *timer);
void put_box_in_room(WHOUSE_T *whouse, ROOM_T *room, size_t *boxes_moved, size_t *timer, size_t room_time);
int can_put_box(ROOM_T *room);
void remove_box_from_room(WHOUSE_T *whouse, ROOM_T *room, size_t *boxes_moved, size_t *timer, size_t room_time);
ROOM_T *find_room(ROOM_T *room, size_t number, size_t *time);
ROOM_T *innit_room(size_t n_boxes);
void *my_malloc(size_t size);
void *my_realloc(void *ptr, size_t size);

int main(int argc, char *argv[])
{
    int n;
    int b;
    WHOUSE_T *whouse;
    if (scanf("%d %d", &n, &b) == 2)
    {
        whouse = innit_warehouse(n, b);
    }
    // print_warehouse(whouse->first_room);
    size_t timer = 0;
    size_t balancer = 0;
    size_t n_boxes = 0;
    find_solution(whouse, &balancer, &n_boxes, &timer);
    printf("%zu %zu\n", whouse->balance, whouse->time);
    free(whouse->boxes);
    free(whouse);
}

WHOUSE_T *innit_warehouse(int n, int b)
{
    WHOUSE_T *whouse = my_malloc(sizeof(WHOUSE_T));
    whouse->boxes = my_malloc(sizeof(size_t) * b);
    whouse->n_rooms = n;
    whouse->n_boxes = b;
    whouse->balance = (size_t)0 - 1;
    whouse->time = (size_t)0 - 1;
    for (size_t i = 0; i < b; i++)
    {
        if (!scanf("%zu", &whouse->boxes[i]))
        {
            fprintf(stderr, "ERROR: input at boxes\n");
            free(whouse->boxes);
            free(whouse);
            exit(101);
        }
    }
    fill_rooms(whouse);
    return whouse;
}

void print_tree(ROOM_T *room)
{
    ROOM_T *left_tree = room->left;
    ROOM_T *right_tree = room->right;
    // printf("%zu %zu %zu\n", room->number, room->h_left.time, room->h_right.time);

    printf("BOXES in %zu:\n", room->number);
    if (room->n_boxes)
    {
        for (size_t i = 0; i < room->n_boxes; i++)
        {
            printf("%zu ", room->boxes[i]);
        }
    }
    printf("\nroom_weight: %d\n", room->weight);
    printf("left balance: %zu\n", room->h_left.balance);
    printf("right balance: %zu\n", room->h_right.balance);
    printf("\n--------------------------------------------\n");
    if (left_tree)
    {
        print_tree(room->left);
    }

    if (right_tree)
    {
        print_tree(room->right);
    }
}

void calc_balance(ROOM_T *room, size_t *balancer)
{
    ROOM_T *left_tree = room->left;
    ROOM_T *right_tree = room->right;
    // printf("%zu %zu %zu\n", room->number, room->h_left.time, room->h_right.time);

    /*printf("BOXES in %zu:\n", room->number);
    if (room->n_boxes)
    {
        for (size_t i = 0; i < room->n_boxes; i++)
        {
            printf("%zu ", room->boxes[i]);
        }
    }
    printf("\nroom_weight: %d\n", room->weight);
    printf("left balance: %zu\n", room->h_left.balance);
    printf("right balance: %zu\n", room->h_right.balance);
    printf("\n--------------------------------------------\n");*/

    if (left_tree)
    {
        room->h_left.balance = abs(room->weight - left_tree->weight);
        calc_balance(room->left, balancer);
    }

    if (right_tree)
    {
        room->h_right.balance = abs(room->weight - right_tree->weight);
        calc_balance(room->right, balancer);
    }
    *balancer += room->h_left.balance;
    *balancer += room->h_right.balance;
}

void fill_rooms(WHOUSE_T *whouse)
{
    whouse->first_room = innit_room(whouse->n_boxes);
    ROOM_T *tmp = whouse->first_room;
    tmp->number = 0;
    size_t number;
    size_t connect;
    size_t time;
    size_t dump = 0;
    size_t i = 0;
    while (i < whouse->n_rooms - 1)
    {
        if (scanf("%zu %zu %zu", &number, &connect, &time) == 3)
        {
            if (number == tmp->number && tmp->h_left.time == 0)
            {
                i += 1;
                tmp->h_left.time = time;
                tmp->left = innit_room(whouse->n_boxes);
                tmp->left->parent = tmp;
                tmp->left->number = connect;
            }
            else if (number == tmp->number && tmp->h_right.time == 0)
            {
                i += 1;
                tmp->h_right.time = time;
                tmp->right = innit_room(whouse->n_boxes);
                tmp->right->parent = tmp;
                tmp->right->number = connect;
            }
            else
            {
                if (tmp->left->number == number)
                {
                    tmp = tmp->left;
                }
                else if (tmp->right->number == number)
                {
                    tmp = tmp->right;
                }
                else
                {
                    // TODO - maybe tmp->left->parent = tmp; is needed here too, idk
                    tmp = find_room(whouse->first_room, number, &dump);
                    if (!tmp)
                    {
                        fprintf(stderr, "ERROR: no room found\n");
                        exit(1);
                    }
                }
                if (number == tmp->number && !tmp->h_left.time)
                {
                    i += 1;
                    tmp->h_left.time = time;
                    tmp->left = innit_room(whouse->n_boxes);
                    tmp->left->parent = tmp;
                    tmp->left->number = connect;
                }
                else if (number == tmp->number && !tmp->h_right.time)
                {
                    i += 1;
                    tmp->h_right.time = time;
                    tmp->right = innit_room(whouse->n_boxes);
                    tmp->right->parent = tmp;
                    tmp->right->number = connect;
                }
            }
        }
    }
}

void find_solution(WHOUSE_T *whouse, size_t *balancer, size_t *boxes_moved, size_t *timer)
{
    /*
    recur(N):
    for r in rooms:
    if can_put_box(room):
      put_box(room)
      recur(N-1)
      remove_box(room);


      N -> items in whouse->n_boxes
    */
    if (*boxes_moved == whouse->n_boxes)
    {
        calc_balance(whouse->first_room, balancer);
        if (*balancer < whouse->balance || (*balancer == whouse->balance && *timer < whouse->time))
        {
            whouse->balance = *balancer;
            whouse->time = *timer;
        }
        if (*balancer == 23 && *timer == 290)
        {
            // print_tree(whouse->first_room);
        }

        // printf("end of iteration-----------------%zu--------\n", *timer);
        // printf("balancer: %zu\n", *balancer);
        (*balancer) = 0;

        return;
    }
    for (size_t i = 0; i < whouse->n_rooms; i++)
    {
        size_t room_time = 0;
        ROOM_T *tmp = find_room(whouse->first_room, i, &room_time);
        if (can_put_box(tmp))
        {
            put_box_in_room(whouse, tmp, boxes_moved, timer, room_time);
            find_solution(whouse, balancer, boxes_moved, timer);
            remove_box_from_room(whouse, tmp, boxes_moved, timer, room_time);
        }
    }
}

int can_put_box(ROOM_T *room)
{
    if (room->left)
    {
        if (room->left->n_boxes == 0)
        {
            return 0;
        }
    }

    if (room->right)
    {
        if (room->right->n_boxes == 0)
        {
            return 0;
        }
    }
    
    if (room->parent && room->parent->n_boxes != 0)
    {
        return 0;
    }

    if (room->left && room->left->n_boxes == 0)
    {
        return can_put_box(room->left);
    }

    if (room->right && room->right->n_boxes == 0)
    {
        return can_put_box(room->right);
    }

    return 1;
}

void put_box_in_room(WHOUSE_T *whouse, ROOM_T *room, size_t *boxes_moved, size_t *timer, size_t room_time)
{
    room->boxes[room->n_boxes] = whouse->boxes[*boxes_moved];
    // printf("%zu -> %zu\n", room->boxes[room->n_boxes], room->number);
    room->weight += room->boxes[room->n_boxes];
    room->n_boxes += 1;
    *boxes_moved += 1;
    (*timer) += room_time;
}

void remove_box_from_room(WHOUSE_T *whouse, ROOM_T *room, size_t *boxes_moved, size_t *timer, size_t room_time)
{
    (*timer) -= room_time;
    *boxes_moved -= 1;
    room->n_boxes -= 1;
    room->weight -= room->boxes[room->n_boxes];
    // printf("%zu <- %zu\n", room->boxes[room->n_boxes], room->number);
    room->boxes[room->n_boxes] = 0;
}

ROOM_T *find_room(ROOM_T *room, size_t number, size_t *time)
{
    if (room->number == number)
    {
        return room;
    }

    ROOM_T *ret = NULL;
    if (room->left)
    {
        *time += room->h_left.time;
        ret = find_room(room->left, number, time);
        if (ret)
        {
            return ret;
        }
    }
    *time -= room->h_left.time;
    if (room->right)
    {
        *time += room->h_right.time;
        ret = find_room(room->right, number, time);
    }
    if (!ret)
    {
        *time -= room->h_right.time;
    }
    return ret;
}

ROOM_T *innit_room(size_t n_boxes)
{
    ROOM_T *room = my_malloc(sizeof(ROOM_T));
    room->boxes = my_malloc(sizeof(size_t) * n_boxes);
    room->n_boxes = 0;
    room->left = NULL;
    room->right = NULL;
    room->parent = NULL;
    room->h_left.balance = 0;
    room->h_left.time = 0;
    room->h_right.balance = 0;
    room->h_right.time = 0;
    room->weight = 0;
    return room;
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
