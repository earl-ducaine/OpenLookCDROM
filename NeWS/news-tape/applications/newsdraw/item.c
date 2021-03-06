/* $Header: item.c,v 1.4 88/12/02 10:43:32 bvs Exp $ */
/* Copyright (C) 1988 by Sun Microsystems. All rights reserved. */

#include <stdio.h>
#include <math.h>
#include "go.h"
#include "gopvt.h"

extern PROP *GOItemProp();
extern ITEM *GOItemRotatedBounds();
static ITEM *plist;

int GOItemType(pitem) ITEM *pitem; { return(pitem->type); }

GOItemInit()
  {
	plist = (ITEM *)malloc(sizeof(ITEM));
	plist->pnext = plist;
	plist->pprev = plist;
  }
GOItemReInit()
  {
	GOItemInit();
  }

ITEM *GOItemAddToTop(pitem)
ITEM *pitem;
  {
	pitem->pprev = plist;
	pitem->pnext = plist->pnext;
	plist->pnext->pprev = pitem;
	plist->pnext = pitem;
	return(pitem);
  }
ITEM *GOItemAddToBottom(pitem)
ITEM *pitem;
  {
	pitem->pprev = plist->pprev;
	pitem->pnext = plist;
	plist->pprev->pnext = pitem;
	plist->pprev = pitem;
	return(pitem);
  }
ITEM *GOItemDelete(pitem)
ITEM *pitem;
  {
	pitem->pprev->pnext = pitem->pnext;
	pitem->pnext->pprev = pitem->pprev;
	pitem->pnext = (ITEM *)0;
	pitem->pprev = (ITEM *)0;
	return(pitem);
  }
ITEM *GODrawItem(pitem)
ITEM *pitem;
  {
	int x = (X0 + X1)/2;
	int y = (Y0 + Y1)/2;
	ps_dorotate(x, y, pitem->rotation);
	goprocs[pitem->type].draw(pitem);
	ps_unrotate();
	return(pitem);
  }
int GOPointInItem(pitem, x, y)
ITEM *pitem;
int x;
int y;
  {
	int x0 = X0;
	int x1 = X1;
	int y0 = Y0;
	int y1 = Y1;
	int xpt = x;
	int ypt = y;
	/* make sure we're at least 5 units wide! */
	if((x1 - x0) < 5) { x0--; x1++; }
	if((x1 - x0) < 5) { x0--; x1++; }
	if((x1 - x0) < 5) { x0--; x1++; }
	if((y1 - y0) < 5) { y0--; y1++; }
	if((y1 - y0) < 5) { y0--; y1++; }
	if((y1 - y0) < 5) { y0--; y1++; }
	if(pitem->rotation != 0.0)
	  {
		int xc = (x0 + x1)/2;
		int yc = (y0 + y1)/2;
		double fcos, fsin;

		sincos((double)(-pitem->rotation) / 180.0 *  M_PI, &fsin, &fcos);

		xpt = ((x - xc) * fcos) - ((y - yc) * fsin) + xc;
		ypt = ((x - xc) * fsin) + ((y - yc) * fcos) + yc;
	  }
	if((x0 <= xpt) && (x1 >= xpt) && (y0 <= ypt) && (y1 >= ypt))
		{
		  /* More detailed check... */
		  return(1);
		}
	else
		return(0);
  }

int GOItemInRect(pitem, x0, y0, x1, y1)
ITEM *pitem;
int x0, y0, x1, y1;
  {
	int xmin = MIN(X0, X1);
	int xmax = MAX(X0, X1);
	int ymin = MIN(Y0, Y1);
	int ymax = MAX(Y0, Y1);
	if(pitem->rotation != 0.0)
	  {
		GOItemRotatedBounds(pitem, &xmin, &ymin, &xmax, &ymax);
	  }
	if( (x0 > xmax) || (x1 < xmin) || (y0 > ymax) || (y1 < ymin) )
	  {
		return(0);
	  }
	else
		return(1);
  }
ITEM *GOPrintItem(pitem, pfile)
ITEM *pitem;
FILE *pfile;
  {
	if(pitem->rotation != 0.0)
	  {
		fprintf(pfile, "matrix currentmatrix ");
		fprintf(pfile, "%d %d translate ", X, Y);
		fprintf(pfile, "%f rotate ", pitem->rotation);
		fprintf(pfile, "%d %d translate\n", -X, -Y);
		goprocs[pitem->type].print(pitem, pfile);
		fprintf(pfile, "setmatrix\n");
	  }
	else
		goprocs[pitem->type].print(pitem, pfile);
	return(pitem);
  }
ITEM *GOWriteItem(pitem, pfile)
ITEM *pitem;
FILE *pfile;
  {
	fprintf(pfile, "type %d\n", pitem->type);
	PropWrite(GOItemProp(pitem), pfile);
	fprintf(pfile, "%d %d %d %d %f %d %d\n",
		pitem->x0,
		pitem->y0,
		pitem->x1,
		pitem->y1,
		pitem->rotation,
		pitem->xscale,
		pitem->yscale
		);
	goprocs[pitem->type].write(pitem, pfile);
	return(pitem);
  }
ITEM *GOReadItem(pfile)
FILE *pfile;
  {
	int ret;
	int type;
	ITEM *pitem;

	if(feof(pfile))
		return((ITEM *)0);

	if(1 != fscanf(pfile, "type %d\n", &type))
	  {
		fprintf(stderr, "Bad read of item\n");
		return((ITEM *)0);
	  }

	if(type == -1)
		return((ITEM *)0);

	pitem	= (ITEM *)goprocs[type].holder();
	pitem->type = type;
	pitem->prop = PropRead(pfile);
	if(pitem->prop == 0)
	  {
		return((ITEM *)0);
	  }

	/* fprintf(stderr, "reading item type: %d\n", type); */
	ret = fscanf(pfile, "%d %d %d %d %f %d %d\n",
		&pitem->x0,
		&pitem->y0,
		&pitem->x1,
		&pitem->y1,
		&pitem->rotation,
		&pitem->xscale,
		&pitem->yscale
		);
	if(ret != 7)
	  {
		fprintf(stderr, "Bad item type %d read...\n", type);
		return((ITEM *)0);
	  }
	goprocs[type].read(pitem, pfile);
	return(pitem);
  }

PROP * GOItemProp(pitem)
ITEM *pitem;
  {
	return(pitem->prop);
  }

ITEM * GOItemSetProp(pitem, prop)
ITEM *pitem;
PROP *prop;
  {
	pitem->prop = prop;
	return(pitem);
  }


ITEM *GOItemMove(pitem, dx, dy)
ITEM *pitem;
int dx, dy;
  {
	pitem->x0 += dx;
	pitem->x1 += dx;
	pitem->y0 += dy;
	pitem->y1 += dy;
	return(pitem);
  }
ITEM *GOItemSetRotation(pitem, angle)
ITEM *pitem;
float angle;
  {
	pitem->rotation = angle;
	return(pitem);
  }
float GOItemGetRotation(pitem)
ITEM *pitem;
  {
	return (float)pitem->rotation;
  }

minmax4(pmin, pmax, in1, in2, in3, in4)
int *pmin, *pmax;
int in1, in2, in3, in4;
  {
	*pmin = MIN( MIN(in1, in2), MIN(in3, in4));
	*pmax = MAX( MAX(in1, in2), MAX(in3, in4));
  }
ITEM *GOItemUnrotatedBounds(pitem, px0, py0, px1, py1)
ITEM *pitem;
int *px0, *py0, *px1, *py1;
  {
	int width = PropLineWidth(pitem->prop);
	*px0 = MIN(X0, X1) - width;
	*py0 = MIN(Y0, Y1) - width;
	*px1 = MAX(X0, X1) + width;
	*py1 = MAX(Y0, Y1) + width;
	return(pitem);
  }
ITEM *GOItemRotatedBounds(pitem, px0, py0, px1, py1)
ITEM *pitem;
int *px0, *py0, *px1, *py1;
  {
	if(pitem->rotation != 0.0)
	  {
		int width = PropLineWidth(pitem->prop);
		int xc = (X0 + X1 + 1)/2;
		int yc = (Y0 + Y1 + 1)/2;
		double fsin, fcos;
		int dx1, dy1, dx2, dy2;
		sincos((double)(pitem->rotation) / 180.0 *  M_PI, &fsin, &fcos);
		dx1 = ((X1 - xc) * fcos) - ((Y0 - yc) * fsin);
		dx2 = ((X1 - xc) * fcos) - ((Y1 - yc) * fsin);
		dy1 = ((X1 - xc) * fsin) + ((Y0 - yc) * fcos);
		dy2 = ((X1 - xc) * fsin) + ((Y1 - yc) * fcos);

		*px0 = xc - MAX( ABS(dx1), ABS(dx2) ) - width;
		*py0 = yc - MAX( ABS(dy1), ABS(dy2) ) - width;

		*px1 = xc + MAX( ABS(dx1), ABS(dx2) ) + width;
		*py1 = yc + MAX( ABS(dy1), ABS(dy2) ) + width;
	  }
	else
		GOItemUnrotatedBounds(pitem, px0, py0, px1, py1);
	return(pitem);
  }
ITEM *GOItemMaxBounds(pitem, px0, py0, px1, py1)
ITEM *pitem;
int *px0, *py0, *px1, *py1;
  {
	/* just do a rough calculation */
	int width = PropLineWidth(pitem->prop);
	int xc = (X0 + X1 + 1)/2;
	int yc = (Y0 + Y1 + 1)/2;
	int radius = (int)((MAX( ABS(X0 - X1), ABS(Y0 - Y1) ) + 1) * .75);
	radius += width;	/* this should really be the linewidth */
	*px0 = xc - radius;
	*px1 = xc + radius;
	*py0 = yc - radius;
	*py1 = yc + radius;
	return(pitem);
  }

ITEM *GOFindItem(x, y)
int x, y;
  {
	ITEM *pitem;
	for(pitem = plist->pnext; pitem != plist; pitem = pitem->pnext)
	  {
		if(GOPointInItem(pitem, x, y))
			  return(pitem);
	  }
  	return((ITEM *)0);
  }

GORepairItems(x0, y0, x1, y1)
  {
	ITEM *pitem;
	ps_pushclip(x0, y0, x1, y1);
	ps_setgray(1.0);
	ps_drawrect(x0, y0, x1, y1);
	ps_fill();
	for(pitem = plist->pprev; pitem != plist; pitem = pitem->pprev)
	  {
		if(GOItemInRect(pitem, x0, y0, x1, y1))
			GODrawItem(pitem);
	  }
	ps_popclip();
  }
GODrawItems()
  {
	ITEM *pitem;
	for(pitem = plist->pprev; pitem != plist; pitem = pitem->pprev)
		GODrawItem(pitem);
  }
GOBoundsItems(px0, py0, px1, py1)
int *px0, *py0, *px1, *py1;
  {
	ITEM *pitem = plist->pprev;
	int x0 = X0;
	int y0 = Y0;
	int x1 = X1;
	int y1 = Y1;
	for(pitem = plist->pprev; pitem != plist; pitem = pitem->pprev)
	  {
		x0 = MIN(x0, X0);
		y0 = MIN(y0, Y0);
		x1 = MAX(x1, X1);
		y1 = MAX(y1, Y1);
	  }
	*px0 = x0;
	*py0 = y0;
	*px1 = x1;
	*py1 = y1;
  }
GOPrintItems(pfile)
FILE *pfile;
  {
	ITEM *pitem;
	for(pitem = plist->pprev; pitem != plist; pitem = pitem->pprev)
		GOPrintItem(pitem, pfile);
  }
GOWriteItems(pfile)
FILE *pfile;
  {
	ITEM *pitem;
	for(pitem = plist->pprev; pitem != plist; pitem = pitem->pprev)
	  {
		GOWriteItem(pitem, pfile);
	  }
	fprintf(pfile, "type %d\n", -1);
  }
GOReadItems(pfile)
FILE *pfile;
  {
	ITEM *pitem;
	GOItemReInit();
	while(pitem = GOReadItem(pfile))
	  {
	 	GOItemAddToTop(pitem);
	  }
	GORepairItems(-1000, -1000, 1000, 1000);
  }

