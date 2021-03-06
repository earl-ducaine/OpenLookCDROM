/* FILE pagelist.h *****************************************************
 *
 * xplan - project planning tool
 * Copyright (C) 1992 Brian Gaubert, Mark M. Lacey, Richard Malingkas,
 * and Mike Marlow.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License (distributed with this program in the file
 * COPYING) for more details.
 * 
 * If you did not received a copy of the GNU General Public License
 * along with this program, write to the Free Software Foundation,
 * Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 * Since this was a project for a one semester software engineering
 * course, the authors will not be offering support for the product
 * after its release.
 *
 * FILE pagelist.h */

#ifndef _pagelist_h_
#define _pagelist_h_

struct object_node {
   /* PERT_BOX for PERT box, PERT_LINE for dependency line, GANTT_BAR */
   /* for Gantt bar */
   int object_type;

   char *name;

   int x, y;

   int length;

   int dx, dy;

   int continued_from_previous;

   int continued_on_next;

   int critical;

   struct object_node *next;
};

struct page_list {
   int pages_across, pages_down;

   struct object_node **pages;
};

#define PERT_BOX  0
#define PERT_LINE 1
#define GANTT_BAR 2

struct page_list *create_page_list(int, int);
struct object_node *get_page_from_list(struct page_list *, int, int);
void add_object_to_page(struct page_list *, int, int, struct
			object_node *);
struct object_node *create_object_node(int, int, int, int, int, int,
				       int, int, int, char *, struct
				       object_node *);


#endif

