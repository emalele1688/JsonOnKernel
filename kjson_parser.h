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

#ifndef _KJSON_PARSER_H
#define _KJSON_PARSER_H

#include <linux/types.h>
#include "kjstring.h"

struct kjson_container;

extern struct kjson_container *kjson_parse(const struct kjstring_t *json_str);

#endif
