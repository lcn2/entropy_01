#!/bin/make
#
# entropy_01 - determine the entropy from lines of "0" and "1" chars
#
# Copyright (c) 2003-2006,2023 by Landon Curt Noll.  All Rights Reserved.
#
# Permission to use, copy, modify, and distribute this software and
# its documentation for any purpose and without fee is hereby granted,
# provided that the above copyright, this permission notice and text
# this comment, and the disclaimer below appear in all of the following:
#
#       supporting documentation
#       source copies
#       source works derived from this source
#       binaries derived from this source or from derived source
#
# LANDON CURT NOLL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
# INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
# EVENT SHALL LANDON CURT NOLL BE LIABLE FOR ANY SPECIAL, INDIRECT OR
# CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
# USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
# OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.
#
# chongo (Landon Curt Noll, http://www.isthe.com/chongo/index.html) /\oo/\
#
# Share and enjoy! :-)

SHELL= bash
CC= cc
CFLAGS= -O3 -g3
BINMODE= 0555
DESTBIN= /usr/local/bin
INSTALL= install

all: entropy_01

entropy_01: entropy_01.c chi_tbl.h
	${CC} ${CFLAGS} entropy_01.c -o $@

install: all
	${INSTALL} -m ${BINMODE} entropy_01 ${DESTBIN}

clean:
	${RM} -f entropy_01.o

clobber: clean
	${RM} -f entropy_01
