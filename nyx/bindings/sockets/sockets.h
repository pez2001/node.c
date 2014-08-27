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

#ifndef BINDING_SOCKETS_H
#define BINDING_SOCKETS_H

#include "../../nyx.h"


#ifdef USE_SOCKETS

#ifdef WIN32
#define _WIN32_WINNT  0x501 
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#endif

#include <stdio.h>

void sockets_binding_open(node *state);
void sockets_binding_close(node *state);

node *sockets_bind(node *class);
node *sockets_create_class_object(void);
node *sockets_open(node *state,node *obj,node *block,node *parameters);
//node *sockets_bind(node *state,node *obj,node *block,node *parameters);
node *sockets_connect(node *state,node *obj,node *block,node *parameters);
node *sockets_close(node *state,node *obj,node *block,node *parameters);
node *sockets_set_address(node *state,node *obj,node *block,node *parameters);
//node *sockets_is_connected(node *state,node *obj,node *block,node *parameters);
node *sockets_read(node *state,node *obj,node *block,node *parameters);
node *sockets_write(node *state,node *obj,node *block,node *parameters);
#endif

#endif
