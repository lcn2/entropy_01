/*
 * entropy_01 - determine the entropy from lines of "0" and "1" chars
 *
 * @(#) $Revision: 1.3 $
 * @(#) $Id: entropy_01.c,v 1.3 2002/03/02 11:14:03 chongo Exp $
 * @(#) $Source: /home/chongo/bench/sample/RCS/entropy_01.c,v $
 *
 * We will perform tally stats on a collection of lines containing the
 * same number of "0" and "1" characters.
 *
 * usage:
 *	cat lines | ./entropy_01 [-l maxlen] [-i ignsize]
 *
 *	-l maxlen	do not scan more than maxlen octets per line
 *	-i ignsize	ignore files smaller than ignsize octets
 *
 * NOTE: We will consider anything not a "0" char to be a "1" character.
 *
 * Copyright (c) 2001 by Landon Curt Noll.  All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright, this permission notice and text
 * this comment, and the disclaimer below appear in all of the following:
 *
 *       supporting documentation
 *       source copies
 *       source works derived from this source
 *       binaries derived from this source or from derived source
 *
 * LANDON CURT NOLL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
 * EVENT SHALL LANDON CURT NOLL BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * chongo <was here> /\oo/\
 *
 * Share and enjoy!
 */


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include "chi_tbl.h"

/*
 * forward declarations
 */
typedef long long llng;
static int chi1_slot(long data0, long data1);


/*
 * private globals
 */
static char *usage =
"usage: %s [-m maxlen] [-i ignsize]\n"
"\n"
"\t-m maxlen	do not scan more than maxlen octets per line (def: BUFSIZ)\n"
"\t-i ignsize	ignore lines smaller than ignsize octets (def: 0)\n";
static char *program;	/* our name */


int
main(int argc, char *argv[])
{
    char line[BUFSIZ+1];	/* line buffer */
    long bit_sum[BUFSIZ+1];	/* bit by bit sum of values */
    long maxlen;		/* maxlen octets to process per line */
    long ignsize;		/* ignore lines shorter than ignsize chars */
    long chi_lvl_tally[CHI_PROB];	/* tally of chi-square levels found */
    long linecnt;		/* number of lines processed */
    long minlen;		/* length of shortest non-ignroed line */
    long skipcnt;		/* number of lines ignored or skipped */
    llng bitcnt;		/* number of "0" and "1" char bits processed */
    double entropy;		/* entropy estimate */
    extern char *optarg;	/* option argument */
    extern int optind;		/* argv index of the next arg */
    int i;

    /*
     * setup
     */
    maxlen = BUFSIZ;
    skipcnt = 0;
    ignsize = 0;
    linecnt = 0;
    for (i=0; i < BUFSIZ+1; ++i) {
	bit_sum[i] = 0;
    }

    /*
     * parse args
     */
    program = argv[0];
    while ((i = getopt(argc, argv, "m:i:")) != -1) {
        switch (i) {
	case 'm':
	    maxlen = strtol(optarg, NULL, 0);
	    if (maxlen <= 0 || maxlen > BUFSIZ) {
	    	fprintf(stderr, "%s: -m value: %ld must be > 0 and <= %d\n",
				program, maxlen, BUFSIZ);
		exit(1);
	    }
	    break;
	case 'i':
	    ignsize = strtol(optarg, NULL, 0);
	    if (ignsize <= 0) {
	    	fprintf(stderr, "%s: -i value: %ld must be > 0\n",
				program, ignsize);
		exit(3);
	    }
	    break;
	default:
	    fprintf(stderr, usage, program);
	    exit(4);
	}
    }

    /*
     * read lines
     */
    minlen = maxlen;
    while (fgets(line, BUFSIZ, stdin) != NULL) {
	int len;	/* current line length */
	char *p;

	/*
	 * firewall
	 */
    	line[BUFSIZ] = '\0';

	/*
	 * convert the trailing newline or return to a NUL
	 */
	p = strrchr(line, '\n');
	if (p != NULL) {
	    *p = '\0';
	}
	p = strrchr(line, '\r');
	if (p != NULL) {
	    *p = '\0';
	}

	/*
	 * ignore lines that are too small
	 */
	len = strlen(line);
	if (len <= ignsize) {
	    ++skipcnt;
	    continue;
	}
	if (len < minlen) {
	    minlen = len;
	}

	/*
	 * count "1"'s on the line until EOL or end of buffer or minlen
	 */
	for (i=0; line[i] != '\0' && line[i] != '\n' && i < minlen; ++i) {
	    if (line[i] == '1') {
		++bit_sum[i];
	    }
	}

	/*
	 * count line
	 */
	++linecnt;
    }

    /*
     * empty or single count firewall
     */
    if (linecnt <= 0) {
	printf("no lines longer than %d octets\n", ignsize);
    } else if (linecnt < 2) {
	printf("only 1 line longer than %d octets\n", ignsize);
    }

    /*
     * chi-square level tally each bit count
     */
    for (i=0; i < minlen; ++i) {
	int level;		/* return from chi_slot() */

    	/*
	 * determine the chi-square level
	 */
	level = chi1_slot(bit_sum[i], linecnt - bit_sum[i]);

	/*
	 * tally valid levels
	 */
	if (level >= 0) {
	    ++chi_lvl_tally[level];
	} else {
	    ++chi_lvl_tally[CHI_PROB-1];
	}
    }

    /*
     * print stats
     */
    printf("\nchi^2 distribution\n");
    printf("lines counted: %d\n", linecnt);
    printf("lines ignored: %d\n", skipcnt);
    printf("shortest processed line: %d octets\n", minlen);
    printf("octets processed: %lld octets\n\n", (llng)minlen*(llng)linecnt);
    entropy = 0.0;
    for (i=0; i < CHI_PROB-1; ++i) {
	printf("chi^2 [%.2f-%.2f]%%:\t%ld\t%6.2f%%\n",
	       100.0*chiprob[i+1], 100.0*chiprob[i], chi_lvl_tally[i],
	       100.0*(double)chi_lvl_tally[i]/((double)minlen));
    	entropy += (double)chiprob[i+1]*(double)chi_lvl_tally[i];
    }
    printf("chi^2 [excess]:\t\t%ld\t%6.2f%%\n",
	chi_lvl_tally[CHI_PROB-1],
	100.0*(double)chi_lvl_tally[CHI_PROB-1]/((double)minlen));
    printf("\nentropy bit est: %ld bits\t%6.2f%%\n\n",
	   (long)entropy, 100.0*entropy/((double)minlen));

    /*
     * all done
     */
    exit(0);
}


/*
 * chi1_slot - determine the prob slot for 1 degree of freedom
 *
 * given:
 *	slot0		1st data count
 *	slot1		2nd data count
 *
 * returns:
 *	i, such that there between a:
 *
 *		chiprob[i+1] and a chiprob[i]
 *
 *	chance that the data is random.  That is, the percentage is in
 *	the range:
 *
 *		100*chiprob[i] <= actual_percentage < 100*chiprob[i+1]
 */
static int
chi1_slot(long slot0, long slot1)
{
    double num_samples;		/* total number of samples */
    double expected;		/* expected number of samples */
    double chisquare;		/* chi-square value of data */
    int i;

    /*
     * determine the expected number per sample
     */
    num_samples = (double)slot0 + (double)slot1;
    expected = num_samples / 2.0;
    if (expected == 0.0) {
	fprintf(stderr, "%s: chi1_slot 0 count\n", program);
	exit(18);
    }

    /*
     * calculate chi-square
     */
    chisquare = (((double)slot0-expected) * ((double)slot0-expected) +
    		 ((double)slot1-expected) * ((double)slot1-expected)) /
		 expected;

    /*
     * determine the chi-square levels we are between
     *
     * The last entry in chi_tbl[1][CHI_PROB-1] is a sentinel.
     */
    for (i=CHI_PROB-2; i >= 0; --i) {
	if (chisquare >= chi_tbl[1][i]) {
	    return i;
	}
    }
    return 0;
}
