/*********************************************************************
 * buffered_input.h - buffered version of fgetc                      *
 * Written by Ben Perlin - 2018                                      *
 * License: GNU GPL 3.0                                              *
 ********************************************************************/

#ifndef __BUFFERED_FGETC_H__
#define __BUFFERED_FGETC_H__

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

BufferedInput* BufferedInput_create(FILE * file, size_t buffer_size) {
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
int BufferedInput_fgetc(BufferedInput * const buffer) {
    assert(buffer != 0);
    assert(buffer->index <= buffer->length);

    if (buffer->index < buffer->length) {
        return buffer->buffer[(buffer->index)++];

    } else if (buffer->index == buffer->length) {
        if (buffer->EOF_read) {
            return EOF;
        } else { // into buffer
            buffer->length = fread(buffer->buffer, sizeof(char), buffer->capacity, buffer->file);
            buffer->index = 0;

            if (buffer->length < buffer->capacity) {
                if (ferror(buffer->file)) {
                    fputs("ERROR: error reading from input file", stderr);
                    abort(); // TODO implement better error handling
                }

                if (feof(buffer->file)) {
                    buffer->EOF_read = true;
                }
            }

            return BufferedInput_fgetc(buffer); // recurse 1 level to try again
        }
    }

    assert(0);
}

#endif /* __BUFFERED_FGETC_H__ */
