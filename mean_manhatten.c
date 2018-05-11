/* Command Line interface for mean Manhatten Script
 * Written by Ben Perlin - 2018
 */

/* format
 * chars '0', '1', ',', '\n', ' ', '\t' only
 * must start on first line, no empty lines, finish with last row and then newline EOF
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "buffered_input.h"
#include "databuffer.h"
#include "mean_manhatten.h"

#define INPUT_BUFFER_SIZE 1024LL

// starting data buffer size
#define DATA_BUFFER_CHUNKSIZE 1024LL

Matrix * readMatrix(FILE * file) {
    BufferedInput * inputBuffer = BufferedInput_create(file, INPUT_BUFFER_SIZE);
    if (inputBuffer == NULL) return NULL;

    DataBuffer *buffer = DataBuffer_create(sizeof(bool), DATA_BUFFER_CHUNKSIZE);
    if (buffer == NULL) {
        free(inputBuffer);
        return NULL;
    }

    size_t nrow = 0;
    size_t ncol = 0;
    size_t col = 0;

    bool canStartRow = true;
    bool seperatorExpected = false;

    char * msg = "unspecified error";

    int c = EOF;
    // read from file
    while ((c = BufferedInput_fgetc(inputBuffer)) != EOF) {
        switch (c) {
        case ' ':
        case '\t':
            break;

        case '0':
        case '1':
            if (seperatorExpected) {
                msg = "Seperator Expected";
                goto error;
            }

            if (col == 0) {
                nrow++; // begin reading row
            }
            bool value = (c == '1');
            DataBuffer_append(buffer, &value);
            seperatorExpected = true;
            ++col;

            if (nrow == 1) {
                ncol = col;
            }

            break;

        case ',':
            if (seperatorExpected) {
                seperatorExpected = false;
            } else {
                msg = "unexpected seperator found";
                goto error;
            }
            break;

        // end newline optional
        case '\n':
            if (!seperatorExpected) {
                msg = "unexpected newline";
                goto error;
            }

            seperatorExpected = false;

            if (col < ncol) {
                msg = "too few columns found";
                goto error;
            } else if (col > ncol) {
                msg = "too many columns found";
                goto error;
            }

            col = 0;
            break;

        case '\r':
            break; // drop windows carrage return for consistancy

        default:
            msg = "unexpected symbol found";
            goto error;
        }
    }

    free(inputBuffer);
    bool *outputData = (bool *) DataBuffer_collapse_and_free(buffer);
    if (outputData == NULL) {return NULL;}

    return Matrix_create(nrow, ncol, outputData);

error:
    fprintf(stderr, "Error parsing matrix (row %ld, col %ld)%s\n", nrow, col, msg);
    DataBuffer_free(buffer);
    free(inputBuffer);
    return NULL;
}

int main(int argc, char** argv) {
FILE * sourceFile = NULL;

    if (argc == 2) {
        sourceFile = fopen(argv[1], "r");

        if (sourceFile == NULL) {
            fprintf(stderr, "ERROR: Failed to open file: \"%s\"\n", argv[1]);
            return 1;
        }
    } else {
        fputs("USAGE: mean_manhatten [filename]\n", stderr);
        return 1;
    }


    Matrix *matrix = readMatrix(sourceFile);
    if (sourceFile != stdin) fclose(sourceFile);
    if (matrix == NULL) {
        fputs("ERROR: failed to parse matrix", stderr);
        return 1;
    }
    double mm_val = mean_manhatten(matrix);
    printf("%f\n", mm_val);
    Matrix_free(matrix);

    return 0;
}
