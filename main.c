#include <stdbool.h>
#include <stdio.h>

#include "mean_manhatten.h"

const bool test0[] = {1, 1, 0, 0, 0, 0, 0, 1, 1};
const bool test1[] = {1, 1, 1, 0, 0, 0, 0, 1, 1};
const bool test2[] = {1, 0, 0, 0, 0, 0, 0, 0, 1};


int main(char** argv, int argc) {
    printf("Test 0 gives %f\n", mean_manhatten((const bool * const) &test0, 3, 3));
    printf("Test 1 gives %f\n", mean_manhatten((const bool * const) &test1, 3, 3));
    printf("Test 2 gives %f\n", mean_manhatten((const bool * const) &test2, 3, 3));
    return 0;
}
