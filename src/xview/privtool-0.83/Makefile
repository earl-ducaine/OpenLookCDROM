#
# @(#)Makefile	1.14 6/29/94
#

#
# In theory, you could have different code to replace mail_reader.c and
# x.c (e.g. for Motif, Xt, etc). In practice, that's not done yet.
#

MAIL_OBJECT = mail_reader.o
DISPLAY_OBJECT = x.o

OFILES = pgplib.o buffers.o $(MAIL_OBJECT) messages.o main.o gui.o \
	$(DISPLAY_OBJECT)

#
# Use the following lines if you don't have PGPTools.
#
PGPLDFLAGS=
PGPTOOLDIR=

#
# Use the following lines if you do have PGPTools, and set PGPTOOLDIR
# to the directory with the header files and libpgptools.a. You
# may want to simply create a link from this directory to the PGPTools
# directory.
#
#PGPTOOLDIR=./pgptools
#PGPLDFLAGS=-L$(PGPTOOLDIR) -lpgptools

#
# Set the following to the path for your PGP executable - can be blank
# if using PGPTools
#
#PGPEXEC=$(HOME)/bin/pgp

#
# Set the version number to claim in encrypted/signed messages if using
# PGPTools - can be blank if using PGP for everything. You'll probably
# want to set this to 2.5 now that has been released.
#
#PGPVERSION=2.3a
PGPVERSION=2.6

#
# Use the first lines if your OpenWindows files are installed in standard
# lib/include directories, or the second if they're in a tree under 
# $OPENWINHOME.
#

#OPENWINLDFLAGS=
#OPENWINCPPFLAGS=

OPENWINLDFLAGS=-L$(OPENWINHOME)/lib
OPENWINCPPFLAGS=-I$(OPENWINHOME)/include

LDFLAGS= -Bdynamic $(OPENWINLDFLAGS) -lxview -lolgx -lX11 -lm \
	$(PGPLDFLAGS)

#
# If you have PGPTools, use the first line, otherwise use the
# second. 
#
# You should modify the flags after -I$(PGPTOOLDIR) to match the
# flags you used when compiling libpgptools.a.
#
# Removing USE_HASH will save memory, but decrease performance. Using
# the public key hash table *significantly* improves the speed of
# signature verification for small messages when you have a large
# pubring.pgp.
#

#PGPTOOLS=-DPGPTOOLS -DUSE_HASH -I$(PGPTOOLDIR) -DUNIX -DDYN_ALLOC \
#	-DNO_ASM -DHIGHFIRST -DIDEA32
PGPTOOLS=

#
# Use -g for debugging, change to -O for production use
#

CFLAGS=-g -DPGPEXEC=\"$(PGPEXEC)\" -DPGPVERSION=\"$(PGPVERSION)\"
#CFLAGS=-O -DPGPEXEC=\"$(PGPEXEC)\" -DPGPVERSION=\"$(PGPVERSION)\"

#
# Note: Keep -DSAFE until you are sure of correct operation on
# your machine !
#
# The SunOS strstr() code seems to take an age, so we define CRAP_STRSTR
# to use the simple C implementation in pgplib.c. Add -DACCEPT_PATH if
# you want to support Usenet archives that use Path: instead of From,
# but be aware that this may cause problems with mail....
#

#CPPFLAGS=$(OPENWINCPPFLAGS) -DSAFE $(PGPTOOLS) -DCRAP_STRSTR
CPPFLAGS=$(OPENWINCPPFLAGS) $(PGPTOOLS) -DCRAP_STRSTR 

#
# Code is written for cc, but should work with gcc. However, I'm wary
# of trying to get the Xview code to with with gcc as I've had problems
# with that in the past.
#

CC=gcc -g
#CC=cc -DNON_ANSI

# Or, use acc for Solaris 2.x
#CC=acc -DSYSV

#
# Following provides automatic dependencies on SunOS
#

.KEEP_STATE:

#
# Actually do the compilation...
#

all:		 privtool

privtool:	$(OFILES)
		$(CC) $(CFLAGS) $(OFILES) -o privtool $(LDFLAGS)

.c.o:	
		$(CC) -c $< -o $@ $(CPPFLAGS) $(CFLAGS)


