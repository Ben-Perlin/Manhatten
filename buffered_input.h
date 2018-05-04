#ifndef BUFFERED_FGETC_H
#define BUFFERED_FGETC_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct BufferedInput {
    FILE * file;
    size_t capacity;
    size_t length;
    size_t index;
    bool EOF_read;
    char buffer[];
} BufferedInput;

BufferedInput* BufferedInput_create(File * file, size_t buffer_size) {
    BufferedInput *buffer = malloc(offsetof(BufferedInput, buffer) + buffer_size*sizeof(char));
    buffer->file = file;
    buffer->capacity = buffer_size;
    buffer->length = 0L;
    buffer->index = 0L;
    buffer->EOF_read = false;

    return buffer;
}

// drops carrage returns if encountered
// abort on read error
int BufferedInput_fgetc(BufferedInput *buffer) {
    assert(buffer != 0);
    assert(buffer->index <= buffer->length);

    if (buffer->index < buffer->length) {
        return buffer[++buffer->index];

    } else if (buffer->index == buffer->length) {
        if (buffer->EOF_read) {
            return EOF;
        } else { // into buffer
            buffer->length = fread(buffer->buffer, sizeof(char), buffer->buffer, buffer->file);
            buffer->index = 0;

            if (buffer->length < buffer->capacity) {
                if (ferror()) {
                    fputs("ERROR: error reading from input file", stderr);
                    abort(); // TODO implement better error handling
                }

                if (feof()) {
                    buffer->EOF_read = true;
                }
            }

            return BufferedInput_fgetc(buffer); // recurse 1 level to try again
        }
    }
}

#endif /* BUFFERED_FGETC_H */
