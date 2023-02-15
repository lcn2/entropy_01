#!/bin/make
#
# entropy_01 - determine the entropy from lines of "0" and "1" chars
#
# @(#) $Revision: 1.1 $
# @(#) $Id: Makefile,v 1.1 2002/03/26 21:39:13 chongo Exp $
# @(#) $Source: /usr/local/src/bin/entropy_01/RCS/Makefile,v $
#
# Please do not copyright this Makefile.  This Makefile is in the public domain.
#
# LANDON CURT NOLL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
# INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
# EVENT SHALL LANDON CURT NOLL BE LIABLE FOR ANY SPECIAL, INDIRECT OR
# CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
# USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
# OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.
#
# chongo <was here> /\oo/\
#
# Share and enjoy!

SHELL=/bin/sh
CC=cc
CFLAGS=-O3
BINMODE=0555
DESTBIN=/usr/local/bin
INSTALL=install

all: entropy_01

entropy_01: entropy_01.c chi_tbl.h
	${CC} ${CFLAGS} entropy_01.c -o $@

install: all
	${INSTALL} -m ${BINMODE} entropy_01 ${DESTBIN}

clean:
	rm -f entropy_01.o

clobber: clean
	-rm -f entropy_01
