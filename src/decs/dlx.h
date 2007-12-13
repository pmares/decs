/**
 * libdlx - Exact cover solver library using the Dancing Links algorithm.
 * Copyright (C) 2007  Jan Magne Tjensvold
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#ifndef DLX_H_
#define DLX_H_

#include "type.h"
#include "sbmatrix.h"

const uint MAX_LEVELS = 128;

int dlx_solve(SBMatrix* matrix);
void dlx_set_verbose_level(uint level);
void dlx_set_heuristic(bool value);
bool dlx_get_heuristic();
uint dlx_get_updates();
uint dlx_get_max_level();
uint dlx_get_profile(uint level);
uint dlx_count_solutions();
uint dlx_count_columns();
bool dlx_working();


#endif /*DLX_H_*/
