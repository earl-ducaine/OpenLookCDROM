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
#ifndef	lbmain_HEADER
#define	lbmain_HEADER

/*
 * lbmain_ui.h - User interface object and function declarations.
 * This file was generated by `gxv' from `lbmain.G'.
 * DO NOT EDIT BY HAND.
 */

extern Attr_attribute	INSTANCE;


typedef struct {
	Xv_opaque	locbif_pu;
	Xv_opaque	lb_main_cntl;
	Xv_opaque	lbforw;
	Xv_opaque	lbcontinue;
	Xv_opaque	lbback;
	Xv_opaque	lbbifmode;
	Xv_opaque	lbpause;
	Xv_opaque	lbparamsel;
	Xv_opaque	lbcontpu;
	Xv_opaque	lbsolverp;
	Xv_opaque	lbdisplayp;
	Xv_opaque	lbstatepu;
	Xv_opaque	lbactvarb;
	Xv_opaque	lbmode;
	Xv_opaque	lbdirect;
	Xv_opaque	lbfilerd;
	Xv_opaque	lbfilename;
} lbmain_locbif_pu_objects;

extern lbmain_locbif_pu_objects	*lbmain_locbif_pu_objects_initialize();

extern Xv_opaque	lbmain_locbif_pu_locbif_pu_create();
extern Xv_opaque	lbmain_locbif_pu_lb_main_cntl_create();
extern Xv_opaque	lbmain_locbif_pu_lbforw_create();
extern Xv_opaque	lbmain_locbif_pu_lbcontinue_create();
extern Xv_opaque	lbmain_locbif_pu_lbback_create();
extern Xv_opaque	lbmain_locbif_pu_lbbifmode_create();
extern Xv_opaque	lbmain_locbif_pu_lbpause_create();
extern Xv_opaque	lbmain_locbif_pu_lbparamsel_create();
extern Xv_opaque	lbmain_locbif_pu_lbcontpu_create();
extern Xv_opaque	lbmain_locbif_pu_lbsolverp_create();
extern Xv_opaque	lbmain_locbif_pu_lbdisplayp_create();
extern Xv_opaque	lbmain_locbif_pu_lbstatepu_create();
extern Xv_opaque	lbmain_locbif_pu_lbactvarb_create();
extern Xv_opaque	lbmain_locbif_pu_lbmode_create();
extern Xv_opaque	lbmain_locbif_pu_lbdirect_create();
extern Xv_opaque	lbmain_locbif_pu_lbfilerd_create();
extern Xv_opaque	lbmain_locbif_pu_lbfilename_create();
#endif
