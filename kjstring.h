// SPDX-License-Identifier: GPL-2.0

/* This file is part of JsonOnKernel.
 *
 * JsonOnKernel is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * JsonOnKernel is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nome-Programma.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2023, Emanuele Santini
 *
 * Authors: Emanuele Santini <emanuele.santini.88@gmail.com>
*/

#ifndef _KJSTRING_H
#define _KJSTRING_H

#ifdef __KERNEL__

#include <linux/string.h>
#include <linux/slab.h>

#define kjstring_allocator(size) kmalloc(size, GFP_KERNEL)
#define kjstring_free(str) kfree(str)

#else

#include <stdlib.h>
#include <string.h>

#define kjstring_allocator(size) malloc(size)

#endif

struct kjstring_t {
    size_t buffer_size;
    size_t off;

    /*
     * Data could be allocated in another place or embedded in the current struct.
     * In this last case, str_data points to __data (so it points the field just after it self)
     */
    char *str_data;
    char __data[];
};

struct kjstring_iterator {
    const struct kjstring_t *str;
    size_t pos;
};

#define kjstring_str(str) (str)->str_data
#define kjstring_size(str) (str)->off

#define INTOS_SIZE  16  // Size of buffer for integer to string convertion

#define kjstring_static_declare(name, size)                                 \
    struct kjstring_static_##name {                                         \
        size_t buffer_size;                                                 \
        size_t off;                                                         \
        char *str_data;                                                     \
        char __data[size];                                                  \
    } _##name;                                                              \
    struct kjstring_t *name = (struct kjstring_t*)&_##name;                 \
    name->buffer_size = size; name->off = 0; name->str_data = name->__data; \
    memset(name->str_data, 0x0, size)

static inline struct kjstring_t* kjstring_new_string_buffer(struct kjstring_t *str, char *buffer, size_t buffer_size)
{
    if(!str || !buffer)
        return NULL;

    str->buffer_size = buffer_size;
    str->off = strnlen(buffer, buffer_size);
    str->str_data = buffer;

    return str;
}

static inline struct kjstring_t *kjstring_alloc(size_t default_size)
{
    struct kjstring_t *str;

    if(!default_size || (str = kjstring_allocator(sizeof(struct kjstring_t) + default_size)) == NULL)
        return NULL;

    memset(str, 0, sizeof(struct kjstring_t) + default_size);
    str->buffer_size = default_size;
    str->str_data = str->__data;

    return str;
}

#define kjstring_clear(str) do  {                   \
    str->off = 0;                                   \
    memset(str->str_data, 0x0, str->buffer_size);   \
} while(0)

#define kjstring_append_pos(str, src, pos) do {                                 \
    if(pos < str->buffer_size) {                                                \
        size_t len = strlen(src);                                               \
        strncpy(&str->str_data[pos], src, (str->buffer_size - pos));            \
        if((str->buffer_size - pos) <= len) {                                   \
            str->off = str->buffer_size - 1;                                    \
            str->str_data[str->off] = '\0';                                     \
        }                                                                       \
        else {                                                                  \
            str->off = pos + len;                                               \
        }                                                                       \
    }                                                                           \
} while(0)

static inline void kjstring_append_string(struct kjstring_t *str, char *src, size_t pos) {
    kjstring_append_pos(str, src, pos);
}

static inline void kjstring_append_integer(struct kjstring_t *str, int64_t val, size_t pos) {
    char integer[INTOS_SIZE];

    memset(integer, 0, INTOS_SIZE);
    snprintf(integer, INTOS_SIZE, "%lld", val);
    kjstring_append_pos(str, integer, pos);
}

// Type to put on the string is not recognized
static inline void __kjstring_no_append(struct kjstring_t *str, int64_t val, size_t pos) {}

#define kjstring_insert_type(str, src, pos) _Generic((src), \
    char*: kjstring_append_string,                          \
    int64_t: kjstring_append_integer,                       \
    default: __kjstring_no_append                           \
    )(str, src, pos)

#define kjstring_append(str, src) kjstring_insert_type(str, src, str->off)
#define kjstring_trunc(str, src) kjstring_insert_type(str, src, 0)

#define kjstring_push(str, chr) do {                    \
    if(str->off < str->buffer_size - 1)                 \
        str->str_data[str->off++] = chr;                \
} while(0)

static inline void kjstring_interator_init(const struct kjstring_t *str, struct kjstring_iterator *iterator)
{
    iterator->str = str;
    iterator->pos = 0;
}

#define kjstring_iterator_reset(iterator) (iterator)->pos = 0

#define kjstring_copy_iterator(iterator_src, iterator_dest) do {    \
    (iterator_dest)->str = (iterator_src)->str;                     \
    (iterator_dest)->pos = (iterator_src)->pos;                     \
} while(0)

// TODO convert in a inline function
#define kjstring_iterator_next(iterator, chr) do {          \
    chr = '\0';                                             \
    if((iterator)->pos < (iterator)->str->off)              \
        chr = (iterator)->str->str_data[(iterator)->pos++]; \
} while(0)

static inline char kjstring_iterator_get(struct kjstring_iterator *iterator)
{
    char ret = '\0';

    if(!iterator && !iterator->str)
        return ret;

    if(iterator->pos < iterator->str->off)
        ret = iterator->str->str_data[iterator->pos];

    return ret;
}

static inline bool kjstring_iterator_end(struct kjstring_iterator *iterator)
{
    return iterator->pos == iterator->str->off;
}

#define kjstring_for_each(iterator, chr)                    \
    for(chr = (iterator)->str->str_data[(iterator)->pos] ;  \
        (iterator)->pos < (iterator)->str->off ;            \
        chr = (iterator)->str->str_data[++(iterator)->pos])

#endif
