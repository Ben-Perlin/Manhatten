/* Command Line interface for mean Manhatten Script */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "buffered_input.h"
#include "databuffer.h"
#include "mean_manhatten.h"

#define INPUT_BUFFER_SIZE 1024

// starting data buffer size
#define DATA_BUFFER_CHUNKSIZE (1<<10)

Matrix * readMatrix(FILE * file) {
    char *inputBuffer = BufferedInput_create(INPUT_BUFFER_SIZE);
    if (inputBuffer == NULL) return NULL;

    DataBuffer *buffer = DataBuffer_create(sizeof(bool), DATA_BUFFER_CHUNKSIZE);
    if (buffer == NULL) {
        free(inputBuffer);
        return NULL;
    }

    size_t nrow = 0;
    size_t ncol = 0;
    size_t row = 0;
    size_t col = 0;

    bool canStartRow = true;
    bool seperatorExpected = false;

    // read from file
    while ((int c = BufferedInput_fgetc(inputBuffer)) != EOF) {
        switch (c) {
        case ' ':
        case '\t':
            // discard
            break;

        case '0':

            break;

        case '1':

            break;

        case ',':

            break;

        case '\n':
            if (nrow != 0) {

            }

            break;

        default:

        }
    }

    bool *outputData = (bool *) DataBuffer_collapse(buffer);
    DataBuffer_free(buffer);
    free(inputBuffer);

    if (outputData == NULL) {return NULL;}

    Matrix *output = malloc(sizeof(Matrix));
    if (output = NULL) {
        free(outputData);
        return NULL;
    }

    output->nrow = nrow;
    output->ncol = ncol;
    output->data = outputData;
    return output;
}

int main(char** argv, int argc) {
FILE * sourceFile = NULL;

    if (argc > 1) {
        if (argc > 2) {
            fputs("USAGE: mean_manhatten [filename]\n", stderr);
            return 1;
        }

        sourceFile = fopen(argv[1], "r");

        if (sourceFile == NULL) {
            fprintf(stderr, "ERROR: Failed to open file: \"%s\"\n", argv[1]);
            return 1;
        }
    } else if (isatty(fileno(stdin))) {
        sourceFile = stdin;
    } else {
        fputs("ERROR: No imput available", stderr)
    }

    Matrix *matrix = readMatrix(sourceFile);
    if (sourceFile != stdin) fclose(sourceFile);
    if (matrix == NULL) return 1;
    double mm_val = mean_manhatten(matrix);
    printf("%f\n", mm_val);

    return 0;
}
