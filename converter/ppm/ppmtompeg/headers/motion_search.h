/*===========================================================================*
 * search.h                                  *
 *                                       *
 *  stuff dealing with the motion search                     *
 *                                       *
 *===========================================================================*/

/*==============*
 * HEADER FILES *
 *==============*/



/*===========*
 * CONSTANTS *
 *===========*/

#define PSEARCH_SUBSAMPLE   0
#define PSEARCH_EXHAUSTIVE  1
#define PSEARCH_LOGARITHMIC 2
#define PSEARCH_TWOLEVEL    3

#define BSEARCH_EXHAUSTIVE  0
#define BSEARCH_CROSS2      1
#define BSEARCH_SIMPLE      2


/*========*
 * MACROS *
 *========*/

#define COMPUTE_MOTION_BOUNDARY(by,bx,stepSize,leftMY,leftMX,rightMY,rightMX)\
leftMY = -2*DCTSIZE*by; /* these are valid motion vectors */         \
leftMX = -2*DCTSIZE*bx;                          \
/* these are invalid motion vectors */       \
rightMY = 2*(Fsize_y - (by+2)*DCTSIZE + 1) - 1;              \
rightMX = 2*(Fsize_x - (bx+2)*DCTSIZE + 1) - 1;              \
\
if ( stepSize == 2 ) { \
    rightMY++;      \
    rightMX++;      \
    }

#define VALID_MOTION(m)   \
(((m).y >= leftMY) && ((m).y < rightMY) &&   \
 ((m).x >= leftMX) && ((m).x < rightMX) )


/*===============================*
 * EXTERNAL PROCEDURE prototypes *
 *===============================*/

void
SetPSearchAlg(const char * const alg);
void
SetBSearchAlg(const char * const alg);
const char *
BSearchName(void);
const char *
PSearchName(void);

int
PLogarithmicSearch(const LumBlock * const currentBlockP,
                   MpegFrame *      const prev,
                   int              const by,
                   int              const bx,
                   vector *         const motionP,
                   int              const searchRange);

int
PSubSampleSearch(const LumBlock * const currentBlockP,
                 MpegFrame *      const prev,
                 int              const by,
                 int              const bx,
                 vector *         const motionP,
                 int              const searchRange);

int
PLocalSearch(const LumBlock * const currentBlockP,
             MpegFrame *      const prev,
             int              const by,
             int              const bx,
             vector *         const motionP,
             int              const bestSoFar,
             int              const searchRange);

int
PTwoLevelSearch(const LumBlock * const currentBlockP,
                MpegFrame *      const prev,
                int              const by,
                int              const bx,
                vector *         const motionP,
                int              const bestSoFar,
                int              const searchRange);
void
PMotionSearch(const LumBlock * const currentBlockP,
              MpegFrame *      const prev,
              int              const by,
              int              const bx,
              vector *         const motionP);

void
MotionSearchPreComputation(MpegFrame * const frameP);

/*==================*
 * GLOBAL VARIABLES *
 *==================*/

extern int psearchAlg;

/*
 * Copyright (c) 1995 The Regents of the University of California.
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

/*
 *  $Header: /n/picasso/project/mpeg/mpeg_dist/mpeg_encode/headers/RCS/search.h,v 1.6 1995/08/15 23:43:36 smoot Exp $
 *  $Log: search.h,v $
 *  Revision 1.6  1995/08/15 23:43:36  smoot
 *  *** empty log message ***
 *
 *  Revision 1.5  1995/01/19 23:55:20  eyhung
 *  Changed copyrights
 *
 * Revision 1.4  1994/12/07  00:42:01  smoot
 * Added separate P and B search ranges
 *
 * Revision 1.3  1994/11/12  02:12:58  keving
 * nothing
 *
 * Revision 1.2  1993/07/22  22:24:23  keving
 * nothing
 *
 * Revision 1.1  1993/07/09  00:17:23  keving
 * nothing
 *
 */



