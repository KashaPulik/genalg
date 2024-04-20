#include <stdio.h>
#include <stdlib.h>

#define N_INDS 5

struct pair {
    int dad;
    int mom;
};

int summ_array(int* a, int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
        sum += a[i];
    return sum;
}

int validate(int* a, int size, int e)
{
    if (summ_array(a, size) > e)
        return 1;
    return 0;
}

int** generate_first_generation(int size, int e)
{
    int** x = malloc(sizeof(*x) * N_INDS);
    for (int i = 0; i < N_INDS; i++) {
        x[i] = malloc(sizeof(int) * size);
        for (int j = 0; j < size; j++) {
            x[i][j] = rand() % e + 1;
        }
    }
    return x;
}

int count_norm(int* a, int size, int e, int* x)
{
    int left_side = 0;
    for (int i = 0; i < size; i++)
        left_side += a[i] * x[i];

    return left_side > e ? left_side - e : e - left_side;
}

double count_reverse_k(int* norm)
{
    double sum = 0;
    for (int i = 0; i < N_INDS; i++)
        sum += 1.0 / (double)norm[i];
    return sum;
}

void count_percents(int* norm, double* percents, double reverse_k)
{
    for (int i = 0; i < N_INDS; i++)
        percents[i] = (1.0 / (double)norm[i]) / reverse_k;
}

double count_average_percent(double* percents)
{
    double sum = 0;
    for (int i = 0; i < N_INDS; i++)
        sum += percents[i];
    return sum / N_INDS;
}

int this_pair_exist(struct pair* pairs, int dad, int mom)
{
    for (int i = 0; i < N_INDS; i++) {
        if (pairs[i].dad == dad && pairs[i].mom == mom)
            return 1;
        if (pairs[i].dad == mom && pairs[i].mom == dad)
            return 1;
    }
    return 0;
}

void generate_new_generation(int** x, int size, int e, int mutations)
{
    int new_generation[N_INDS][size];
    struct pair pairs[N_INDS];
    int dad, mom;

    for (int i = 0; i < N_INDS; i++) {
        do {
            dad = rand() % N_INDS;
            mom = dad;
            while (mom == dad)
                mom = rand() % N_INDS;
        } while (this_pair_exist(pairs, dad, mom));
        pairs[i].dad = dad;
        pairs[i].mom = mom;
    }
    int gen;
    int mutation;
    if (mutations == 0) {
        for (int i = 0; i < N_INDS; i++) {
            for (int j = 0; j < size; j++) {
                gen = rand() % 2;
                if (gen == 0)
                    new_generation[i][j] = x[pairs[i].dad][j];
                else
                    new_generation[i][j] = x[pairs[i].mom][j];
            }
        }
    } else {
        for (int i = 0; i < N_INDS; i++) {
            for (int j = 0; j < size; j++) {
                mutation = rand() % 10;
                if (mutation) {
                    new_generation[i][j] = rand() % e + 1;
                } else {
                    gen = rand() % 2;
                    if (gen == 0)
                        new_generation[i][j] = x[pairs[i].dad][j];
                    else
                        new_generation[i][j] = x[pairs[i].mom][j];
                }
            }
        }
    }
    for (int i = 0; i < N_INDS; i++)
        for (int j = 0; j < size; j++)
            x[i][j] = new_generation[i][j];
}

int norm_sum(int* norm)
{
    int sum = 0;
    for (int i = 0; i < N_INDS; i++)
        sum += norm[i];
    return sum;
}

int** solve(int* a, int size, int e)
{
    int** x = generate_first_generation(size, e);
    int norm[N_INDS];

    for (int i = 0; i < N_INDS; i++)
        norm[i] = count_norm(a, size, e, x[i]);
    float reverse_k = count_reverse_k(norm);
    int n_generations = 1;
    double accuracy_percent[N_INDS];
    double average_percent = 0;
    int need_mutation = 0;

    while (norm_sum(norm) != 0) {
        n_generations++;
        for (int i = 0; i < N_INDS; i++) {
            norm[i] = count_norm(a, size, e, x[i]);
            printf("%d\n", norm[i]);
        }
        reverse_k = count_reverse_k(norm);
        count_percents(norm, accuracy_percent, reverse_k);
        if (count_average_percent(accuracy_percent) <= average_percent)
            need_mutation = 1;

        average_percent = count_average_percent(accuracy_percent);
        generate_new_generation(x, size, e, need_mutation);
        need_mutation = 0;
        printf("Generation %i\n", n_generations);
    }
    return x;
}

int main()
{
    int a[4] = {1, 2, 3, 4};
    int** x = solve(a, 4, 30);
    for (int i = 0; i < N_INDS; i++) {
        printf("%d * %d + %d * %d + %d * %d + %d * %d = 30\n", x[i][0], a[0], x[i][1], a[1], x[i][2], a[2], x[i][3], a[3]);
    }
}