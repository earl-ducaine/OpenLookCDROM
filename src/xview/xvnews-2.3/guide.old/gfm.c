/*
 * This file is a product of Sun Microsystems, Inc. and is provided for
 * unrestricted use provided that this legend is included on all tape
 * media and as a part of the software program in whole or part.  Users
 * may copy or modify this file without charge, but are not authorized to
 * license or distribute it to anyone else except as part of a product
 * or program developed by the user.
 *
 * THIS FILE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
 * WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 *
 * This file is provided with no support and without any obligation on the
 * part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 *
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY THIS FILE
 * OR ANY PART THEREOF.
 *
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even
 * if Sun has been advised of the possibility of such damages.
 *
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */

#ifndef lint
static char	sccsid[] = "@(#)gfm.c	2.25 91/08/14 Copyright 1990 Sun Microsystems";
#endif

/*
 * gfm.c - Notify and event callback function stubs.
 * This file was generated by `gxv' from `gfm.G'.
 */

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <xview/xview.h>
#include <xview/font.h>
#include <xview/defaults.h>
#include <xview/frame.h>
#include <xview/panel.h>
#include <xview/notify.h>
#include <X11/Xutil.h>
#include <group.h>
#include "gfm.h"

#ifdef __STDC__
extern char		*getcwd();
#else
extern char		*getwd();
#endif
extern void		expand_path();
extern Server_image	Gfm_document_glyph;

extern void		gfm_load_dir();
extern void		gfm_initialize_glyphs();

static int		Gfm_multiclick_timeout;
static void		gfm_notify_client();
static int		double_click();

static int              set_new_sizes();
extern char             *dgettext();
     
#define	STRCMP(s1, s2)	(strcmp(s1 ? s1 : "", s2 ? s2 : ""))

/*
 * Initialize the gfm popup window with a title.
 *
 * Arguments:
 *	ip		An instance pointer to a file chooser, NULL means
 *			create a new one from scratch
 *	owner		The XView window to parent the file chooser off of
 *	title		String containing initial frame title
 *
 * Returns:
 *	NULL if okay
 *	String containing error if error occurred
 */
gfm_popup_objects *
gfm_initialize(ip, owner, title)
	gfm_popup_objects	*ip;
	Xv_opaque		owner;
	char			*title;
{
	int		char_width;
	int		row_height;
	int		excess;
	int		min_width;
	int		min_height;
	long		supplied;
	Rect		*screen_rect;
	XSizeHints	wmhints;
	GFM_PRIVATE	*gfm_private;

	if (!ip && !(ip = gfm_popup_objects_initialize(ip, owner)))
		return NULL;

	/*
	 * Set up glyphs
	 */
	gfm_initialize_glyphs();

	xv_set(ip->controls, PANEL_DEFAULT_ITEM, ip->load, NULL);

	/*
	 * Allocate space for our private stash of data, fill it
	 * in with all the necessary information, then store it away
	 * in key data under the window.
	 */
	gfm_private = (GFM_PRIVATE *) calloc(1, sizeof(GFM_PRIVATE));
	gfm_private->height = (int)xv_get(ip->popup, XV_HEIGHT);
	gfm_private->width = (int)xv_get(ip->popup, XV_WIDTH);
	gfm_private->initial_win_height = gfm_private->height;
	gfm_private->initial_list_height = (int)xv_get(ip->list, XV_HEIGHT);
	gfm_private->initial_list_excess = gfm_private->initial_list_height -
		(xv_get(ip->list, PANEL_LIST_ROW_HEIGHT) *
		 xv_get(ip->list, PANEL_LIST_DISPLAY_ROWS));
	xv_set(ip->popup, XV_KEY_DATA, GFM_KEY, gfm_private, NULL);

	Gfm_multiclick_timeout =
		defaults_get_integer("openwindows.multiclicktimeout",
				     "OpenWindows.MultiClickTimeout", 4);

	/*
	 * Set the minimum allowable size for the window.  The minimum
	 * size for the list determines the overall minimum.  The smallest
	 * size is around 8 characters wide by 5 rows.  For the width we must
	 * take into account the width of the glyph and the border spacing.
	 */
	XGetWMNormalHints((Display *)xv_get(ip->popup, XV_DISPLAY),
			  (Window) xv_get(ip->popup, XV_XID),
			  &wmhints, &supplied);

	row_height = (int)xv_get(ip->list, PANEL_LIST_ROW_HEIGHT);
	excess = gfm_private->initial_list_height - 
		(xv_get(ip->list, PANEL_LIST_DISPLAY_ROWS) * row_height);
	min_height = xv_get(ip->controls, XV_HEIGHT) -
		gfm_private->initial_list_height +
			((6 * row_height) + excess);

	char_width = xv_get(xv_get(ip->controls, XV_FONT), FONT_DEFAULT_CHAR_WIDTH );
	excess = xv_get(xv_get(ip->list, PANEL_LIST_SCROLLBAR), XV_WIDTH);
	min_width = 56 + (8 * char_width) + excess;

	wmhints.flags |= PMinSize;
	wmhints.min_width = min_width;
	wmhints.min_height = min_height;
	XSetWMNormalHints((Display *)xv_get(ip->popup, XV_DISPLAY),
			  (Window) xv_get(ip->popup, XV_XID), &wmhints);

	/*
	 * Set the initial title, directory, and file
	 */
	xv_set(ip->popup, XV_LABEL, title, NULL);
	xv_set(ip->directory, PANEL_VALUE, "", NULL);
	xv_set(ip->file, PANEL_VALUE, "", NULL);

	/*
	 * Initially position in center of screen
	 * XXX - need to use new window positioning functions here!
	 */
	screen_rect = (Rect *)xv_get(ip->popup, WIN_SCREEN_RECT);
	xv_set(ip->popup,
	       XV_X, screen_rect->r_width/2 - gfm_private->width/2,
	       XV_Y, screen_rect->r_height/2 - gfm_private->height/2,
	       NULL);

	return ip;
}

/*
 * Show/don't show "." files
 */
void
gfm_show_dotfiles(ip, flag)
	gfm_popup_objects *ip;
	int		  flag;
{
	GFM_PRIVATE	  *gfm_private;

	gfm_private = (GFM_PRIVATE *) xv_get(ip->popup, XV_KEY_DATA, GFM_KEY);
	gfm_private->show_dotfiles = flag;
}

/*
 * Change the label on the action button
 */
void
gfm_set_action(ip, label)
	gfm_popup_objects *ip;
	char	*label;
{
	xv_set(ip->load, PANEL_LABEL_STRING, label, NULL);
	xv_set(ip->props_group, GROUP_LAYOUT, TRUE, NULL);
	panel_paint(ip->controls, PANEL_NO_CLEAR);
}

/*
 * Activate the file chooser
 *
 * gfm_activate is called from within a callback procedure written by the
 * client.  For example, the application might have a "Load..." button.
 * This button's notify handler would call gfm_activate to have the file
 * chooser appear when that button is pushed.
 *
 * Arguments:
 *
 *	ip		is an instance pointer to the particular file
 *			chooser that you would like to activate.  This
 *			is returned from gfm_initialize().
 *
 *	directory	is a string containing the directory that the client
 *			would like gfm to start up in.  If NULL is passed in
 *			the current working directory is used.  The current
 *			working directory is never changed even when the user
 * 			changes directories.
 *
 *	filter_pat	is a string containing a regular expression that will
 *			match the files the client program would like to show
 *			the user.  If NULL is passed int then all files are
 *			considered for the next test- the magic number stuff.
 *			Otherwise only those files that matched the regular
 *			expression will be considered to match the magic number
 *			test.
 *
 *	filter_callback	is a pointer to a function that will be called back
 *			for every file in a directory.  If the callbacks
 *			returns TRUE then the file will be included in 
 *			the list, return FALSE and the file will not be
 *			in the scrollng list.  If filter_pat is also
 *			specified, gfm will filter files against the filter
 *			pattern first, then call filter_callback for any
 *			remaining files.
 *
 *	callback	is a pointer to the procedure that should be called
 *			when the user pushes select or doubleclicks on the file
 *			that they wish to choose. callback is passed the file
 *			and directory that were selected.
 *			If no callback is passed in then NOTHING HAPPENS;
 *			gfm_activate will just return.
 *
 *	glyph		is a pointer to a Server_image.  This is only used in
 *			conjunction with the filters.  If there is a filter
 *			then all non-directories that match the tests will
 *			be shown with this glyph.  If no glyph is specified, the
 *			default is Gfm_document_glyph.
 *
 *	mode		is a flag to tell gfm what label it should use for its
 *			action button.
 *				GFM_LOAD sets the label to "Load"
 *				GFM_SAVE set the label to "Save"
 *				GFM_CREATE set the label to "Create"
 *				GFM_DEFAULT leaves the label alone
 */
void
gfm_activate(ip, directory, filter_pat, filter_callback, callback, glyph, mode)
	gfm_popup_objects	*ip;
	char			*directory;
	char			*filter_pat;
	int			(*filter_callback)();
	int			(*callback)();
	Xv_opaque		glyph;
	GFM_MODE		mode;
{
	char		*current_dir;
	char		new_dir[MAXPATHLEN + 1];
	GFM_PRIVATE	*gfm_private;

	/*
	 * Check to see if this is a valid file chooser
	 */
	if (!ip)
	{
		fprintf(stderr, dgettext("libguidexv_labels",
					 "gfm: no file chooser to activate\n"));
		return;
	}

	gfm_private = (GFM_PRIVATE *) xv_get(ip->popup, XV_KEY_DATA, GFM_KEY);

	current_dir = (char *)xv_get(ip->directory, PANEL_VALUE);

	if (directory && *directory)
		strcpy(new_dir, directory);
	else if (current_dir && *current_dir)
		strcpy(new_dir, current_dir);
	else
#ifdef __STDC__
		if (!getcwd(new_dir, MAXPATHLEN+1))
#else
		if (!getwd(new_dir))
#endif
		{
			fprintf(stderr, dgettext("libguidexv_labels",
				 "gfm: Couldn't read current directory, using \"/\" instead\n"));
			strcpy(new_dir, "/");
		}


	/*
	 * Check to see if filter pattern has changed.  If so,
	 * store it away and mark the directory listing invalid.
	 */
	if (STRCMP(filter_pat, gfm_private->filter_pattern) != 0)
	{
		if (gfm_private->filter_pattern)
			free(gfm_private->filter_pattern);

		gfm_private->dir_mtime = 0;

		if (filter_pat && *filter_pat)
			gfm_private->filter_pattern = strdup(filter_pat);
		else
			gfm_private->filter_pattern = NULL;
	}

	gfm_private->filter_callback = filter_callback;

	/*
	 * Must specify callback, otherwise file choose is worthless
	 */
	if (!callback)
	{
		fprintf(stderr, dgettext("libguidexv_labels",
					 "gfm: No callback procedure specified - file chooser not activated\n"));
		return;
	} else
		gfm_private->callback = callback;

	/*
	 * If no glyph was specified, use the default (Gfm_document_glyph)
	 */
	gfm_private->user_glyph = glyph ? glyph : Gfm_document_glyph;
	gfm_private->mode = mode;

	switch (mode)
	{
	case GFM_LOAD:
		gfm_set_action(ip, dgettext("libguidexv_labels", "Load"));
		break;
	case GFM_SAVE:
		gfm_set_action(ip, dgettext("libguidexv_labels", "Save"));
		break;
	case GFM_CREATE:
		gfm_set_action(ip, dgettext("libguidexv_labels", "Create"));
		break;
	case GFM_DEFAULT:
	default:
		break;
	}

	xv_set(ip->popup, XV_SHOW, TRUE, NULL);
	XFlush(XV_DISPLAY_FROM_WINDOW(ip->popup));

	gfm_load_dir(ip, new_dir, 1);
}

/*
 * Notify callback function for `list'.
 */
int
gfm_list_proc(item, string, client_data, op, event)
	Panel_item	item;
	char		*string;
	Xv_opaque	client_data;
	Panel_list_op	op;
	Event		*event;
{
	static Xv_opaque	last_sel = (Xv_opaque)NULL;
	static struct timeval	then = {0, 0};
	static struct timeval	now = {0, 0};
	gfm_popup_objects *ip = (gfm_popup_objects *) xv_get(item, XV_KEY_DATA,
							     INSTANCE);

	switch (op)
	{
	case PANEL_LIST_OP_SELECT:
		xv_set(ip->list,
		       PANEL_PAINT, PANEL_NONE,
		       PANEL_CLIENT_DATA, client_data,
		       NULL);

		if (strcmp(GFM_DOTDOT_STR, string) == 0)
			xv_set(ip->file, PANEL_VALUE, "..", NULL);
		else
			xv_set(ip->file, PANEL_VALUE, string, NULL);
		now = event_time(event);

		/*
		 * Check for double click, open directory or file
		 */
		if (double_click(last_sel, &then, client_data, &now))
		{
			if ((client_data & GFM_TYPE_MASK) == GFM_FOLDER)
			{
				if (strcmp(string, GFM_DOTDOT_STR) == 0)
					gfm_load_dir(ip, "..", 1);
				else
					gfm_load_dir(ip, string, 1);
			}
			else
			{
				xv_set(ip->file, PANEL_VALUE, string, NULL);
				gfm_notify_client(ip);
			}
		}
		last_sel = client_data;
		then = now;
		break;

	case PANEL_LIST_OP_DESELECT:
		break;

	case PANEL_LIST_OP_VALIDATE:
		return XV_ERROR;

	case PANEL_LIST_OP_DELETE:
		break;
	}

	return XV_OK;
}

/*
 * Notify callback function for `file'.
 */
Panel_setting
gfm_file_notify(item, event)
	Panel_item	item;
	Event		*event;
{
	char 		   *file;
	gfm_popup_objects *ip = (gfm_popup_objects *) xv_get(item, XV_KEY_DATA,
							     INSTANCE);

	file = (char *)xv_get(ip->file, PANEL_VALUE);

	if (!file || !*file)
		return panel_text_notify(item, event);

	gfm_load_dir(ip, file, 1);

	file = (char *)xv_get(ip->file, PANEL_VALUE);

	if (file && *file)
		gfm_notify_client(ip);

	return PANEL_NONE;
}

/*
 * Notify callback function for `open'.
 */
void
/* ARGSUSED */
gfm_open(item, event)
	Panel_item	item;
	Event		*event;
{
	char		  *file;
	gfm_popup_objects *ip = (gfm_popup_objects *) xv_get(item, XV_KEY_DATA, INSTANCE);

	xv_set(item, PANEL_NOTIFY_STATUS, XV_ERROR, NULL);
	file = (char *)xv_get(ip->file, PANEL_VALUE);

	if (!file || !*file)
	{
		file = (char *) xv_get(ip->list, PANEL_LIST_STRING,
				       (int) xv_get(ip->list,
						    PANEL_LIST_FIRST_SELECTED));

		if (strcmp(GFM_DOTDOT_STR, file) == 0)
		{
			file = "..";
			xv_set(ip->file, PANEL_VALUE, file, NULL);
		}
		else
			xv_set(ip->file, PANEL_VALUE, file, NULL);
	}

	gfm_load_dir(ip, file, 1);

	file = (char *)xv_get(ip->file, PANEL_VALUE);

	if (file && *file)
		gfm_notify_client(ip);
}

/*
 * Event callback function for `popup'.
 */
Notify_value
gfm_resize(win, event, arg, type)
	Xv_window	win;
	Event		*event;
	Notify_arg	arg;
	Notify_event_type type;
{
	int		   win_width;
	int		   win_height;
	gfm_popup_objects *ip;
	GFM_PRIVATE	  *gfm_private;
	
	ip = (gfm_popup_objects *) xv_get(win, XV_KEY_DATA, INSTANCE);
	gfm_private = (GFM_PRIVATE *) xv_get(ip->popup, XV_KEY_DATA, GFM_KEY);

	if (event_action(event) == WIN_RESIZE)
	{
		win_width = (int)xv_get(win, XV_WIDTH);
		win_height = (int)xv_get(win, XV_HEIGHT);

		/*
		 * Only do something if size has changed
		 */
		if ((win_width != gfm_private->width) ||
		    (win_height != gfm_private->height))
		{
			set_new_sizes(ip, win_width, win_height);
			xv_set(ip->file_list_group, GROUP_LAYOUT, TRUE, NULL);
			panel_paint(ip->controls, PANEL_NO_CLEAR);
			gfm_private->width = win_width;
			gfm_private->height = win_height;
		}
	}

	return notify_next_event_func(win, (Notify_event) event, arg, type);
}

/*
 * Calculate a new size for the scrolling list.  The list behaves
 * as a free space hog taking up any remaining vertical space.
 */
static int
set_new_sizes(ip, win_width, win_height)
	gfm_popup_objects	*ip;
	int			win_width;
	int			win_height;
{
	int		row_height = (int)xv_get(ip->list,
						 PANEL_LIST_ROW_HEIGHT);
	int		rem, height, nrows;
	int		height_diff;
	int		char_width;
	int		new_length;
	GFM_PRIVATE	*gfm_private;

	gfm_private = (GFM_PRIVATE *) xv_get(ip->popup, XV_KEY_DATA, GFM_KEY);

	height_diff = win_height - gfm_private->initial_win_height;
	height = gfm_private->initial_list_height + height_diff;
	nrows = (height - gfm_private->initial_list_excess) / row_height;
	rem = (height - gfm_private->initial_list_excess) % row_height;

	if (rem > (row_height * 0.3))
		nrows++;

	new_length = xv_get(ip->popup, WIN_COLUMNS) - 
		strlen((char *)xv_get(ip->directory, PANEL_LABEL_STRING));

	char_width = xv_get(xv_get(ip->controls, XV_FONT),
			    FONT_DEFAULT_CHAR_WIDTH );
	new_length = xv_get(ip->popup, WIN_COLUMNS) -
		((xv_get(ip->directory, PANEL_VALUE_X) + 10) / char_width);
	xv_set(ip->directory, PANEL_VALUE_DISPLAY_LENGTH, new_length, NULL);

	new_length = xv_get(ip->popup, WIN_COLUMNS) -
		((xv_get(ip->file, PANEL_VALUE_X) + 10)/ char_width);
	xv_set(ip->file, PANEL_VALUE_DISPLAY_LENGTH, new_length, NULL);

	xv_set(ip->list,
	       PANEL_LIST_WIDTH,	 win_width - 40,
	       PANEL_LIST_DISPLAY_ROWS, nrows,
	       NULL);
}

/*
 * This procedure notifies the client of the selected file and dismisses the
 * window if the pushpin is out and the result was GFM_OK.
 */
static void
gfm_notify_client(ip)
	gfm_popup_objects	*ip;
{
	int		result;
	int		pin_in = xv_get(ip->popup, FRAME_CMD_PUSHPIN_IN);
	char		*dir = (char *)xv_get(ip->directory, PANEL_VALUE);
	char		*file = (char *)xv_get(ip->file, PANEL_VALUE);
	struct stat	buf;
	GFM_PRIVATE	*gfm_private;

	gfm_private = (GFM_PRIVATE *) xv_get(ip->popup, XV_KEY_DATA, GFM_KEY);
	result = (gfm_private->callback)(ip, dir, file);

	/*
	 * If the client callback returned okay we need to check
	 * a few things.  If the directory has been modified, we mark
	 * it as invalid.  It the file chooser is pinned then we go ahead
	 * an reload it before returning to the client.  If it is unpinned
	 * we dismiss it, it will be reloaded the next time it is activated.
	 */
	if (result == GFM_OK)
	{
		if (stat(dir, &buf) < 0)
		{
			buf.st_mtime = 0;
			fprintf(stderr, dgettext("libguidexv_labels",
				 "gfm: Couldn't stat directory %s\n"), dir);
		}


		if (buf.st_mtime != gfm_private->dir_mtime) {
			gfm_private->dir_mtime = 0;
			gfm_load_dir(ip, dir, 0);
		}
	}
	xv_set(ip->popup, FRAME_BUSY, FALSE, NULL);
}

/*
 * double_click --check for double click
 */
static int
double_click(last_sel, then, this_sel, now)
	Xv_opaque	last_sel;
	struct timeval	*then;
	Xv_opaque	this_sel;
	struct timeval	*now;
{
	struct timeval	delta;

	if (this_sel != last_sel)
		return 0;

	delta.tv_sec = now->tv_sec - then->tv_sec;
	if ((delta.tv_usec = now->tv_usec - then->tv_usec) < 0)
	{
		delta.tv_usec += 1000000;
		delta.tv_sec -= 1;
	}

	/*
	 * Compare delta against multiclick timeout.
	 */
	return (delta.tv_sec*10 + delta.tv_usec/100000) <= Gfm_multiclick_timeout;
}

/*
 * Front end to regexp(3).  Usage:
 *
 *	gfm_compile_regex(regular_expression);
 *	...
 *	matched = gfm_match_regex(string);
 *
 */

/* I removed the original code, and now the routines use the more portable
   routines in the xvnews directory */

void
gfm_compile_regex(regex_str)
	char	*regex_str;
{
	xvnews_comp(regex_str);
}

int
gfm_match_regex(s)
	char	*s;
{
	return xvnews_exec(s);
}
