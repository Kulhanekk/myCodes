#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INIT_LINE_SIZE 4

typedef enum err
{
    REALLOC_ERR = 102,
    MALLOC_ERR = 101,
    INPUT_ERR = 100,
    OK = 0
} err_enum;

typedef struct matrix_struct
{
    int *data;
    int height;
    int width;
    int len;
} matrix_t;

matrix_t *matrix_innit(int height, int width, int *data);
matrix_t *matrix_input();
void print_matrix(matrix_t *matrix);
matrix_t *matrix_sub_add(matrix_t *matrix_1, matrix_t *matrix_2, char operation);
matrix_t *matrix_multiply(matrix_t *matrix_1, matrix_t *matrix_2);
int calculate_answer(matrix_t **matrixes, char *signs, int n_matrixes);
void zero_out_matrix(matrix_t *matrix);
void free_matrix(matrix_t *matrix);
void *my_malloc(size_t size);
void *my_realloc(void *ptr, size_t size);

/* The main program */
int main()
{
    int ret = OK;
    size_t capacity = INIT_LINE_SIZE;
    size_t size = 0;
    matrix_t **matrixes = my_malloc(sizeof(matrix_t *) * capacity);
    char *signs = my_malloc(sizeof(char) * capacity);
    bool correct_matrix = true;
    while (true)
    {
        matrixes[size] = matrix_input();
        if (!matrixes[size])
        {
            correct_matrix = false;
            ret = INPUT_ERR;
            break;
        }
        if (!scanf("%c\n", &signs[size]))
        {
            correct_matrix = false;
            ret = INPUT_ERR;
            break;
        }
        if (feof(stdin))
        {
            break;
        }
        if ((size + 1) >= capacity)
        {
            capacity *= 2;
            matrixes = my_realloc(matrixes, sizeof(matrix_t *) * capacity);
            signs = my_realloc(signs, sizeof(char) * capacity);
        }
        size += 1;
    }
    signs[size] = '\0';
    if (correct_matrix)
    {
        ret = calculate_answer(matrixes, signs, size);
    }
    if (ret)
    {
        fprintf(stderr, "Error: Chybny vstup!\n");
        ret = INPUT_ERR;
    }
    for (size_t i = 0; i <= size; i++)
    {
        free_matrix(matrixes[i]);
    }
    free(signs);
    free(matrixes);
    return ret;
}

matrix_t *matrix_innit(int height, int width, int *data)
{
    matrix_t *matrix = my_malloc(sizeof(matrix_t));
    matrix->width = width;
    matrix->height = height;
    matrix->len = height * width;
    matrix->data = my_malloc(sizeof(int) * matrix->len * 8);
    for (int i = 0; i < matrix->len; i++)
    {
        matrix->data[i] = data[i];
    }
    return matrix;
}

matrix_t *matrix_input()
{
    int height;
    int width;
    matrix_t *matrix = NULL;
    if (scanf("%d %d\n", &height, &width) == 2)
    {
        int len = height * width;
        int *data = my_malloc(sizeof(int) * len * 2);

        int position = 0;
        int rows = 1;
        char test_char = 0;

        while (scanf("%d%c", &data[position], &test_char) == 2 && position + 1 < len)
        {
            if (test_char == '\n')
            {
                rows += 1;
            }
            position += 1;
        }
        if (height != rows || position + 1 != len)
        {
            matrix = NULL;
        }
        else
        {
            matrix = matrix_innit(height, width, data);
        }
        free(data);
    }
    return matrix;
}

void print_matrix(matrix_t *matrix)
{
    printf("%d %d\n", matrix->height, matrix->width);
    for (int i = 1; i <= matrix->len; i++)
    {
        printf("%d", matrix->data[i - 1]);
        if (i % matrix->width == 0)
        {
            printf("\n");
        }
        else
        {
            printf(" ");
        }
    }
}

matrix_t *matrix_sub_add(matrix_t *matrix_1, matrix_t *matrix_2, char operation)
{
    matrix_t *new_matrix = NULL;
    if (matrix_1 && matrix_2 &&
        matrix_1->height == matrix_2->height &&
        matrix_1->width == matrix_2->width)
    {
        int len = matrix_1->height * matrix_1->width;
        new_matrix = matrix_innit(matrix_1->height, matrix_1->width, matrix_1->data);

        for (int i = 0; i < len; i++)
        {
            if (operation == '+')
            {
                new_matrix->data[i] = matrix_1->data[i] + matrix_2->data[i];
            }
            else if (operation == '-')
            {
                new_matrix->data[i] = matrix_1->data[i] - matrix_2->data[i];
            }
        }
    }
    return new_matrix;
}

matrix_t *matrix_multiply(matrix_t *matrix_1, matrix_t *matrix_2)
{
    matrix_t *new_matrix = NULL;
    if (matrix_1 && matrix_2 && matrix_1->width == matrix_2->height)
    {
        int new_height = matrix_1->height;
        new_matrix = matrix_innit(matrix_1->height, matrix_2->width, matrix_1->data);
        zero_out_matrix(new_matrix);
        int new_mat_index = 0;
        for (int row_push = 0; row_push <= new_height; row_push++)
        {
            for (int j = 0; j < matrix_2->width; j++)
            {
                for (int i = 0; i < matrix_1->width; i++)
                {
                    int tmp = matrix_1->data[i + matrix_1->width * row_push] *
                              matrix_2->data[i * matrix_2->width + j];
                    new_matrix->data[new_mat_index] += tmp;
                }
                new_mat_index += 1;
            }
        }
    }
    return new_matrix;
}

int calculate_answer(matrix_t **matrixes, char *signs, int n_matrixes)
{
    int ret = OK;
    matrix_t *final_mat = NULL;
    for (int i = 0; i <= n_matrixes; i++)
    {
        if (i >= 1 && signs[i - 1] != '*' && signs[i] != '*')
        {
            matrix_t *tmp = matrix_sub_add(final_mat, matrixes[i], signs[i - 1]);
            free_matrix(final_mat);
            if (!tmp)
            {
                ret = INPUT_ERR;
                break;
            }
            final_mat = tmp;
        }
        else if (i < 1 && signs[i] != '*')
        {
            final_mat = matrix_innit(matrixes[i]->height, matrixes[i]->width, matrixes[i]->data);
        }
        else
        {
            int n = i;
            matrix_t *mult_mat = NULL;
            while (signs[n] == '*')
            {
                if (n == i)
                {
                    mult_mat = matrix_multiply(matrixes[n], matrixes[n + 1]);
                    if (!mult_mat)
                    {
                        ret = INPUT_ERR;
                        break;
                    }
                }
                else
                {
                    matrix_t *tmp = matrix_multiply(mult_mat, matrixes[n + 1]);
                    if (!tmp)
                    {
                        ret = INPUT_ERR;
                        break;
                    }
                    free_matrix(mult_mat);
                    mult_mat = tmp;
                }
                n += 1;
            }
            if (i > 0)
            {
                matrix_t *tmp = matrix_sub_add(final_mat, mult_mat, signs[i - 1]);
                if (!tmp)
                {
                    ret = INPUT_ERR;
                    break;
                }
                free_matrix(final_mat);
                free_matrix(mult_mat);
                final_mat = tmp;
            }
            else
            {
                final_mat = mult_mat;
            }
            i = n;
        }
        if (ret)
        {
            break;
        }
    }
    if (!ret)
    {
        print_matrix(final_mat);
        free_matrix(final_mat);
    }
    return ret;
}

void zero_out_matrix(matrix_t *matrix)
{
    if (matrix)
    {
        for (int i = 0; i < matrix->len; i++)
        {
            matrix->data[i] = 0;
        }
    }
}

void free_matrix(matrix_t *matrix)
{
    if (matrix)
    {
        if (matrix->data)
        {
            free(matrix->data);
        }
        free(matrix);
    }
}

void *my_malloc(size_t size)
{
    void *ret = malloc(size);
    if (!ret)
    {
        fprintf(stderr, "ERROR: malloc!\n");
        exit(MALLOC_ERR);
    }
    return ret;
}

void *my_realloc(void *ptr, size_t size)
{
    void *ret = realloc(ptr, size);
    if (!ret)
    {
        fprintf(stderr, "ERROR: realloc!\n");
        exit(REALLOC_ERR);
    }
    return ret;
}
