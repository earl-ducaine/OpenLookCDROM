#
# This file is a product of Sun Microsystems, Inc. and is provided for
# unrestricted use provided that this legend is included on all tape
# media and as a part of the software program in whole or part.
# Users may copy, modify or distribute this file at will.
#
# THIS FILE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
# WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
# PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
#
# This file is provided with no support and without any obligation on the
# part of Sun Microsystems, Inc. to assist in its use, correction,
# modification or enhancement.
#
# SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
# INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY THIS FILE
# OR ANY PART THEREOF.
#
# In no event will Sun Microsystems, Inc. be liable for any lost revenue
# or profits or other special, indirect and consequential damages, even
# if Sun has been advised of the possibility of such damages.
#
# Sun Microsystems, Inc.
# 2550 Garcia Avenue
# Mountain View, California  94043

NEWSDIR = /usr/NeWS
#TARGET_ARCH=-mc68020
TARGET_ARCH=-sparc
#TARGET_ARCH=-i386

CFLAGS = -O $(TARGET_ARCH) -I$(NEWSDIR)/include
CPS = $(NEWSDIR)/bin/cps
LIBS = $(NEWSDIR)/lib/libcps.a

OBJS = sunclock.o
COMMAND = sunclock

$(COMMAND): $(OBJS)
	cc -o $(COMMAND) $(CFLAGS) $(OBJS) $(LIBS)

sunclock.o: sunclock.c sunclock.h

clean:
	rm -f sunclock core $(OBJS) sunclock.h *.BAK *~

.SUFFIXES: .h .cps
.cps.h:
	$(CPS) $*.cps
