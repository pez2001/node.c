/* 
 * node.c - c based data tree with various imports & exports
 * (c) 2013 by Tim Theede aka Pez2001 <pez2001@voyagerproject.de> / vp
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * THIS SOFTWARE IS SUPPLIED AS IT IS WITHOUT ANY WARRANTY!
 *
 */


#ifndef NYX_IMPORT_H
#define NYX_IMPORT_H

#include "../node.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"

#ifdef USE_MEMORY_DEBUGGING
#include "../memory.h"
#endif

#define NYX_STATE_IN_NOTHING 0
#define NYX_STATE_IN_VALUE 1
#define NYX_STATE_IN_STRING 2
#define NYX_STATE_IN_COMMENT 8
#define NYX_STATE_IN_OP 4
#define NYX_STATE_IN_SINGLE_QUOTE_STRING 32


/**
 * nyx_Load() - Load nyx ast from string
 * @json: nyx formatted input string
 * @len: Length of input string
 *
 * Creates a node tree from the nyx formatted input string
 * Note: string does not need to be terminated
 *
 * Return: Node tree or NULL if failed
 */
node *nyx_Load(char *NYX,unsigned long len); 

/**
 * nyx_LoadFile() - Load nyx ast from a file
 * @filename: Filename of nyx formatted input file
 *
 * Creates a node tree from the nyx formatted input file
 *
 * Return: Node tree or NULL if failed
 */
node *nyx_LoadFile(char *filename); 

/**
 * nyx_LoadString() - Load nyx ast from string
 * @content: nyx formatted input string
 *
 * Creates a node tree from the nyx formatted input string
 *
 * Return: Node tree or NULL if failed
 */
node *nyx_LoadString(char *content); 

#endif