/*
    The MIT License (MIT)

    Copyright (c) 2025 Andrey Stekolnikov

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#pragma once

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define VECTOR_VERSION "v1.0"
#define VECTOR_PAGE_COUNT 4
#define VECTOR_INDEX_NULL (-1)

typedef struct {
    void *buffer;
    size_t pos;
    size_t size;
    size_t typeSize;
} Vector;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void *_vector_error(const char *msg)
{
    fprintf(stderr, "dynamico: %s\n", msg);
    exit(EXIT_FAILURE);
    return NULL;
}
static void *_vector_malloc(size_t count)
{
    void *buffer = malloc(count);
    if (!buffer)
        _vector_error("Failed to allocate memory");

    return buffer;
}
static void *_vector_realloc(void *src, size_t count)
{
    void *buffer = realloc(src, count);
    if (!buffer)
        _vector_error("Failed to reallocate memory");

    return buffer;
}
static void _vector_check_overflow(Vector *vec)
{
    if (vec->pos + vec->typeSize >= vec->size) {
        vec->buffer = _vector_realloc(vec->buffer, (vec->size += VECTOR_PAGE_COUNT * vec->typeSize));
        memset((char*) vec->buffer + vec->pos, 0, vec->size - vec->pos);
    }
}
static void *_vector_search(Vector *vec, void *src)
{
    for (size_t i = 0; i < vec->pos / vec->typeSize; i++) {
        if (memcmp((char*) vec->buffer + i * vec->typeSize, src, vec->typeSize) == 0)
            return (char*) vec->buffer + i * vec->typeSize;
    }
    return NULL;
}
#define vector_unsafe_at(vec, type, index) *((type*) ((char*) (vec)->buffer + (index) * (vec)->typeSize))

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Get pointer to the buffer of the vector.
 * 
 */
#define vector_buffer(vec, type) ((type*) (vec)->buffer)
/**
 * @brief Get the element at the specified index.
 * 
 */
#define vector_at(vec, type, index) (((vec)->pos > index * (vec)->typeSize) ? vector_unsafe_at(vec, type, index) \
                                                                            : *((type*) _vector_error("Index out of range")))
#define vector_size(vec) ((vec)->pos / (vec)->typeSize))

/**
 * @brief Resize the vector to the specified size.
 * 
 * @param vec Pointer to the vector.
 * @param newSize New size of the vector.
 */
static void vector_resize(Vector *vec, size_t newSize)
{
    size_t oldSize = vec->size / vec->typeSize;
    vec->size = newSize * vec->typeSize;
    vec->buffer = _vector_realloc(vec->buffer, vec->size);

    if (oldSize > newSize)
        memset((char*) vec->buffer + oldSize * vec->typeSize, 0, (vec->size - oldSize) * vec->typeSize);
}

/**
 * @brief Push an element to the end of the vector.
 * 
 * @param vec Pointer to the vector.
 * @param src Value/Pointer to the value to push.
 */
static void vector_push(Vector *vec, void *src)
{
    // vector_resize(vec, vec->pos / vec->typeSize + 1);
    _vector_check_overflow(vec);
    memcpy((char*) vec->buffer + vec->pos, src, vec->typeSize);
    vec->pos += vec->typeSize;
}

/**
 * @brief Push multiple elements to the end of the vector.
 * 
 * @param vec Pointer to the vector.
 * @param src Value/Pointer to the array of values to push.
 * @param count Number of elements to push.
 */
static void vector_push_multiple(Vector *vec, void *src, size_t count)
{
    for (size_t i = 0; i < count; i++)
        vector_push(vec, (char*) src + i * vec->typeSize);
}

/**
 * @brief Search and get the element using keyword.
 * 
 */
#define vector_search(vec, type, src) ((type*) _vector_search(vec, src))

/**
 * @brief Remove the element at the specified index.
 * 
 * @param vec Pointer to the vector.
 * @param index Index of the element to remove.
 */
static void vector_remove(Vector *vec, size_t index)
{
    if (index >= vec->pos / vec->typeSize)
        _vector_error("Index out of range");

    memmove((char*) vec->buffer + index * vec->typeSize,
            (char*) vec->buffer + (index + 1) * vec->typeSize,
            vec->pos - (index + 1) * vec->typeSize);
    vec->pos -= vec->typeSize;
    memset((char*) vec->buffer + vec->pos, 0, vec->typeSize);

    if (vec->pos < vec->size - vec->typeSize * VECTOR_PAGE_COUNT) {
        vec->size = vec->pos + vec->typeSize * VECTOR_PAGE_COUNT;
        vec->buffer = _vector_realloc(vec->buffer, vec->size);
    }
}

/**
 * @brief Remove a range of elements from the vector.
 * 
 * @param vec Pointer to the vector.
 * @param min Index of the first element to remove.
 * @param max Index of the last element to remove.
 */
static void vector_remove_range(Vector *vec, size_t min, size_t max) {
    for (size_t i = min; i < max; i++)
        vector_remove(vec, min);
}

/**
 * @brief Create a new vector with the specified type size.
 * 
 * @param type_size Type size of the element in the vector.
 * @return Vector
 */
static Vector vector_create(size_t type_size)
{
    Vector vec = {
        .buffer=_vector_malloc(VECTOR_PAGE_COUNT * type_size),
        .pos=0,
        .size=VECTOR_PAGE_COUNT * type_size,
        .typeSize=type_size
    };
    memset(vec.buffer, 0, vec.size);
    return vec;
}

/**
 * @brief Free the vector and its allocated memory.
 * 
 * @param vec Pointer to the vector.
 */
static void vector_free(Vector *vec)
{
    free(vec->buffer);
    vec->pos = 0;
    vec->size = 0;
}

/**
 * @brief Clear the vector and reset its size to zero.
 * 
 * @param vec Pointer to the vector.
 */
static void vector_clear(Vector *vec)
{
    size_t type_size = vec->typeSize;

    vector_free(vec);
    *vec = vector_create(type_size);
}