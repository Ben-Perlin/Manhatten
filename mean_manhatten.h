#include <stdbool.h>
#include <stdint.h>

size_t abs_diff(size_t a, size_t b) {
    return (a >= b) ? a - b : b - a;
}
typedef struct Matrix {
    size_t nrow;
    size_t ncol;
    bool *data;
} Matrix;

double mean_manhatten(const Matrx * const matrix) {
    size_t nrow = Matrix->nrow;
    size_t ncol = Matrix->ncol;
    bool * data = Matrix->data;
    
    size_t count = 0;
    size_t dist_accum = 0;

    for (size_t from_row = 0; from_row < nrow; from_row++) {
        for (size_t from_col = 0; from_col < ncol; from_col ++) {
            if (!data[from_row*ncol + from_col]) continue;

            /* Finish Current Row */
            for (size_t to_col = from_col +1 ; to_col < ncol; to_col++) {
                // to_col = from_row
                if (data[from_row*ncol + to_col]) {
                    // invariant: to_col > from_col
                    ++count;
                    dist_accum += (to_col - from_col);
                }
            }

            // continue rest of rows
            for (size_t to_row = from_row + 1; to_row < nrow; to_row++) {
                for (size_t to_col = 0; to_col < ncol; to_col++) {
                    if(data[to_row*ncol + to_col]) {
                        // invariant: to_row > from_row
                        count++;
                        dist_accum += (to_row - from_row) + abs_diff(to_col, from_col);
                    }
                }
            }
        }
    }

    return ((double) dist_accum)/((double) count);
}
