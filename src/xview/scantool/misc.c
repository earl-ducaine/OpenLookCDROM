 
/*  @(#)misc.c 1.4 90/04/04
 *
 *  Miscellaneous routines used by the scantool program.
 *
 *  Copyright (c) Rich Burridge.
 *                Sun Microsystems, Australia - All rights reserved.
 *
 *  Permission is given to distribute these sources, as long as the
 *  copyright messages are not removed, and no monies are exchanged.
 *
 *  No responsibility is taken for any errors or inaccuracies inherent
 *  either to the comments or the code of this program, but if
 *  reported to me, then an attempt will be made to fix them.
 */

#include <stdio.h>
#include <ctype.h>
#include <strings.h>
#include "scantool.h"
#include "patchlevel.h"
#include "scantool_extern.h"


draw_settings()     /* Display text for current scanner settings. */
{
  int texty = 20 ;
  int x, y ;

  x = (SCAN_WIDTH - 430) / 2 ;
  y = (SCAN_HEIGHT - 100) / 2 ;
  draw_frame(x, y, 430, 100) ;

  SPRINTF(output, "Brightness:  %s", br_strs[brightness-1]) ;
  draw_text(x+10, y+texty, STEN_OFF, NFONT, output) ;
 
  SPRINTF(output, "Contrast:    %s", con_strs[contrast-1]) ;
  draw_text(x+10, y+texty+15, STEN_OFF, NFONT, output) ;
 
  SPRINTF(output, "Grain:       %s", gr_strs[grain]) ;
  draw_text(x+10, y+texty+30, STEN_OFF, NFONT, output) ;
 
  SPRINTF(output, "Resolution:  %s", res_strs[resolution]) ;
  draw_text(x+10, y+texty+45, STEN_OFF, NFONT, output) ;
 
  SPRINTF(output, "Filename:    %s", picname) ;
  draw_text(x+10, y+texty+60, STEN_OFF, NFONT, output) ;
}


get_help()                 /* Read the help file and display. */
{
  char help_str[MAXLINE], *p ;
  int x, y ;
  int length = 0 ;         /* Length of longest help line. */
  int nolines = 0 ;        /* Number of lines in help file. */
  char line[MAXLINE] ;     /* Current line read from help file. */
  int texty = 20 ;         /* Initial y coordinate for help message. */

  if (nohelp) make_display("No help file found.") ;
  else
    {
      SPRINTF(help_str, "%%%s%%\n", helpname) ;
      rewind(hfd) ;
      while (p = fgets(line, MAXLINE, hfd))
        if (*p == '%' && EQUAL(p, help_str)) break ;
 
      for (;;)             /* Check for the length of the longest line. */
        {
          FGETS(line, MAXLINE, hfd) ;
          if (EQUAL(line, "%%\n")) break ;
          if (strlen(line) > length) length = strlen(line) ;
          nolines++ ;
        }
 
      rewind(hfd) ;
      while (p = fgets(line, MAXLINE, hfd))
        if (*p == '%' && EQUAL(p, help_str)) break ;
 
      set_cursor(HELP_CUR) ;
      x = (SCAN_WIDTH - length*font_width - 20) / 2 ;
      y = (SCAN_HEIGHT - nolines*15+texty) / 2 ;
      draw_frame(x, y, length*font_width+20, nolines*15+texty) ;

      for (;;)
        {
          FGETS(line, MAXLINE, hfd) ;
          if (EQUAL(line, "%%\n")) break ;
          line[strlen(line)-1] = '\0' ;
          draw_text(x+10, y+texty, STEN_OFF, NFONT, line) ;
          texty += 15 ;
        }
    }
}


get_options(argc, argv)        /* Extract command line options. */
int argc ;
char *argv[] ;
{
  INC ;
  while (argc > 0)
    {
      if (argv[0][0] == '-')
        switch (argv[0][1])
          {
            case 'h' : INC ;                         /* Help filename. */
                       getparam(helpname, argv, "-h needs help filename") ;
                       break ;
            case 'v' : FPRINTF(stderr, "%s version 1.4.%1d\n",
                                        progname, PATCHLEVEL) ;
                       break ;
            case '?' : usage() ;
          }
      INC ;
    }
}


getparam(s, argv, errmes)
char *s, *argv[], *errmes ;
{
  if (*argv != NULL && argv[0][0] != '-') STRCPY(s, *argv) ;
  else
    { 
      FPRINTF(stderr,"%s: %s as next argument.\n", progname, errmes) ;
      exit(1) ;
    }
}


make_button(x, y, text, state)      /* Draw a text button. */
int x, y ;
char *text ;
enum image_type state ;
{
  enum sten_type stencil ;

  draw_image(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, state) ;
  width = get_strwidth(BFONT, text) ;
  if (state == B_NORMAL) stencil = STEN_ON ;
  else                   stencil = STEN_INV ;
  draw_text(x + ((64 - width) / 2),
            y + FONT_HEIGHT + ((BUTTON_HEIGHT - FONT_HEIGHT) / 2) - 5,
            stencil, BFONT, text) ;
}

 
make_display(output)       /* Output message to the display window. */
char *output ;
{
  STRCPY(last_message, output) ;  /* Save in case of a redraw. */
  make_message() ;                /* Output message as popup to the canvas. */
}


make_help(value)
{
  if (value)
    {
      STRCPY(helpname, help_strs[value-1]) ;
      if (isupper(helpname[0])) helpname[0] = tolower(helpname[0]) ;
      get_help() ;             /* Read the help file and display. */
      drawstate = DO_HELP ;
      butx = buty = 0 ;
    }
}


make_message()      /* Output message to the display window. */
{
  int x, y ;

  width = strlen(last_message) * font_width + 20 ;
  if (width < 300) width = 300 ;
  x = (SCAN_WIDTH - width) / 2 ;
  y = (SCAN_HEIGHT - 90) / 2 ;
  draw_frame(x, y, width, 90) ;
  draw_image(x + 30, y + 15, 64, 64, EXCLAIM_IMAGE) ;
  butx = x + width - 90 ;
  buty = y + 15 ;
  make_button(x + width - 90, y + 15, "OK", B_NORMAL) ;
  draw_text(x + 10, y + 65, STEN_OFF, NFONT, last_message) ;
  drawstate = DO_MESSAGE ;
}


make_scan()    /* Start scanning process, and wait for it to terminate. */
{
  char arg_contrast[MAXLINE] ;      /* Contrast value. */
  char arg_brightness[MAXLINE] ;    /* Brightness value. */
  char arg_datatrans[MAXLINE] ;     /* Data transfer (or blank). */
  char arg_framevals[4][MAXLINE] ;  /* Scanning frame values. */
  char arg_grain[MAXLINE] ;         /* Grain value. */
  char arg_mode[MAXLINE] ;          /* Mode (or blank). */
  char arg_picname[MAXLINE] ;       /* Picture name. */
  char arg_resolution[MAXLINE] ;    /* Resolution value. */
  char arg_speed[MAXLINE] ;         /* Speed of connection (or blank). */
  char arg_ttyport[MAXLINE] ;       /* Tty port (or blank). */

  SPRINTF(arg_brightness,    "%1d", brightness) ;
  SPRINTF(arg_contrast,      "%1d", contrast) ;
  SPRINTF(arg_datatrans,     "%1d", switches[DATA_TRANSFER]) ;
  SPRINTF(arg_framevals[X1], "%1d", framevals[X1]) ;
  SPRINTF(arg_framevals[Y1], "%1d", framevals[Y1]) ;
  SPRINTF(arg_framevals[X2], "%1d", framevals[X2]) ;
  SPRINTF(arg_framevals[Y2], "%1d", framevals[Y2]) ;
  SPRINTF(arg_grain,         "%1d", grain) ;
  SPRINTF(arg_mode,          "%1d", switches[MODE]) ;
  SPRINTF(arg_picname,       "%s",  picname) ;
  SPRINTF(arg_resolution,    "%1d", resolution) ;
  SPRINTF(arg_speed,         "%1d", switches[BAUD_RATE]) ;
  SPRINTF(arg_ttyport,       "%1d", switches[SERIAL_PORT]) ;

  if (scanning)
    {
      make_display("There is a scan already in progress") ;
      return ;
    }
  scanning = 1 ;
  make_button(SCAN_WIDTH-150, 4*SWITCH_HEIGHT+20, "Scan", B_INVERT) ;
  if ((pid = vfork()) == 0)
    {
      EXECL("scan"," ","-c", arg_contrast, "-b", arg_brightness,
                    "-d", arg_datatrans,  "-f", arg_framevals[X1],
                    arg_framevals[Y1], arg_framevals[X2], arg_framevals[Y2],
                    "-g", arg_grain,      "-m", arg_mode,
                    "-p", arg_picname,    "-r", arg_resolution,
                    "-s", arg_speed,      "-t", arg_ttyport,
                    (char *) 0) ;
      FPRINTF(stderr, "%s: Couldn't execute the \"scan\" program.\n", progname) ;
      exit(1) ;                 /* Should never get here. */
    }
  wait_on_child(pid) ;
}


make_set(value)
int value ;
{
  switch (value)
    {
      case SET_PICTURE     : STRCPY(old_picname, picname) ;
                             draw_picarea() ;
                             break ;
      case SET_DEFAULTS    : set_defaults() ;
                             break ;
      case SET_CLEAR_FRAME : framevals[X1] = 0 ;
                             framevals[Y1] = 0 ;
                             framevals[X2] = 68 ;
                             framevals[Y2] = 88 ;
                             draw_area(SCAN_FRAME_X+2, SCAN_FRAME_Y+2,
                                       34*LINEGAP-4, 44*LINEGAP-4, GCLR) ;
                             draw_area(SCAN_FRAME_X-9, SCAN_FRAME_Y,
                                       9, 46*LINEGAP-1, GCLR) ;
                             draw_area(SCAN_FRAME_X, SCAN_FRAME_Y-9,
                                       34*LINEGAP-1, 9, GCLR) ;
                             break ;
      case SET_NEW_FRAME   : select_frame() ;
                             break ;
      case SET_CURRENT     : show_settings() ;
    } 
}
 
 
make_switch(x, y, title, boxa, boxb)
int x, y ;
char *title, *boxa, *boxb ;
{
  draw_text(x, y, STEN_OFF, BFONT, title) ;
  set_switch(x, y+10, OFF) ;
  draw_text(x+30, y+20, STEN_OFF, BFONT, boxa) ;
  set_switch(x, y+30, OFF) ;
  draw_text(x+30, y+40, STEN_OFF, BFONT, boxb) ;
}


process_menu(mtype, value)        /* Process a menu choice. */
enum menu_type mtype ;
int value ;
{
  switch (mtype)
    {
      case BRIGHTNESS_M : brightness = value ;
                          break ;
      case CONTRAST_M   : contrast = value ;
                          break ;
      case GRAIN_M      : grain = value-1 ;
                          break ;
      case HELP_M       : make_help(value) ;
                          break ;
      case RESOLUTION_M : resolution = value-1 ;
                          break ;
      case SET_M        : make_set(value) ;
    }
}


set_defaults()       /* Return scanner setting to default. */
{
  brightness = 7 ;   /* Brightness value (no adjustment). */
  contrast = 7 ;     /* Contrast value (no adjustment). */
  grain = 2 ;        /* Grain value (grain size: 8x8  levels: 33). */
  resolution = 0 ;   /* Resolution value (300 dpi). */
}


show_settings()      /* Display current scanner settings in a popup window. */
{
  draw_settings() ;           /* Display setting values. */
  drawstate = DO_SETTINGS ;
  butx = buty = 0 ;
}


test_switch(x, y)       /* Test for a switch box being pressed. */
int x, y ;
{
  int i, j ;

  if (x > SCAN_WIDTH-150 && x < SCAN_WIDTH-130)
    for (i = 0; i < 2; i++)
      for (j = 0; j < 4; j++)
        if ((y > j*SWITCH_HEIGHT+50+i*20) && (y < j*SWITCH_HEIGHT+70+i*20))
          if (i)
            {   
              set_switch(SCAN_WIDTH-150, j*SWITCH_HEIGHT+50, OFF) ;
              set_switch(SCAN_WIDTH-150, j*SWITCH_HEIGHT+70, ON) ;
              switches[j] = i ;
            }
          else
            { 
              set_switch(SCAN_WIDTH-150, j*SWITCH_HEIGHT+50, ON) ;
              set_switch(SCAN_WIDTH-150, j*SWITCH_HEIGHT+70, OFF) ;
              switches[j] = i ;
            }
}


usage()
{
  FPRINTF(stderr, "Usage: %s: [-h helpname] [-v] [-?]\n", progname) ;
  exit(1) ;
}


wait_for_ok()    /* Wait for user to acknowledge. */
{
  int doexit = 0 ;

  switch (type)
    {
      case LEFT_DOWN : if (butx && buty)
                         {
                           if (curx > butx && curx < butx+BUTTON_WIDTH &&
                               cury > buty && cury < buty+BUTTON_HEIGHT)
                             {
                               make_button(butx, buty, "OK", B_INVERT) ;
                               doexit = 1 ;
                             }
                         }
                       else doexit = 1 ;
                       break ;
      case KEYBOARD  : if (cur_ch == LINEFEED || cur_ch == RETURN)
                         doexit = 1 ;
    }
 
  if (doexit)
    {
      set_cursor(MAIN_CUR) ;
      drawstate = DO_NOTHING ;
      do_repaint() ;
    }        
}
