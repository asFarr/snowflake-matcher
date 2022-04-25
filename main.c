// snowflakes.c -- Walkthrough introduction to linked list hash table data structure, & algorithm optimization.
// Created by toads on 4/16/22.
// From "Algorithmic Thinking" by Daniel Zingaro.

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 100000
#define DEGREE 6

typedef struct snowflake_node {
    // init linked list node struct
    int snowflake[DEGREE];
    struct snowflake_node *next;
} snowflake_node;

int identical_left(const int snow1[], const int snow2[], int start) {
    // AND Gate for determining identity match when parsed from left to right
    int offset;
    int snow2_index;
    for (offset = 0; offset < DEGREE; offset++) {
        snow2_index = start - offset;
        if (snow2_index < 0) {
            snow2_index = snow2_index + DEGREE;
}
        if (snow1[offset] != snow2[snow2_index]) {
            return 0;
}
    }
    return 1;
}

int identical_right(const int snow1[], const int snow2[], int start) {
    // AND Gate for determining identity match when parsed from left to right
    int offset;
    for (offset = 0; offset < DEGREE; offset++) {
        if (snow1[offset] != snow2[(start + offset) % DEGREE]) {
            return 0;
}
    }
    return 1;
}

int are_identical(int snow1[], int snow2[]) {
    // OR Gate for two possible identity types
    int start;
    for (start = 0; start < DEGREE; start++) {
        if (identical_right(snow1, snow2, start)) {
            return 1;
}
        if (identical_left(snow1, snow2, start)) {
            return 1;
}
    }
    return 0;
}

void identify_identical(snowflake_node *snowflakes[]) {
    // parses linked list hash table structure to compare snowflake hash values
    snowflake_node *node1, *node2;
    for (int i = 0; i < SIZE; i++) {
        node1 = snowflakes[i];
        while (node1 != NULL) {
            node2 = node1->next;
            while (node2 != NULL) {
                if (are_identical(node1->snowflake, node2->snowflake))
                {
                    printf("Twin snowflakes found.\n");
                    return;
                }
                node2 = node2->next;
            }
            node1 = node1->next;
        }
    }
    printf("No two snowflakes are alike.\n");
}

int code(const int snowflake[]) {
    // computes and returns hash code for identity comparison
    int sum = 0;
    for(int i; i < DEGREE; i++){
        sum += snowflake[i];
    }
    return (sum % SIZE);
}

int rand_lim(int limit) {
    // return a random number between 0 and limit inclusive.
    int divisor = RAND_MAX/(limit+1);
    int retval;

    do {
        retval = rand() / divisor;
    } while (retval > limit);

    return retval;
}

int main() {
    // main driving logic
    srand((unsigned int) time(NULL));
    static snowflake_node *snowflakes[SIZE] = {NULL};
    snowflake_node *snow;
    int n, i, j, snowflake_code;

    printf("Enter number of snowflakes to generate: ");
    scanf("%d", &n);

    time_t t0 = time(0);

    printf("\nGenerating ");
    printf("%d", n);
    printf(" Snowflakes of degree: ");
    printf("%d", DEGREE);
    printf(" ...\n\n");

    for (i = 0; i < n; i++) {
        snow = malloc(sizeof(snowflake_node));
        if (snow == NULL) {
            fprintf(stderr, "malloc error\n");
            exit(1);
        }
        for (j = 0; j < DEGREE; j++) {
            snow->snowflake[j] = rand_lim(DEGREE);
        }

        snowflake_code = code(snow->snowflake);
        snow->next = snowflakes[snowflake_code];
        snowflakes[snowflake_code] = snow;
    }

    time_t t1 = time(0);
    double gen_diff_ms = difftime(t1, t0) * 1000.;
    printf("Generation completed in: ");
    printf("%f", gen_diff_ms);
    printf(" ms\n\n");
    printf("Checking for matches...\n\n");

    time_t t2 = time(0);
    identify_identical(snowflakes);

    time_t t3 = time(0);
    double matching_diff_ms = difftime(t3, t2) * 1000.;
    printf("\nComparison completed in: ");
    printf("%f", matching_diff_ms);
    printf(" ms");

    return 0;
}