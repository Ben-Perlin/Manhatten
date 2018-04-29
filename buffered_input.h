#ifndef BUFFERED_FGETC_H
#define BUFFERED_FGETC_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct BufferedInput {
    FILE * file;
    size_t buffer_capacity;
    size_t buffer_length;
    size_t buffer_index;
    bool EOF_read;
    char buffer[];
} BufferedInput;

BufferedInput* BufferedInput_create(File * file, size_t buffer_size) {
    BufferedInput *inputBuffer = malloc(offsetof(BufferedInput, buffer) + buffer_size*sizeof(char));
    inputBuffer->file = file;
    BufferedInput->buffer_capacity = buffer_size;
    BufferedInput->buffer_length = 0L;
    BufferedInput->buffer_index = 0L;
    BufferedInput->EOF_read = false;
}

// drops carrage returns if encountered
// abort on read error
int BufferedInput_fgetc(BufferedInput *buffer) {
    assert(buffer != 0);
    //TODO

    
}

#endif /* BUFFERED_FGETC_H */
