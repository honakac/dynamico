#pragma once

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#define vector_convert(TYPE, SRC) ((TYPE*) (SRC))
#define vector_convert_element(TYPE, SRC) *(vector_convert(TYPE, SRC))

#define VECTOR_PAGE_COUNT 8
#define VECTOR_INDEX_NULL (-1)

typedef struct {
    void *buffer;
    size_t pos;
    size_t size;
    size_t typeSize;
} Vector;

static void *vector_malloc(size_t count)
{
    void *buffer = malloc(count);
    if (!buffer)
        perror("malloc");

    return buffer;
}
static void *vector_realloc(void *src, size_t count)
{
    void *buffer = realloc(src, count);
    if (!buffer)
        perror("realloc");

    return buffer;
}
static void vector_check_overflow(Vector *vec)
{
    if (vec->pos + vec->typeSize >= vec->size) {
        vec->buffer = vector_realloc(vec->buffer, (vec->size += VECTOR_PAGE_COUNT * vec->typeSize));
        memset((char*) vec->buffer + vec->pos, 0, vec->size - vec->pos);
    }
}

static void vector_push(Vector *vec, void *src)
{
    vector_check_overflow(vec);
    memcpy((char*) vec->buffer + vec->pos, src, vec->typeSize);
    vec->pos += vec->typeSize;
}

static void vector_push_multiple(Vector *vec, void *src, size_t count)
{
    for (size_t i = 0; i < count; i++)
        vector_push(vec, (char*) src + i * vec->typeSize);
}

static void *vector_search(Vector *vec, void *src)
{
    for (size_t i = 0; i < vec->pos / vec->typeSize; i++) {
        if (memcmp((char*) vec->buffer + i * vec->typeSize, src, vec->typeSize) == 0)
            return (char*) vec->buffer + i * vec->typeSize;
    }
    return NULL;
}

static void *vector_at(Vector *vec, size_t index)
{
    if (index >= vec->pos / vec->typeSize)
        return NULL;

    return (char*) vec->buffer + index * vec->typeSize;
}

static Vector vector_create(size_t type_size)
{
    Vector vec = {
        vector_malloc(VECTOR_PAGE_COUNT * type_size),
        0,
        VECTOR_PAGE_COUNT * type_size,
        type_size
    };
    memset(vec.buffer, 0, vec.size);
    return vec;
}

static void vector_free(Vector *vec)
{
    free(vec->buffer);
    vec->pos = 0;
    vec->size = 0;
}

static void vector_clear(Vector *vec)
{
    size_t type_size = vec->typeSize;

    vector_free(vec);
    *vec = vector_create(type_size);
}