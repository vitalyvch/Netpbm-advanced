/* pnminvert.c - read a portable anymap and invert it
**
** Copyright (C) 1989 by Jef Poskanzer.
**
** Permission to use, copy, modify, and distribute this software and its
** documentation for any purpose and without fee is hereby granted, provided
** that the above copyright notice appear in all copies and that both that
** copyright notice and this permission notice appear in supporting
** documentation.  This software is provided "as is" without express or
** implied warranty.
*/

#include "pnm.h"

/* Implementation note: A suitably advanced compiler, such as Gcc 4,
   implements the for statements in our algorithm with instructions that do 16
   bytes at a time on CPUs that have them (movdqa on x86).  This is "tree
   vectorization."  A more primitive compiler will do one byte at a time; we
   could change the code to use uint32_t or uint64_t and it will do four or
   eight bytes at a time.  (But we don't think it's worth complicating the
   code for that).
*/



#define CHARBITS (sizeof(unsigned char)*8)



static void
invertPbm(FILE * const ifP,
          FILE * const ofP,
          int    const cols,
          int    const rows,
          int    const format) {
/*----------------------------------------------------------------------------
   Invert a PBM image.  Use the "packed" PBM functions for speed.
-----------------------------------------------------------------------------*/
    int const colChars = pbm_packed_bytes(cols);

    unsigned char * bitrow;
    unsigned int row;

    bitrow = pbm_allocrow_packed(cols);

    for (row = 0; row < rows; ++row) {
        unsigned int colChar;

        pbm_readpbmrow_packed(ifP, bitrow, cols, format);
        for (colChar = 0; colChar < colChars; ++colChar)
            bitrow[colChar] = ~ bitrow[colChar];

        /* Clean off remainder of fractional last character and write */
        pbm_cleanrowend_packed(bitrow, cols);
        pbm_writepbmrow_packed(ofP, bitrow, cols, 0);
    }
    pbm_freerow_packed(bitrow);
}



static void
invertPnm(FILE * const ifP,
          FILE * const ofP,
          int    const cols,
          int    const rows,
          xelval const maxval,
          int    const format) {

    xel * xelrow;
    unsigned int row;

    xelrow = pnm_allocrow(cols);

    for (row = 0; row < rows; ++row) {
        unsigned int col;
        pnm_readpnmrow(ifP, xelrow, cols, maxval, format);
        for (col = 0; col < cols; ++col)
            pnm_invertxel(&xelrow[col], maxval, format);

        pnm_writepnmrow(ofP, xelrow, cols, maxval, format, 0);
    }
    pnm_freerow(xelrow);
}



int
main(int argc, char * argv[]) {
    FILE* ifP;
    xelval maxval;
    int rows, cols, format;

    pnm_init(&argc, argv);

    if (argc-1 > 1)
        pm_error("There is at most 1 argument - the input file name.  "
                 "You specified %d", argc-1);
    if (argc-1 == 1)
        ifP = pm_openr(argv[1]);
    else
        ifP = stdin;

    pnm_readpnminit(ifP, &cols, &rows, &maxval, &format);
    pnm_writepnminit(stdout, cols, rows, maxval, format, 0);

    if (PNM_FORMAT_TYPE(format) == PBM_TYPE)
        /* Take fast path */
        invertPbm(ifP, stdout, cols, rows, format);
    else
        /* PPM , PGM  (logic also works for PBM) */
        invertPnm(ifP, stdout, cols, rows, maxval, format);

    pm_close(ifP);
    pm_close(stdout);

    return 0;
}


