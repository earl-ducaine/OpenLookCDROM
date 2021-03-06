/*  -------------------------------------------------------------------

This program is the property of:

                             Cornell University 
                        Center for Applied Mathematics 
                              Ithaca, NY 14853

and may be used, modified and distributed freely, subject to the 
following restrictions:

       Any product which incorporates source code from the dstool
       program or utilities, in whole or in part, is distributed
       with a copy of that source code, including this notice. You
       must give the recipients all the rights that you have with
       respect to the use of this software. Modifications of the
       software must carry prominent notices stating who changed
       the files and the date of any change.

DsTool is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of FITNESS 
FOR A PARTICULAR PURPOSE.  The software is provided as is without 
any obligation on the part of Cornell faculty, staff or students to 
assist in its use, correction, modification or enhancement.

  -----------------------------------------------------------------  */
/*
 * defaults_ui.c - User interface object initialization functions.
 * This file was generated by `gxv' from `defaults.G'.
 * DO NOT EDIT BY HAND.
 */

#include <stdio.h>
#include <sys/param.h>
#include <sys/types.h>
#include <xview/xview.h>
#include <xview/canvas.h>
#include <xview/icon_load.h>
#include <xview/panel.h>
#include <xview/scrollbar.h>
#include <xview/svrimage.h>
#include <xview/termsw.h>
#include <xview/text.h>
#include <xview/tty.h>
#include <xview/xv_xrect.h>
#include <gcm.h>

#include <constants.h>
#include <symbols.h>
#include "defaults_cui.h"

/*
 * Initialize an instance of object `win'.
 */
defaults_win_objects *
defaults_win_objects_initialize(ip, owner)
	defaults_win_objects	*ip;
	Xv_opaque	owner;
{
	if (!ip && !(ip = (defaults_win_objects *) calloc(1, sizeof (defaults_win_objects))))
		return (defaults_win_objects *) NULL;
	if (!ip->win)
		ip->win = defaults_win_win_create(ip, owner);
	if (!ip->pan)
		ip->pan = defaults_win_pan_create(ip, ip->win);
	if (!ip->coordinates)
		ip->coordinates = defaults_win_coordinates_create(ip, ip->pan);
	if (!ip->record)
		ip->record = defaults_win_record_create(ip, ip->pan);
	if (!ip->clip)
		ip->clip = defaults_win_clip_create(ip, ip->pan);
	if (!ip->precision)
		ip->precision = defaults_win_precision_create(ip, ip->pan);
	if (!ip->disp_points)
		ip->disp_points = defaults_win_disp_points_create(ip, ip->pan);
	if (!ip->diverging)
		ip->diverging = defaults_win_diverging_create(ip, ip->pan);
	if (!ip->symbol)
		ip->symbol = defaults_win_symbol_create(ip, ip->pan);
	if (!ip->size)
		ip->size = defaults_win_size_create(ip, ip->pan);
	if (!ip->message)
		ip->message = defaults_win_message_create(ip, ip->pan);
	return ip;
}

/*
 * Create object `win' in the specified instance.

 */
Xv_opaque
defaults_win_win_create(ip, owner)
	caddr_t		ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	void		defaults_done_proc();
	
	obj = xv_create(owner, FRAME_CMD,
		XV_KEY_DATA, INSTANCE, ip,
		XV_WIDTH, 329,
		XV_HEIGHT, 326,
		XV_LABEL, "Defaults Settings",
		XV_SHOW, FALSE,
		FRAME_SHOW_FOOTER, FALSE,
		FRAME_SHOW_RESIZE_CORNER, TRUE,
		FRAME_DONE_PROC, defaults_done_proc,
		FRAME_CMD_PUSHPIN_IN, TRUE,
		NULL);
	xv_set(xv_get(obj, FRAME_CMD_PANEL), WIN_SHOW, FALSE, NULL);
	return obj;
}

/*
 * Create object `pan' in the specified instance.

 */
Xv_opaque
defaults_win_pan_create(ip, owner)
	caddr_t		ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 0,
		XV_Y, 0,
		XV_WIDTH, WIN_EXTEND_TO_EDGE,
		XV_HEIGHT, WIN_EXTEND_TO_EDGE,
		WIN_BORDER, FALSE,
		NULL);
	return obj;
}

/*
 * Create object `coordinates' in the specified instance.

 */
Xv_opaque
defaults_win_coordinates_create(ip, owner)
	caddr_t		ip;
	Xv_opaque	owner;
{
	extern int		default_setting_notify();
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_CHOICE, PANEL_DISPLAY_LEVEL, PANEL_CURRENT,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 7,
		XV_Y, 12,
		XV_WIDTH, 234,
		XV_HEIGHT, 23,
		PANEL_VALUE_X, 147,
		PANEL_VALUE_Y, 12,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_CHOICE_NROWS, 1,
		PANEL_LABEL_STRING, "Coordinate System:",
		PANEL_NOTIFY_PROC, default_setting_notify,
		PANEL_CHOICE_STRINGS,
			"Cartesian",
			"Polar",
			0,
		NULL);
	return obj;
}

/*
 * Create object `record' in the specified instance.

 */
Xv_opaque
defaults_win_record_create(ip, owner)
	caddr_t		ip;
	Xv_opaque	owner;
{
	extern int		default_setting_notify();
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_CHOICE,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 7,
		XV_Y, 40,
		XV_WIDTH, 162,
		XV_HEIGHT, 23,
		PANEL_VALUE_X, 100,
		PANEL_VALUE_Y, 40,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_CHOICE_NROWS, 1,
		PANEL_LABEL_STRING, "Record Data:",
		PANEL_NOTIFY_PROC, default_setting_notify,
		PANEL_CHOICE_STRINGS,
			"Yes",
			"No",
			0,
		NULL);
	return obj;
}

/*
 * Create object `clip' in the specified instance.

 */
Xv_opaque
defaults_win_clip_create(ip, owner)
	caddr_t		ip;
	Xv_opaque	owner;
{
	extern int		default_setting_notify();
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_CHOICE,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 180,
		XV_Y, 40,
		XV_WIDTH, 91,
		XV_HEIGHT, 23,
		PANEL_VALUE_X, 255,
		PANEL_VALUE_Y, 40,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_CHOICE_NROWS, 1,
		PANEL_LABEL_STRING, "Clip Data:",
		PANEL_NOTIFY_PROC, default_setting_notify,
		PANEL_CHOICE_STRINGS,
			"Yes",
			"No",
			0,
		NULL);
	return obj;
}

/*
 * Create object `precision' in the specified instance.
 */
Xv_opaque
defaults_win_precision_create(ip, owner)
	caddr_t		ip;
	Xv_opaque	owner;
{
	extern Panel_setting	default_text_notify();
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_NUMERIC_TEXT,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 8,
		XV_Y, 76,
		XV_WIDTH, 206,
		XV_HEIGHT, 15,
		PANEL_LABEL_STRING, "Window Precision:",
		PANEL_VALUE_X, 141,
		PANEL_VALUE_Y, 76,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_VALUE_DISPLAY_LENGTH, 3,
		PANEL_VALUE_STORED_LENGTH, 80,
		PANEL_MAX_VALUE, 20,
		PANEL_MIN_VALUE, 0,
		PANEL_READ_ONLY, FALSE,
		PANEL_NOTIFY_PROC, default_text_notify,
		NULL);
	return obj;
}

/*
 * Create object `disp_points' in the specified instance.

 */
Xv_opaque
defaults_win_disp_points_create(ip, owner)
	caddr_t		ip;
	Xv_opaque	owner;
{
	extern Panel_setting	default_text_notify();
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_TEXT,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 7,
		XV_Y, 100,
		XV_WIDTH, 177,
		XV_HEIGHT, 15,
		PANEL_LABEL_STRING, "Display points:",
		PANEL_VALUE_X, 120,
		PANEL_VALUE_Y, 100,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_VALUE_DISPLAY_LENGTH, 8,
		PANEL_VALUE_STORED_LENGTH, 80,
		PANEL_MAX_VALUE, 0,
		PANEL_MIN_VALUE, 0,
		PANEL_READ_ONLY, FALSE,
		PANEL_NOTIFY_PROC, default_text_notify,
		NULL);
	return obj;
}

/*
 * Create object `diverging' in the specified instance.

 */
Xv_opaque
defaults_win_diverging_create(ip, owner)
	caddr_t		ip;
	Xv_opaque	owner;
{
	extern Panel_setting	default_text_notify();
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_TEXT,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 7,
		XV_Y, 124,
		XV_WIDTH, 143,
		XV_HEIGHT, 15,
		PANEL_LABEL_STRING, "Diverging:",
		PANEL_VALUE_X, 86,
		PANEL_VALUE_Y, 124,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_VALUE_DISPLAY_LENGTH, 8,
		PANEL_VALUE_STORED_LENGTH, 80,
		PANEL_MAX_VALUE, 0,
		PANEL_MIN_VALUE, 0,
		PANEL_READ_ONLY, FALSE,
		PANEL_NOTIFY_PROC, default_text_notify,
		NULL);
	return obj;
}

/*
 * Create object `symbol' in the specified instance.

 */
Xv_opaque
defaults_win_symbol_create(ip, owner)
	caddr_t		ip;
	Xv_opaque	owner;
{
	extern int		default_setting_notify();
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_CHOICE, PANEL_DISPLAY_LEVEL, PANEL_CURRENT,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 7,
		XV_Y, 148,
		XV_WIDTH, 177,
		XV_HEIGHT, 23,
		PANEL_VALUE_X, 127,
		PANEL_VALUE_Y, 148,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_CHOICE_NROWS, 1,
		PANEL_LABEL_STRING, "Plotting Symbol:",
		PANEL_NOTIFY_PROC, default_setting_notify,
		PANEL_CHOICE_STRINGS,
			Panel_Sym_Names[0],
			Panel_Sym_Names[1],
			Panel_Sym_Names[2],
			Panel_Sym_Names[3],
			0,
		NULL);
	return obj;
}

/*
 * Create object `size' in the specified instance.

 */
Xv_opaque
defaults_win_size_create(ip, owner)
	caddr_t		ip;
	Xv_opaque	owner;
{
	extern int		default_setting_notify();
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_CHOICE, PANEL_DISPLAY_LEVEL, PANEL_CURRENT,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 65,
		XV_Y, 172,
		XV_WIDTH, 110,
		XV_HEIGHT, 23,
		PANEL_VALUE_X, 105,
		PANEL_VALUE_Y, 172,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_CHOICE_NROWS, 1,
		PANEL_LABEL_STRING, "Size:",
		PANEL_NOTIFY_PROC, default_setting_notify,
		PANEL_CHOICE_STRINGS,
			Panel_Sym_Sizes[0],
			Panel_Sym_Sizes[1],
			Panel_Sym_Sizes[2],
			Panel_Sym_Sizes[3],
			0,
		NULL);
	return obj;
}

/*
 * Create object `message' in the specified instance.

 */
Xv_opaque
defaults_win_message_create(ip, owner)
	caddr_t		ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_MESSAGE,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 112,
		XV_Y, 200,
		XV_WIDTH, 126,
		XV_HEIGHT, 13,
		PANEL_LABEL_STRING, "Min                  Max",
		PANEL_LABEL_BOLD, TRUE,
		NULL);
	return obj;
}

/*
 * Create object `varmin' in the specified instance.

 */
Xv_opaque
defaults_win_varmin_create(ip, owner,i)
	caddr_t		ip;
	Xv_opaque	owner;
{
	extern Panel_setting	default_text_notify();
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_TEXT,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 72,
		XV_Y, 232,
		XV_WIDTH, 83,
		XV_HEIGHT, 15,
		PANEL_LABEL_STRING, "x:",
		PANEL_VALUE_X, 91,
		PANEL_VALUE_Y, 232+20*i,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_VALUE_DISPLAY_LENGTH, 10,
		PANEL_VALUE_STORED_LENGTH, 80,
		PANEL_MAX_VALUE, 0,
		PANEL_MIN_VALUE, 0,
		PANEL_READ_ONLY, FALSE,
		PANEL_NOTIFY_PROC, default_text_notify,
		NULL);
	return obj;
}

/*
 * Create object `varmax' in the specified instance.

 */
Xv_opaque
defaults_win_varmax_create(ip, owner,i)
	caddr_t		ip;
	Xv_opaque	owner;
{
	extern Panel_setting	default_text_notify();
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_TEXT,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 184,
		XV_Y, 232,
		XV_WIDTH, 76,
		XV_HEIGHT, 15,
		PANEL_LABEL_STRING, ":",
		PANEL_VALUE_X, 196,
		PANEL_VALUE_Y, 232+20*i,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_VALUE_DISPLAY_LENGTH, 10,
		PANEL_VALUE_STORED_LENGTH, 80,
		PANEL_MAX_VALUE, 0,
		PANEL_MIN_VALUE, 0,
		PANEL_READ_ONLY, FALSE,
		PANEL_NOTIFY_PROC, default_text_notify,
		NULL);
	return obj;
}

