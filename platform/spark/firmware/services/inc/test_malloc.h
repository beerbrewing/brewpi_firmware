/*
 * Copyright (c) 2016 Particle Industries, Inc.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdlib.h>

#ifndef UNIT_TEST

#define t_malloc malloc
#define t_calloc calloc
#define t_realloc realloc
#define t_free free

#else

#ifdef __cplusplus
extern "C" {
#endif

void* t_malloc(size_t size);
void* t_calloc(size_t count, size_t size);
void* t_realloc(void* ptr, size_t size);
void t_free(void* ptr);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // defined(UNIT_TEST)
