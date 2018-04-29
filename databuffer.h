/*
 */

#ifndef DATABUFFER_H
#define DATABUFFER_H

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

//TODO apply const

// or instead read as blocks to linked list and cat
typedef struct _DataBufferNode {
    _DataBufferNode *next;
    size_t nEntries;
    void *contents;
} _DataBufferNode;

typedef struct DataBuffer {
    _DataBufferNode *head;
    _DataBufferNode *tail;
    size_t datatypeSize;
    size_t chunkSize;
} DataBuffer;

_DataBufferNode _DataBufferNode_create(size_t datatypeSize, size_t chunkSize) {
    //REQUIRES(don't overflow size)
    _DataBufferNode *node = calloc(sizeof(_DataBufferNode), 1);

    if (node == NULL) {return node;}

    node->contents = malloc(chunkSize*datatypeSize);
    if (node->contents == NULL) {
        free(node);
        return NULL;
    }

    return node;
}

void _DataBufferNode_free(_DataBufferNode *node) {
    assert(node != NULL);

    if (node->contents != NULL) free(node->contents);
    free(node);
}

/*
 * datatypeSize: size of datatype in bytes
 * chunkSize: number of datatype values per node
 */
DataBuffer *DataBuffer_create(size_t datatypeSize, size_t chunkSize) {
    DataBuffer *buffer = calloc(sizeof(DataBuffer), 1);

    if (buffer == NULL) return NULL;

    buffer->datatypeSize = datatypeSize;
    buffer->chunkSize = chunkSize;

    buffer->tail = _DataBufferNode_create(buffer->datatypeSize, buffer->chunkSize);

    /* check validity of allocation and clean up if failed */
    if (buffer->head == 0) {
        free(buffer);
        return NULL;
    }

    buffer->tail = buffer->head;

    return buffer;
}

/*
 * Frees until null pointer in linked list
 */
void DataBuffer_free(DataBuffer *buffer) {
    assert(buffer != NULL);

    _DataBufferNode *node = buffer->head;
    while (node != NULL) {
        _DataBufferNode *next = node->next;
        _DataBufferNode_free(node);
        node = next;
    }

    free(buffer);
}

/*
 * return DataBuffer after running, if allocation fails it is deleted
 */
DataBuffer* DataBuffer_append(DataBuffer *buffer, void *value) {
    assert(buffer != NULL);
    assert(value != NULL);

    if (buffer->tail->nEntries == buffer->chunkSize) {
        buffer->tail->next = _DataBufferNode_create(buffer->datatypeSize, buffer->chunkSize);

        if (buffer->tail->next == NULL) { // Failed allocation
            DataBuffer_free(buffer); // cleanup buffer
            return NULL;
        }

        buffer->tail = buffer->tail->next;
    }

    memcpy(buffer->tail->contents + (++(buffer->tail->nEntries) * buffer->datatypeSize),
        value, buffer->datatypeSize);

    return buffer;
}

size_t dataBuffer_length(DataBuffer *buffer) {
    assert(buffer != NULL);

    size_t length = 0;

    for (_DataBufferNode *node = buffer->head; node != NULL; node = node->next) {
        length += node->nEntries;
    }

    return length;
}

/*
 * create a contiguous array with values from databuffer
 */
void* DataBuffer_collapse(DataBuffer *buffer) {
    assert(buffer != 0);

    void *output = malloc(dataBufferLength(buffer) * buffer->datatypeSize);
    if (output == NULL) {return NULL;}

    void *outputDest = output;

    for (_DataBufferNode *node = buffer->head; node != NULL; node = node->next) {
        memcpy(outputDest, node->contents, node->nEntries * buffer->datatypeSize);
        outputDest += node->nEntries * buffer->datatypeSize;
    }

    return output;
}
