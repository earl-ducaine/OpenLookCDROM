#ifndef lint
static char    *RCSid = "$Header: /n/homeserver/i/gounares/pu/apex/src/apex/RCS/navigator.c,v 1.1 93/01/06 03:27:40 gounares Exp Locker: gounares $";

#endif

/*
 * $Log:	navigator.c,v $
 * Revision 1.1  93/01/06  03:27:40  gounares
 * Initial revision
 * 
 */

/*
 * navigator.c
 * 
 * for use in the apeX system.
 */

/*
 * Copyright 1993 Alexander Gounares
 * 
 * This source is covered by the GNU General Public License Version 2
 * 
 * see the apeX manual for more details
 */


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <xview/xview.h>
#include <xview/frame.h>
#include <xview/openmenu.h>
#include <xview/scrollbar.h>
#include <xview/panel.h>
#include <xview/cms.h>
#include <xview/svrimage.h>
#include <xview/textsw.h>
#include <xview/notice.h>
#include <xview/icon.h>
#include <xview/notify.h>
#include <sspkg/canshell.h>
#include <sspkg/tree.h>
#include <sspkg/rectobj.h>
#include <sspkg/drawobj.h>
#include <math.h>
#include "symbol.h"
#include "editor.h"
#include "file_browser.h"
#include "apex_nav_bmp"
#include "apex_help.h"
#include "apex_options.h"

#define gray_width 2
#define gray_height 2
static unsigned char gray_bits[] =
{
	0x01, 0x02};

#define CALLTAG		XV_KEY_DATA, calltag_key
#define BOXTAG		XV_KEY_DATA, boxtag_key
#define NGTAG		XV_KEY_DATA, ngtag_key

static int      calltag_key,
                boxtag_key,
                ngtag_key;
extern int      CLIENT_DATA_KEY;

#define FSIZE		256

static int      fColor = 1;

Rectobj         add_leaf();
void            expand_subdirs();
void            set_horizontal();
void            set_vertical();
void            prune_proc();
void            resize();
void            dbl_click_proc();
static void     draw_background();
static int      load_data();
static void     show_text_proc();
static Panel_setting find_proc();
void            prune_tree();
static void     prune_node_proc();
static void     create_search_panel_proc();
static void     reload_proc();
static void     navigator_done_proc();

typedef struct _NGS {
	Frame           frame;
	Panel           panel;
	Panel_item      pattern_text;
	Panel_item      match_list;
}               NGS;

typedef struct _NG {
	Frame           frame;
	Panel           panel;
	Menu            file_menu;
	Menu            view_menu;
	Panel_item      file_button;
	Panel_item      view_button;
	Panel_item      search_button;
	Panel_item      call_choice;
	Panel_item      dismiss_button;
	Panel_item      find_text;
	Canvas_shell    shell;
	Drawarea        drawarea;
	Tree            tree;
	Editor         *ped;
	char            szDirname[1024];
	char            szFilename[1024];
	NGS            *pngs;
}               NG;



Menu            menu;

#define START_HEIGHT	700
#define START_WIDTH	500

static char    *nav_help[] =
{
	"the Navigator",
	"B", "Overview",
	"b", "    The Navigator interactively displays a call graph of your",
	"b", "program.  Data for the call graph should reside in a file",
	"b", "called 'apex.parse' in the current directory.",
	"b", "This file is produced by the apeX C parser",
	"b", " ",
	"B", "Viewing the Call Graph",
	"b", "    Double-clicking on a function box (such as main) will",
	"b", "show the functions called from (or called by) that function",
	"b", " ",
	"b", "    Box sizes are proportional to the complexity of the",
	"b", "function (specifically, the natural log of the program volume)",
	"b", " ",
	"b", "   The right mouse button will show the options available for",
	"b", "each function",
	"b", " ",
	"B", "Searching for a function",
	"b", "    There are two methods for finding a function:",
	"b", "        1. Enter the name in the Find: field.",
	"b", "           The function name will be automatically completed",
	"b", "           when possible",
	"b", "        2. Use the 'Search...' popup",
	"b", "           Search uses glob pattern matching (like shells)",
	0};


/*
 * load_datafile_proc -- called from the generic browser
 */
static void
load_datafile_proc(item, event)
    Panel_item      item;
    Event          *event;
{
	NG             *png = (NG *) xv_get(item, NGTAG);
	FB             *pfb = (FB *) xv_get(item, PANEL_CLIENT_DATA);

	strcpy(png->szDirname, (char *) xv_get(pfb->dir_item, PANEL_VALUE));
	strcpy(png->szFilename, (char *) xv_get(pfb->file_item, PANEL_VALUE));

	reload_proc(png->file_menu, NULL);
}

/*
 * load_datafile_menu_proc -- called from the menu
 */
static void
load_datafile_menu_proc(menu, menu_item)
    Menu            menu;
    Menu_item       menu_item;
{
	NG             *png = (NG *) xv_get(menu, NGTAG);
	FB             *pfb = create_generic_browser(png->frame, NULL);

	xv_set(pfb->frame, FRAME_LABEL, "Load Datafile for apeX Navigator",
		XV_SHOW, TRUE, NULL);

	pfb->btn1 = (Panel_item) xv_create(pfb->panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, "Load Datafile",
		XV_X, 180,
		XV_Y, 135,
		PANEL_CLIENT_DATA, pfb,
		NGTAG, png,
		PANEL_NOTIFY_PROC, load_datafile_proc,
		NULL);

	xv_set(pfb->file_item, PANEL_NOTIFY_PROC, load_datafile_proc,
		NGTAG, png,
		NULL);
}


/*
 * reload_proc -- reload the current database (thus updating information)
 */
static void
reload_proc(menu, menu_item)
    Menu            menu;
    Menu_item       menu_item;
{
	char           *szFullname[2048];
	Rectobj_list   *list;
	Drawrect        child;

	NG             *png = (NG *) xv_get(menu, NGTAG);

	clear_table();
	sprintf(szFullname, "%s/%s", png->szDirname, png->szFilename);

	while (list = (Rectobj_list *) xv_get(png->tree,
			TREE_LINK_TO_LIST, png->tree)) {
		child = RECTOBJ_LIST_HANDLE(list);
		prune_tree(child, 1);
	}

	load_data(szFullname);

	add_leaf(get_nav_start(), lookup(strsave(get_nav_start()), NULL),
		png->tree, png->tree, png);

	/* now take care of the search popup (if exists) */

	if (png->pngs) {
		int             rows;

		rows = (int) xv_get(png->pngs->match_list, PANEL_LIST_NROWS);
		xv_set(png->pngs->match_list, PANEL_LIST_DELETE_ROWS, 0, rows, NULL);
	}
}

/*
 * navigator_done -- called when the frame is 'Quit'
 */
static Notify_value 
navigator_done(client, status)
    Notify_client   client;
    Destroy_status  status;
{
	if (status == DESTROY_CHECKING) {
		xv_set(client, XV_SHOW, FALSE, NULL);
		notify_veto_destroy(client);
	}
	return NOTIFY_DONE;

}

/*
 * navigator_done_proc -- called when the "Dismiss" button is hit
 */
static void 
navigator_done_proc(item, event)
    Panel_item      item;
    Event          *event;
{
	NG             *png = (NG *) xv_get(item, PANEL_CLIENT_DATA);

	xv_set(png->frame, XV_SHOW, FALSE, NULL);
}

create_navigator_proc(item, event)
    Panel_item      item;
    Event          *event;
{
	Rectobj         root_node;
	char           *root_dir_name;
	Scrollbar       vscroll;
	Scrollbar       hscroll;
	static Cms      cms;
	static Display *dpy;
	static NG      *png;
	static Menu     file_menu,
	                view_menu;
	static Icon     icon;
	static Server_image nav_image;
	Editor         *ped;

	/* this is an ugly hack for now :( :( :( :( :( :( :( */

	ped = (Editor *) xv_get(item, XV_KEY_DATA, CLIENT_DATA_KEY);

	if (!png) {
		png = (NG *) acalloc(1, sizeof(NG));
		png->ped = ped;
	} else {
		xv_set(png->frame, XV_SHOW, TRUE, NULL);
		png->ped = ped;
		return;
	}



	getwd(png->szDirname);
	sprintf(png->szFilename, get_datafile());

	if (!ngtag_key) {
		ngtag_key = xv_unique_key();
		calltag_key = xv_unique_key();
		boxtag_key = xv_unique_key();
	}
	if (!nav_image) {
		nav_image = (Server_image) xv_create(NULL, SERVER_IMAGE,
			SERVER_IMAGE_X_BITS, apex_nav_bmp_bits,
			XV_HEIGHT, apex_nav_bmp_height,
			XV_WIDTH, apex_nav_bmp_width,
			SERVER_IMAGE_DEPTH, 1,
			NULL);
		icon = (Icon) xv_create(NULL, ICON,
			ICON_IMAGE, nav_image,
			ICON_MASK_IMAGE, nav_image,
			ICON_TRANSPARENT, TRUE,
			NULL);
	}
	png->frame = (Frame) xv_create(XV_NULL, FRAME,
		FRAME_LABEL, "apeX Navigator",
		XV_WIDTH, START_WIDTH,
		XV_HEIGHT, START_HEIGHT,
		FRAME_ICON, icon,
		NULL);

	if (!dpy) {
		dpy = (Display *) xv_get(png->frame, XV_DISPLAY);
		if (DefaultDepth(dpy, DefaultScreen(dpy)) < 2)
			fColor = 0;

		if (fColor)
			cms = (Cms) xv_create(XV_NULL, CMS,
				CMS_CONTROL_CMS, TRUE,
				CMS_SIZE, CMS_CONTROL_COLORS + 7,
				CMS_NAMED_COLORS, "black", "blue", "ForestGreen",
				"yellow", "white", "green", "red", 0,
				NULL);
		else
			cms = (Cms) xv_create(XV_NULL, CMS,
				CMS_SIZE, 2,
				CMS_NAMED_COLORS, "white", "black", 0,
				NULL);
	}
	png->file_menu = (Menu) xv_create(XV_NULL, MENU,
		MENU_ACTION_ITEM, "Reload Data", reload_proc,
		MENU_ACTION_ITEM, "Load Datafile...", load_datafile_menu_proc,
		NGTAG, png,
		NULL);

	menu = png->view_menu = (Menu) xv_create(XV_NULL, MENU_COMMAND_MENU,
		MENU_GEN_PIN_WINDOW, png->frame, "Options",
		MENU_ITEM,
		MENU_STRING, "Horizontal Layout",
		MENU_ACTION_PROC, set_horizontal,
		NULL,
		MENU_ITEM,
		MENU_STRING, "Vertical Layout",
		MENU_ACTION_PROC, set_vertical,
		NULL,
		MENU_ITEM,
		MENU_STRING, "Prune Selected Nodes",
		MENU_ACTION_PROC, prune_proc,
		NULL,
		NGTAG, png,
		NULL);

	png->panel = (Panel) xv_create(png->frame, PANEL,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		NULL);

	png->file_button = (Panel_item) xv_create(png->panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, "File",
		PANEL_ITEM_MENU, png->file_menu,
		NULL);

	png->view_button = (Panel_item) xv_create(png->panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, "View",
		PANEL_ITEM_MENU, png->view_menu,
		NULL);

	png->search_button = -(Panel_item) xv_create(png->panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, "Search...",
		PANEL_CLIENT_DATA, png,
		PANEL_NOTIFY_PROC, create_search_panel_proc,
		NULL);

	png->call_choice = (Panel_item) xv_create(png->panel, PANEL_CHOICE,
		PANEL_CHOICE_STRINGS, "Calls", "Called By", NULL,
		NULL);

	png->dismiss_button = (Panel_item) xv_create(png->panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, "Dismiss",
		PANEL_CLIENT_DATA, png,
		PANEL_NOTIFY_PROC, navigator_done_proc,
		NULL);

	png->find_text = (Panel_item) xv_create(png->panel, PANEL_TEXT,
		PANEL_LABEL_STRING, "Find:",
		PANEL_VALUE_DISPLAY_LENGTH, 30,
		PANEL_VALUE_STORED_LENGTH, FSIZE,
		PANEL_NOTIFY_PROC, find_proc,
		PANEL_CLIENT_DATA, png,
		PANEL_NOTIFY_LEVEL, PANEL_ALL,
		NULL);

	window_fit(png->panel);

	png->drawarea = (Drawarea) xv_create(XV_NULL, DRAWAREA,
		NGTAG, png,
		NULL);

	png->shell = (Canvas_shell) xv_create(png->frame, CANVAS_SHELL,
		WIN_CMS, cms,
		XV_X, 0,
		XV_Y, 0,
		CANVAS_MIN_PAINT_WIDTH, START_WIDTH,
		CANVAS_MIN_PAINT_HEIGHT, START_HEIGHT,
		CANVAS_RESIZE_PROC, resize,
		WIN_BELOW, png->panel,
		CANVAS_SHELL_BATCH_REPAINT, TRUE,
		NGTAG, png,
		NULL);

	xv_set(png->drawarea, XV_OWNER, png->shell,
		XV_WIDTH, xv_get(png->shell, XV_WIDTH),
		XV_HEIGHT, xv_get(png->shell, XV_HEIGHT),
		NGTAG, png,
		NULL);

	vscroll = (Scrollbar) xv_create(png->shell, SCROLLBAR,
		SCROLLBAR_DIRECTION, SCROLLBAR_VERTICAL,
		SCROLLBAR_SPLITTABLE, TRUE,
		SCROLLBAR_PIXELS_PER_UNIT, 10,
		NULL);

	hscroll = (Scrollbar) xv_create(png->shell, SCROLLBAR,
		SCROLLBAR_DIRECTION, SCROLLBAR_HORIZONTAL,
		SCROLLBAR_SPLITTABLE, TRUE,
		SCROLLBAR_PIXELS_PER_UNIT, 10,
		NULL);

	if (fColor)
		xv_set(png->shell, WIN_BACKGROUND_COLOR, CMS_CONTROL_COLORS, 0);

	png->tree = (Tree) xv_create(png->shell, TREE,
		TREE_PARENT_DISTANCE, 50,
		XV_WIDTH, START_WIDTH,
		XV_HEIGHT, START_HEIGHT,
		NGTAG, png,
		NULL);

	if (get_nav_3d())
		draw_background(png);

	window_fit(png->frame);

	xv_set(png->panel, XV_WIDTH, START_WIDTH, NULL);

	set_help_button(png->frame, png->panel, nav_help);

	xv_set(png->panel,
		XV_WIDTH, WIN_EXTEND_TO_EDGE,
		NULL);

	load_data(get_datafile());
	add_leaf(get_nav_start(), lookup(strsave(get_nav_start()), NULL),
		png->tree, png->tree, png);
	xv_set(png->frame, XV_SHOW, TRUE, NULL);

	notify_interpose_destroy_func(png->frame, navigator_done);

}



Rectobj
add_leaf(szName, s, tree, parent, png)
    char           *szName;
    Symbol          s;
    Tree            tree;
    Rectobj         parent;
    NG             *png;
{
	Rectobj         new;
	Drawrect        drawrect;
	Drawline        drawline;
	int             border = 0,
	                text_color = 1,
	                text_bg_color = 1;
	Menu            lmenu;
	char            szBuf[256];

	if (s) {
		border = (int) log((double) s->V + 1.0);
		sprintf(szBuf, "%s: %d", s->szFilename, s->lineno);
		lmenu = (Menu) xv_create(NULL, MENU,
			MENU_TITLE_ITEM, strdup(szBuf),
			MENU_ITEM,
			MENU_STRING, "Display Source",
			MENU_GEN_PULLRIGHT, editor_menu_gen_proc,
			MENU_CLIENT_DATA, png->ped,
			MENU_NOTIFY_PROC, show_text_proc,
			CALLTAG, s,
			NGTAG, png,
			NULL,
			MENU_ITEM,
			MENU_STRING, "Prune this node",
			MENU_NOTIFY_PROC, prune_node_proc,
			NULL,
			MENU_CLIENT_DATA, png->ped,
			CALLTAG, s,
			NGTAG, png,
			NULL);
	}
	border = (border > 10) ? 10 : border;

	drawrect = (Drawrect) xv_create(tree, DRAWRECT,
		DRAWOBJ_FILLED, TRUE,
		RECTOBJ_FG, (fColor) ? CMS_CONTROL_COLORS + 2 : 1,
		RECTOBJ_BG, (fColor) ? 1 : 0 /* CMS_CONTROL_COLORS + 2 */ ,
		DRAWRECT_BORDER3, 3 + border,
		DRAWRECT_BORDER2, 5 + border,
		CALLTAG, s,
		NGTAG, png,
		RECTOBJ_MENU, (s) ? lmenu : menu,
		NULL);

	if (s)
		xv_set(lmenu, BOXTAG, drawrect, NULL);

	/* draw library modules in another color */
	if (fColor && !s)
		text_color = CMS_CONTROL_COLORS;
	else if (fColor)
		text_color = CMS_CONTROL_COLORS + 1;
	else if (!s) {
		text_color = 0;
		text_bg_color = 1;
	} else
		text_bg_color = 0;


	new = (Rectobj) xv_create(drawrect, DRAWTEXT,
		DRAWTEXT_STRING, szName,
		CALLTAG, s,
		RECTOBJ_FG, text_color,
		RECTOBJ_BG, text_bg_color,
		BOXTAG, drawrect,
		RECTOBJ_DBL_CLICK_PROC, dbl_click_proc,
		NGTAG, png,
		NULL);

	xv_set(tree,
		TREE_ADD_LINK, parent, drawrect,
		NULL);

	drawline = (Drawline) xv_get(tree, TREE_DRAWLINE, drawrect);

	if (xv_get(png->call_choice, PANEL_VALUE) == 1)
		xv_set(drawline, DRAWLINE_ARROW_STYLE, 0, ARROW_FILLED,
			RECTOBJ_FG, (fColor) ? CMS_CONTROL_COLORS + 3 : 1, NULL);
	else
		xv_set(drawline,
			RECTOBJ_FG, (fColor) ? CMS_CONTROL_COLORS + 6 : 1, NULL);

	return (Rectobj) drawrect;
}


void
dbl_click_proc(paint_window, event, canvas_shell, rectobj)
    Xv_window       paint_window;
    Event          *event;
    Canvas_shell    canvas_shell;
    Rectobj         rectobj;
{
	Rectobj_list   *list;
	Rectobj         selected;
	NG             *png = (NG *) xv_get(rectobj, NGTAG);

	xv_set(png->frame,
		FRAME_BUSY, TRUE,
		NULL);

	list = rectobj_get_selected_list();
	list_for(list) {
		selected = RECTOBJ_LIST_HANDLE(list);
		expand_subdirs(selected);
	}

	xv_set(png->frame,
		FRAME_BUSY, FALSE,
		NULL);
}


void
expand_subdirs(rectobj)
    Rectobj         rectobj;
{
	Symbol          s;
	int             i,
	                j,
	                c;
	char          **calls;
	NG             *png;

	s = (Symbol) xv_get(rectobj, CALLTAG);
	png = (NG *) xv_get(rectobj, NGTAG);

	c = xv_get(png->call_choice, PANEL_VALUE);

	if (!s || s->seen & (c + 1))
		return;
	xv_set(png->tree,
		XV_WIDTH, xv_get(png->shell, XV_WIDTH),
		XV_HEIGHT, xv_get(png->shell, XV_HEIGHT),
		NULL);


	if (c != 1)
		for (calls = s->calls; calls && *calls; calls++) {
			add_leaf(*calls, lookup(*calls, s->szFilename), png->tree,
				xv_get(rectobj, BOXTAG), png);
		}
	else {
		Symbol         *all = allsymbols(0);
		Symbol         *hall = all;

		for (; all && *all; all++) {
			for (calls = (*all)->calls; calls && *calls; calls++) {
				if (*calls == s->szName &&
					lookup(*calls, (*all)->szFilename) == s) {
					add_leaf((*all)->szName, *all, png->tree,
						xv_get(rectobj, BOXTAG), png);
				}
			}
		}
		free(hall);
	}
	s->seen += 1 + c;

	xv_set(png->shell,
		CANVAS_MIN_PAINT_WIDTH, xv_get(png->tree, XV_WIDTH),
		CANVAS_MIN_PAINT_HEIGHT, xv_get(png->tree, XV_HEIGHT),
		NULL);
}


void
set_horizontal(menu, menu_item)
    Menu            menu;
    Menu_item       menu_item;
{
	NG             *png = (NG *) xv_get(menu, NGTAG);

	/*
	 * Set the size of the tree before changing it. This will have the
	 * effect of centering it if all the space isn't used.
	 */
	xv_set(png->tree,
		XV_WIDTH, xv_get(png->shell, XV_WIDTH),
		XV_HEIGHT, xv_get(png->shell, XV_HEIGHT),
		TREE_LAYOUT, TREE_LAYOUT_HORIZONTAL,
		NULL);

	/*
	 * In case the tree grew bigger than the paint window, set the paint
	 * window size.  This way, the scrollbars will be properly set.
	 */
	xv_set(png->shell,
		CANVAS_MIN_PAINT_WIDTH, xv_get(png->tree, XV_WIDTH),
		CANVAS_MIN_PAINT_HEIGHT, xv_get(png->tree, XV_HEIGHT),
		NULL);
}


void
set_vertical(menu, menu_item)
    Menu            menu;
    Menu_item       menu_item;
{
	NG             *png = (NG *) xv_get(menu, NGTAG);

	xv_set(png->tree,
		XV_WIDTH, xv_get(png->shell, XV_WIDTH),
		XV_HEIGHT, xv_get(png->shell, XV_HEIGHT),
		TREE_LAYOUT, TREE_LAYOUT_VERTICAL,
		NULL);

	xv_set(png->shell,
		CANVAS_MIN_PAINT_WIDTH, xv_get(png->tree, XV_WIDTH),
		CANVAS_MIN_PAINT_HEIGHT, xv_get(png->tree, XV_HEIGHT),
		NULL);
}


void
prune_tree(drawrect, killit)
    Drawrect        drawrect;
    int             killit;
{
	Rectobj_list   *list;
	Drawrect        child;
	Symbol          s;
	NG             *png = (NG *) xv_get(drawrect, NGTAG);

	while (list = (Rectobj_list *) xv_get(png->tree,
			TREE_LINK_TO_LIST, drawrect)) {
		child = RECTOBJ_LIST_HANDLE(list);
		prune_tree(child, 1);
	}
	s = (Symbol) xv_get(drawrect, CALLTAG);
	if (s)
		s->seen = 0;

	if (killit) {
		/* xv_set(tree, TREE_UNLINK, drawrect, NULL ); */
		xv_destroy(drawrect);
	} else
		xv_set((Drawtext) xv_get(drawrect, RECTOBJ_NTH_CHILD, 0),
			RECTOBJ_SELECTED, FALSE, NULL);
}

void
prune_proc(menu, menu_item)
    Menu            menu;
    Menu_item       menu_item;
{
	Rectobj_list   *list;
	NG             *png = (NG *) xv_get(menu, NGTAG);

	xv_set(png->shell,
		CANVAS_SHELL_DELAY_REPAINT, TRUE,
		NULL);

	xv_set(png->tree,
		XV_WIDTH, xv_get(png->shell, XV_WIDTH),
		XV_HEIGHT, xv_get(png->shell, XV_HEIGHT),
		NULL);

	/*
	 * destroy the selected nodes... Have to be careful with the list
	 * because they list is being altered as we traverse it.
	 */
	while (list = rectobj_get_selected_list()) {
		Drawrect        drawrect;

		drawrect = (Drawrect) xv_get(RECTOBJ_LIST_HANDLE(list), BOXTAG);
		prune_tree(drawrect, 0);
	}

	xv_set(png->shell,
		CANVAS_MIN_PAINT_WIDTH, xv_get(png->tree, XV_WIDTH),
		CANVAS_MIN_PAINT_HEIGHT, xv_get(png->tree, XV_HEIGHT),
		CANVAS_SHELL_DELAY_REPAINT, FALSE,
		NULL);

}

static void
prune_node_proc(menu, menu_item)
    Menu            menu;
    Menu_item       menu_item;
{
	Drawrect        drawrect = (Drawrect) xv_get(menu, BOXTAG);

	prune_tree(drawrect, 0);
}

void
resize(shell, width, height)
    Canvas_shell    shell;
    int             width;
    int             height;
{
	NG             *png = (NG *) xv_get(shell, NGTAG);

	if (png->tree)
		xv_set(png->tree,
			XV_WIDTH, width,
			XV_HEIGHT, height,
			NULL);
	xv_set(png->drawarea,
		XV_WIDTH, width,
		XV_HEIGHT, height,
		NULL);
}

/*
 * draw_background -- draw the funky cool background
 */
static void
draw_background(png)
    NG             *png;
{
	int             i,
	                d;

	VSetColor(png->drawarea, (fColor) ? CMS_CONTROL_COLORS + 2 : 1);

	for (i = 0; i < 20; i++)
		VDrawLine(png->drawarea, -5000 + i * 1000, 10000, i * 500, 5549);

	d = i = 500;
	while (d < 5000 && i > 50) {
		VDrawLine(png->drawarea, 0, 10000 - d, 10000, 10000 - d);
		i = (int) ((double) i * .9);
		d += i;
	}
}

/*
 * strip_newline -- remove the newline at the end of a string (if any exists)
 */
char           *
strip_newline(string)
    char           *string;
{
	int             len = strlen(string);

	if (len) {
		if (string[len - 1] == '\n') {
			string[len - 1] = '\0';
		}
	}
	return string;
}

/*
 * load_data -- load a parse data file, inserting symbols into the symbol
 * table
 */
static int
load_data(szFilename)
    char           *szFilename;
{
	Symbol          s;
	char            szBuf[2024];
	char            szName[1024],
	                szFile[1024];
	int             cCalls;
	FILE           *fp;
	char            c;

	if ((fp = fopen(szFilename, "r")) == NULL)
		return 0;

	while (fgets(szBuf, 2024, fp) != NULL) {
		int             i;

		s = (Symbol) acalloc(1, sizeof(*s));
		sscanf(szBuf, "%c %s %s %d %d %d", &c, szName, szFile, &(s->lineno),
			&(s->V), &cCalls);
		s->fStatic = (c == 'S') ? 1 : 0;
		s->calls = (char **) acalloc(cCalls + 1, sizeof(char **));
		s->szName = strsave(szName);
		s->szFilename = strsave(szFile);
		for (i = 0; i < cCalls; i++) {
			if (fgets(szBuf, 1024, fp) == NULL) {
				fprintf(stderr, "apeX->Error: corrupt data file\n");
				return 0;
			}
			s->calls[i] = strsave(strip_newline(szBuf));
		}
		install(s);
	}
	fclose(fp);
}

/*
 * show_text_proc -- called when a menu item is selected...display the text
 * of a function
 */
static void
show_text_proc(menu, menu_item)
    Menu            menu;
    Menu_item       menu_item;
{
	Editor         *ped = (Editor *) xv_get(menu_item, MENU_CLIENT_DATA);
	Symbol          s = (Symbol) xv_get(menu, CALLTAG);
	NG             *png = (NG *) xv_get(menu, NGTAG);
	char            szFullname[2048];
	FILE           *fp;

	if (!s) {
		/* selected a sub-menu */
		s = (Symbol) xv_get(xv_get(menu, MENU_PARENT), CALLTAG);
		png = (NG *) xv_get(xv_get(menu, MENU_PARENT), NGTAG);
	}
	sprintf(szFullname, "%s/%s", png->szDirname, s->szFilename);

	add_apex(szFullname);
	if ((fp = fopen(szFullname, "r")) == NULL) {

		/*
		 * there was no .apex file for the code, use the normal file
		 * then
		 */
		remove_apex(szFullname);
	} else {
		fclose(fp);
	}

	if (ped)
		set_editor(ped, szFullname, s->lineno);
	else
		new_editor(szFullname, s->lineno, EDITOR_MAIN);

}

/*
 * find_proc -- find the selection and create a new tree (if needed)
 */
static          Panel_setting
find_proc(item, event)
    Panel_item      item;
    Event          *event;
{
	NG             *png = (NG *) xv_get(item, PANEL_CLIENT_DATA);
	Rectobj_list   *list;
	Drawrect        child;
	int             action = event_action(event);
	static char     szMatch[FSIZE + 1];
	int             pos = 0;
	static int      finish = 0;
	Symbol         *all,
	               *hall,
	                match;

	if ( /* finish && */ (action == '\n' || action == '\r' || action == '\033')) {
		char           *szName = (char *) xv_get(item, PANEL_VALUE);
		Symbol          s = lookup(strsave(szName));

		if (!s) {
			char            szBuf[1024];

			/*
			 * sprintf(szBuf, "Unable to find function '%s' in
			 * apeX database", szName); notice_prompt(png->frame,
			 * event, NOTICE_FOCUS_XY, event_x(event),
			 * event_y(event), NOTICE_MESSAGE_STRINGS, szBuf,
			 * NULL, NOTICE_BUTTON_NO, "OK", NULL);
			 */
			return PANEL_INSERT;
		}
		while (list = (Rectobj_list *) xv_get(png->tree,
				TREE_LINK_TO_LIST, png->tree)) {
			child = RECTOBJ_LIST_HANDLE(list);
			prune_tree(child, 1);
		}

		add_leaf(s->szName, s, png->tree, png->tree, png);
		xv_set(item, PANEL_VALUE, "", NULL);
		finish = 0;
		return PANEL_INSERT;
	}
	/* try to do auto-fill now */

	strcpy(szMatch, (char *) xv_get(item, PANEL_VALUE));
	pos = strlen(szMatch);
	szMatch[pos++] = action;
	szMatch[pos] = '\0';
	match = 0;
	for (hall = all = allsymbols(0); all && *all; all++) {
		if (!strncmp(szMatch, (*all)->szName, pos)) {
			if (match)
				return (finish == 1) ? PANEL_NONE : PANEL_INSERT;
			else
				match = *all;
		}
	}
	free(hall);
	if (match) {		       /* unique match here */
		xv_set(item, PANEL_VALUE, match->szName, NULL);
		finish = 1;
		return PANEL_NONE;
	}
	if (action < 256 && (isalnum(action) || action == '_') && finish)
		return PANEL_NONE;

	finish = 0;
	return (panel_text_notify(item, event));
}

/*
 * pattern_proc -- see if a pattern matches
 */
static void
pattern_proc(item, event)
    Panel_item      item;
    Event          *event;
{
	NGS            *pngs = (NGS *) xv_get(item, PANEL_CLIENT_DATA);
	char           *szPattern = (char *) xv_get(item, PANEL_VALUE);
	char           *hDFA;
	Symbol         *all = allsymbols(0);
	Symbol         *hall = all;
	int             rows = (int) xv_get(pngs->match_list, PANEL_LIST_NROWS);
	char           *szList[1024];
	int             match = 0;

	/* first delete any existing entries */

	xv_set(pngs->match_list, PANEL_LIST_DELETE_ROWS, 0, rows, NULL);

	hDFA = re_comp(szPattern);

	for (all; all && *all; all++) {
		if (pmatch((*all)->szName, hDFA)) {
			sprintf(szList, "%s (%s)", (*all)->szName,
				(*all)->szFilename);
			xv_set(pngs->match_list, PANEL_LIST_INSERT, 0,
				PANEL_LIST_STRING, 0, szList,
				PANEL_LIST_CLIENT_DATA, 0, *all,
				NULL);
			match = 1;
		}
	}

	free(hall);

	xv_set(pngs->match_list, PANEL_LIST_SORT, PANEL_FORWARD, NULL);
	if (match)
		xv_set(pngs->frame, XV_WIDTH,
			xv_get(pngs->match_list, XV_WIDTH) + 3, NULL);
}

/*
 * match_item_proc -- called when a match item is selected
 */
static void
match_item_proc(item, string, client_data, op, event)
    Panel_item      item;
    char           *string;
    caddr_t         client_data;
    Panel_list_op   op;
    Event          *event;
{
	NG             *png = (NG *) xv_get(item, NGTAG);
	Rectobj_list   *list;
	Symbol          s = (Symbol) client_data;
	Drawrect        child;

	if (op == PANEL_LIST_OP_SELECT) {
		while (list = (Rectobj_list *) xv_get(png->tree,
				TREE_LINK_TO_LIST, png->tree)) {
			child = RECTOBJ_LIST_HANDLE(list);
			prune_tree(child, 1);
		}

		add_leaf(s->szName, s, png->tree, png->tree, png);
	}
}

/*
 * create_search_panel_proc
 */
static void
create_search_panel_proc(item, event)
    Panel_item      item;
    Event          *event;
{
	NG             *png = (NG *) xv_get(item, PANEL_CLIENT_DATA);
	NGS            *pngs;

	if (!png->pngs)
		pngs = png->pngs = (NGS *) acalloc(1, sizeof(NGS));
	else {
		xv_set(png->pngs->frame, XV_SHOW, TRUE, NULL);
		return;
	}

	pngs->frame = (Frame) xv_create(png->frame, FRAME_CMD,
		FRAME_LABEL, "Search for a function",
		FRAME_SHOW_RESIZE_CORNER, TRUE,
		NULL);

	pngs->panel = (Panel) xv_get(pngs->frame, FRAME_CMD_PANEL);

	xv_set(pngs->panel, PANEL_LAYOUT, PANEL_VERTICAL, NULL);

	pngs->pattern_text = (Panel_item) xv_create(pngs->panel, PANEL_TEXT,
		PANEL_LABEL_STRING, "Pattern",
		PANEL_VALUE_DISPLAY_LENGTH, 30,
		PANEL_CLIENT_DATA, pngs,
		PANEL_NOTIFY_PROC, pattern_proc,
		NULL);

	pngs->match_list = (Panel_item) xv_create(pngs->panel, PANEL_LIST,
		XV_WIDTH, WIN_EXTEND_TO_EDGE,
		PANEL_LIST_DISPLAY_ROWS, 7,
		PANEL_CHOOSE_NONE, TRUE,
		PANEL_CLIENT_DATA, pngs,
		NGTAG, png,
		PANEL_NOTIFY_PROC, match_item_proc,
		NULL);

	window_fit(pngs->panel);
	window_fit(pngs->frame);

	xv_set(pngs->frame, XV_SHOW, TRUE, NULL);
}
