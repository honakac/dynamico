/*
 * Dynamico - A lightweight library for flexible dynamic arrays.
 * Copyright (C) 2025 Andrey Stekolnikov <honakac@yandex.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VECTOR_PAGE_SIZE 32

#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"

static int VECTOR_FAILURE(const char *message)
{
    printf("dynamico: Error: %s\n", message);
    exit(1);
    return 0;
}


#define vector(Type, Name) struct { \
        size_t size;  \
        size_t pos;   \
        unsigned char type_size; \
        Type* buffer; \
    } Name = {0, 0, sizeof(Type), NULL}

#define vector_size(vector) (vector.pos / str.type_size)

#define vector_init(vector) \
    { \
        vector.size = VECTOR_PAGE_SIZE; \
        vector.pos = 0; \
        vector.buffer = malloc(vector.size * sizeof(vector.type_size)); \
        memset(vector.buffer, 0, vector.size * sizeof(vector.type_size)); \
    }

#define vector_push_back(vector, value) \
    { \
        if (vector.pos >= vector.size - 2) \
            vector.buffer = realloc(vector.buffer, (vector.size += VECTOR_PAGE_SIZE) * sizeof(vector.type_size)); \
        vector.buffer[vector.pos++] = value; \
        vector.buffer[vector.pos] = 0; \
    }

#define vector_pop_back(vector) \
    { \
        if (vector.pos <= vector.size - VECTOR_PAGE_SIZE) \
            vector.buffer = realloc(vector.buffer, (vector.size -= VECTOR_PAGE_SIZE) * sizeof(vector.type_size)); \
        vector.buffer[--(vector.pos)] = 0; \
    }

#define vector_clear(vector) \
    { \
        free(vector.buffer); \
        vector.size = 0; \
        vector.pos = 0; \
        vector.buffer = NULL; \
    }

#define vector_fill(vector, value, size) \
    { \
        vector_clear(vector); \
        vector_init(vector); \
        for (size_t i = 0; i < (size_t) size; i++) { \
            vector_push_back(vector, value); \
        } \
    }

#define vector_at(vector, type, index) ((size_t)index >= vector.pos ? (type)VECTOR_FAILURE("Index out of range!") : (type)vector.buffer[index])

// Pre-defined types:
// string:

#define string(Name) vector(char, Name)
#define string_init(Name) vector_init(Name)
#define string_size(Name) vector_size(Name)
#define string_clear(Name) vector_clear(Name)
#define string_pop(Name) vector_pop_back(Name)
#define string_push(Name, value) vector_push_back(Name, value)
#define string_at(Name, Type, Index) vector_at(Name, Type, Index)
#define string_push_string(Name, value) \
    { \
        for (char *ptr = value; *ptr; ptr++) { \
            vector_push_back(Name, *ptr); \
        } \
    }
