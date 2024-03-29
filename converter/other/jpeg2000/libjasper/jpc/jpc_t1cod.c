/*
 * Copyright (c) 1999-2000 Image Power, Inc. and the University of
 *   British Columbia.
 * Copyright (c) 2001-2002 Michael David Adams.
 * All rights reserved.
 */

/* __START_OF_JASPER_LICENSE__
 *
 * JasPer Software License
 *
 * IMAGE POWER JPEG-2000 PUBLIC LICENSE
 * ************************************
 *
 * GRANT:
 *
 * Permission is hereby granted, free of charge, to any person (the "User")
 * obtaining a copy of this software and associated documentation, to deal
 * in the JasPer Software without restriction, including without limitation
 * the right to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the JasPer Software (in source and binary forms),
 * and to permit persons to whom the JasPer Software is furnished to do so,
 * provided further that the License Conditions below are met.
 *
 * License Conditions
 * ******************
 *
 * A.  Redistributions of source code must retain the above copyright notice,
 * and this list of conditions, and the following disclaimer.
 *
 * B.  Redistributions in binary form must reproduce the above copyright
 * notice, and this list of conditions, and the following disclaimer in
 * the documentation and/or other materials provided with the distribution.
 *
 * C.  Neither the name of Image Power, Inc. nor any other contributor
 * (including, but not limited to, the University of British Columbia and
 * Michael David Adams) may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * D.  User agrees that it shall not commence any action against Image Power,
 * Inc., the University of British Columbia, Michael David Adams, or any
 * other contributors (collectively "Licensors") for infringement of any
 * intellectual property rights ("IPR") held by the User in respect of any
 * technology that User owns or has a right to license or sublicense and
 * which is an element required in order to claim compliance with ISO/IEC
 * 15444-1 (i.e., JPEG-2000 Part 1).  "IPR" means all intellectual property
 * rights worldwide arising under statutory or common law, and whether
 * or not perfected, including, without limitation, all (i) patents and
 * patent applications owned or licensable by User; (ii) rights associated
 * with works of authorship including copyrights, copyright applications,
 * copyright registrations, mask work rights, mask work applications,
 * mask work registrations; (iii) rights relating to the protection of
 * trade secrets and confidential information; (iv) any right analogous
 * to those set forth in subsections (i), (ii), or (iii) and any other
 * proprietary rights relating to intangible property (other than trademark,
 * trade dress, or service mark rights); and (v) divisions, continuations,
 * renewals, reissues and extensions of the foregoing (as and to the extent
 * applicable) now existing, hereafter filed, issued or acquired.
 *
 * E.  If User commences an infringement action against any Licensor(s) then
 * such Licensor(s) shall have the right to terminate User's license and
 * all sublicenses that have been granted hereunder by User to other parties.
 *
 * F.  This software is for use only in hardware or software products that
 * are compliant with ISO/IEC 15444-1 (i.e., JPEG-2000 Part 1).  No license
 * or right to this Software is granted for products that do not comply
 * with ISO/IEC 15444-1.  The JPEG-2000 Part 1 standard can be purchased
 * from the ISO.
 *
 * THIS DISCLAIMER OF WARRANTY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE.
 * NO USE OF THE JASPER SOFTWARE IS AUTHORIZED HEREUNDER EXCEPT UNDER
 * THIS DISCLAIMER.  THE JASPER SOFTWARE IS PROVIDED BY THE LICENSORS AND
 * CONTRIBUTORS UNDER THIS LICENSE ON AN ``AS-IS'' BASIS, WITHOUT WARRANTY
 * OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION,
 * WARRANTIES THAT THE JASPER SOFTWARE IS FREE OF DEFECTS, IS MERCHANTABLE,
 * IS FIT FOR A PARTICULAR PURPOSE OR IS NON-INFRINGING.  THOSE INTENDING
 * TO USE THE JASPER SOFTWARE OR MODIFICATIONS THEREOF FOR USE IN HARDWARE
 * OR SOFTWARE PRODUCTS ARE ADVISED THAT THEIR USE MAY INFRINGE EXISTING
 * PATENTS, COPYRIGHTS, TRADEMARKS, OR OTHER INTELLECTUAL PROPERTY RIGHTS.
 * THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE JASPER SOFTWARE
 * IS WITH THE USER.  SHOULD ANY PART OF THE JASPER SOFTWARE PROVE DEFECTIVE
 * IN ANY RESPECT, THE USER (AND NOT THE INITIAL DEVELOPERS, THE UNIVERSITY
 * OF BRITISH COLUMBIA, IMAGE POWER, INC., MICHAEL DAVID ADAMS, OR ANY
 * OTHER CONTRIBUTOR) SHALL ASSUME THE COST OF ANY NECESSARY SERVICING,
 * REPAIR OR CORRECTION.  UNDER NO CIRCUMSTANCES AND UNDER NO LEGAL THEORY,
 * WHETHER TORT (INCLUDING NEGLIGENCE), CONTRACT, OR OTHERWISE, SHALL THE
 * INITIAL DEVELOPER, THE UNIVERSITY OF BRITISH COLUMBIA, IMAGE POWER, INC.,
 * MICHAEL DAVID ADAMS, ANY OTHER CONTRIBUTOR, OR ANY DISTRIBUTOR OF THE
 * JASPER SOFTWARE, OR ANY SUPPLIER OF ANY OF SUCH PARTIES, BE LIABLE TO
 * THE USER OR ANY OTHER PERSON FOR ANY INDIRECT, SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES OF ANY CHARACTER INCLUDING, WITHOUT LIMITATION,
 * DAMAGES FOR LOSS OF GOODWILL, WORK STOPPAGE, COMPUTER FAILURE OR
 * MALFUNCTION, OR ANY AND ALL OTHER COMMERCIAL DAMAGES OR LOSSES, EVEN IF
 * SUCH PARTY HAD BEEN INFORMED, OR OUGHT TO HAVE KNOWN, OF THE POSSIBILITY
 * OF SUCH DAMAGES.  THE JASPER SOFTWARE AND UNDERLYING TECHNOLOGY ARE NOT
 * FAULT-TOLERANT AND ARE NOT DESIGNED, MANUFACTURED OR INTENDED FOR USE OR
 * RESALE AS ON-LINE CONTROL EQUIPMENT IN HAZARDOUS ENVIRONMENTS REQUIRING
 * FAIL-SAFE PERFORMANCE, SUCH AS IN THE OPERATION OF NUCLEAR FACILITIES,
 * AIRCRAFT NAVIGATION OR COMMUNICATION SYSTEMS, AIR TRAFFIC CONTROL, DIRECT
 * LIFE SUPPORT MACHINES, OR WEAPONS SYSTEMS, IN WHICH THE FAILURE OF THE
 * JASPER SOFTWARE OR UNDERLYING TECHNOLOGY OR PRODUCT COULD LEAD DIRECTLY
 * TO DEATH, PERSONAL INJURY, OR SEVERE PHYSICAL OR ENVIRONMENTAL DAMAGE
 * ("HIGH RISK ACTIVITIES").  LICENSOR SPECIFICALLY DISCLAIMS ANY EXPRESS
 * OR IMPLIED WARRANTY OF FITNESS FOR HIGH RISK ACTIVITIES.  USER WILL NOT
 * KNOWINGLY USE, DISTRIBUTE OR RESELL THE JASPER SOFTWARE OR UNDERLYING
 * TECHNOLOGY OR PRODUCTS FOR HIGH RISK ACTIVITIES AND WILL ENSURE THAT ITS
 * CUSTOMERS AND END-USERS OF ITS PRODUCTS ARE PROVIDED WITH A COPY OF THE
 * NOTICE SPECIFIED IN THIS SECTION.
 *
 * __END_OF_JASPER_LICENSE__
 */

/*
 * $Id$
 */

/******************************************************************************\
* Includes.
\******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "jasper/jas_types.h"
#include "jasper/jas_math.h"

#include "jpc_bs.h"
#include "jpc_dec.h"
#include "jpc_cs.h"
#include "jpc_mqcod.h"
#include "jpc_t1cod.h"
#include "jpc_tsfb.h"

double jpc_pow2i(int n);

/******************************************************************************\
* Global data.
\******************************************************************************/

int jpc_zcctxnolut[4 * 256];
int jpc_spblut[256];
int jpc_scctxnolut[256];
int jpc_magctxnolut[4096];

jpc_fix_t jpc_signmsedec[1 << JPC_NMSEDEC_BITS];
jpc_fix_t jpc_refnmsedec[1 << JPC_NMSEDEC_BITS];
jpc_fix_t jpc_signmsedec0[1 << JPC_NMSEDEC_BITS];
jpc_fix_t jpc_refnmsedec0[1 << JPC_NMSEDEC_BITS];

jpc_mqctx_t jpc_mqctxs[JPC_NUMCTXS];

/******************************************************************************\
* Code.
\******************************************************************************/

int JPC_PASSTYPE(int passno)
{
        int passtype;
        switch (passno % 3) {
        case 0:
                passtype = JPC_CLNPASS;
                break;
        case 1:
                passtype = JPC_SIGPASS;
                break;
        case 2:
                passtype = JPC_REFPASS;
                break;
        default:
                passtype = -1;
                assert(0);
                break;
        }
        return passtype;
}

int JPC_NOMINALGAIN(int qmfbid, int numlvls, int lvlno, int orient)
{
if (qmfbid == JPC_COX_INS) {
        return 0;
}
        assert(qmfbid == JPC_COX_RFT);
        if (lvlno == 0) {
                assert(orient == JPC_TSFB_LL);
                return 0;
        } else {
                switch (orient) {
                case JPC_TSFB_LH:
                case JPC_TSFB_HL:
                        return 1;
                        break;
                case JPC_TSFB_HH:
                        return 2;
                        break;
                }
        }
        abort();
}

/******************************************************************************\
* Coding pass related functions.
\******************************************************************************/

int JPC_SEGTYPE(int passno, int firstpassno, int bypass)
{
        int passtype;
        if (bypass) {
                passtype = JPC_PASSTYPE(passno);
                if (passtype == JPC_CLNPASS) {
                        return JPC_SEG_MQ;
                }
                return ((passno < firstpassno + 10) ? JPC_SEG_MQ : JPC_SEG_RAW);
        } else {
                return JPC_SEG_MQ;
        }
}

int JPC_SEGPASSCNT(int passno, int firstpassno, int numpasses, int bypass, int termall)
{
        int ret;
        int passtype;

        if (termall) {
                ret = 1;
        } else if (bypass) {
                if (passno < firstpassno + 10) {
                        ret = 10 - (passno - firstpassno);
                } else {
                        passtype = JPC_PASSTYPE(passno);
                        switch (passtype) {
                        case JPC_SIGPASS:
                                ret = 2;
                                break;
                        case JPC_REFPASS:
                                ret = 1;
                                break;
                        case JPC_CLNPASS:
                                ret = 1;
                                break;
                        default:
                                ret = -1;
                                assert(0);
                                break;
                        }
                }
        } else {
                ret = JPC_PREC * 3 - 2;
        }
        ret = JAS_MIN(ret, numpasses - passno);
        return ret;
}

int JPC_ISTERMINATED(int passno, int firstpassno, int numpasses, int termall,
  int lazy)
{
        int ret;
        int n;
        if (passno - firstpassno == numpasses - 1) {
                ret = 1;
        } else {
                n = JPC_SEGPASSCNT(passno, firstpassno, numpasses, lazy, termall);
                ret = (n <= 1) ? 1 : 0;
        }

        return ret;
}

/******************************************************************************\
* Lookup table code.
\******************************************************************************/

static void jpc_initmqctxs(void)
{
        jpc_initctxs(jpc_mqctxs);
}

void jpc_initluts()
{
        int i;
        int orient;
        int refine;
        float u;
        float v;
        float t;

/* XXX - hack */
jpc_initmqctxs();

        for (orient = 0; orient < 4; ++orient) {
                for (i = 0; i < 256; ++i) {
                        jpc_zcctxnolut[(orient << 8) | i] = jpc_getzcctxno(i, orient);
                }
        }

        for (i = 0; i < 256; ++i) {
                jpc_spblut[i] = jpc_getspb(i << 4);
        }

        for (i = 0; i < 256; ++i) {
                jpc_scctxnolut[i] = jpc_getscctxno(i << 4);
        }

        for (refine = 0; refine < 2; ++refine) {
                for (i = 0; i < 2048; ++i) {
                        jpc_magctxnolut[(refine << 11) + i] = jpc_getmagctxno((refine ? JPC_REFINE : 0) | i);
                }
        }

        for (i = 0; i < (1 << JPC_NMSEDEC_BITS); ++i) {
                t = i * jpc_pow2i(-JPC_NMSEDEC_FRACBITS);
                u = t;
                v = t - 1.5;
                jpc_signmsedec[i] = jpc_dbltofix(floor((u * u - v * v) * jpc_pow2i(JPC_NMSEDEC_FRACBITS) + 0.5) / jpc_pow2i(JPC_NMSEDEC_FRACBITS));
/* XXX - this calc is not correct */
                jpc_signmsedec0[i] = jpc_dbltofix(floor((u * u) * jpc_pow2i(JPC_NMSEDEC_FRACBITS) + 0.5) / jpc_pow2i(JPC_NMSEDEC_FRACBITS));
                u = t - 1.0;
                if (i & (1 << (JPC_NMSEDEC_BITS - 1))) {
                        v = t - 1.5;
                } else {
                        v = t - 0.5;
                }
                jpc_refnmsedec[i] = jpc_dbltofix(floor((u * u - v * v) * jpc_pow2i(JPC_NMSEDEC_FRACBITS) + 0.5) / jpc_pow2i(JPC_NMSEDEC_FRACBITS));
/* XXX - this calc is not correct */
                jpc_refnmsedec0[i] = jpc_dbltofix(floor((u * u) * jpc_pow2i(JPC_NMSEDEC_FRACBITS) + 0.5) / jpc_pow2i(JPC_NMSEDEC_FRACBITS));
        }
}

jpc_fix_t jpc_getsignmsedec_func(jpc_fix_t x, int bitpos)
{
        jpc_fix_t y;
        assert(!(x & (~JAS_ONES(bitpos + 1))));
        y = jpc_getsignmsedec_macro(x, bitpos);
        return y;
}

int jpc_getzcctxno(int f, int orient)
{
        int h;
        int v;
        int d;
        int n;
        int t;
        int hv;

        /* Avoid compiler warning. */
        n = 0;

        h = ((f & JPC_WSIG) != 0) + ((f & JPC_ESIG) != 0);
        v = ((f & JPC_NSIG) != 0) + ((f & JPC_SSIG) != 0);
        d = ((f & JPC_NWSIG) != 0) + ((f & JPC_NESIG) != 0) + ((f & JPC_SESIG) != 0) + ((f & JPC_SWSIG) != 0);
        switch (orient) {
        case JPC_TSFB_HL:
                t = h;
                h = v;
                v = t;
        case JPC_TSFB_LL:
        case JPC_TSFB_LH:
                if (!h) {
                        if (!v) {
                                if (!d) {
                                        n = 0;
                                } else if (d == 1) {
                                        n = 1;
                                } else {
                                        n = 2;
                                }
                        } else if (v == 1) {
                                n = 3;
                        } else {
                                n = 4;
                        }
                } else if (h == 1) {
                        if (!v) {
                                if (!d) {
                                        n = 5;
                                } else {
                                        n = 6;
                                }
                        } else {
                                n = 7;
                        }
                } else {
                        n = 8;
                }
                break;
        case JPC_TSFB_HH:
                hv = h + v;
                if (!d) {
                        if (!hv) {
                                n = 0;
                        } else if (hv == 1) {
                                n = 1;
                        } else {
                                n = 2;
                        }
                } else if (d == 1) {
                        if (!hv) {
                                n = 3;
                        } else if (hv == 1) {
                                n = 4;
                        } else {
                                n = 5;
                        }
                } else if (d == 2) {
                        if (!hv) {
                                n = 6;
                        } else {
                                n = 7;
                        }
                } else {
                        n = 8;
                }
                break;
        }
        assert(n < JPC_NUMZCCTXS);
        return JPC_ZCCTXNO + n;
}

int jpc_getspb(int f)
{
        int hc;
        int vc;
        int n;

        hc = JAS_MIN(((f & (JPC_ESIG | JPC_ESGN)) == JPC_ESIG) + ((f & (JPC_WSIG | JPC_WSGN)) == JPC_WSIG), 1) -
          JAS_MIN(((f & (JPC_ESIG | JPC_ESGN)) == (JPC_ESIG | JPC_ESGN)) + ((f & (JPC_WSIG | JPC_WSGN)) == (JPC_WSIG | JPC_WSGN)), 1);
        vc = JAS_MIN(((f & (JPC_NSIG | JPC_NSGN)) == JPC_NSIG) + ((f & (JPC_SSIG | JPC_SSGN)) == JPC_SSIG), 1) -
          JAS_MIN(((f & (JPC_NSIG | JPC_NSGN)) == (JPC_NSIG | JPC_NSGN)) + ((f & (JPC_SSIG | JPC_SSGN)) == (JPC_SSIG | JPC_SSGN)), 1);
        if (!hc && !vc) {
                n = 0;
        } else {
                n = (!(hc > 0 || (!hc && vc > 0)));
        }
        return n;
}

int jpc_getscctxno(int f)
{
        int hc;
        int vc;
        int n;

        /* Avoid compiler warning. */
        n = 0;

        hc = JAS_MIN(((f & (JPC_ESIG | JPC_ESGN)) == JPC_ESIG) + ((f & (JPC_WSIG | JPC_WSGN)) == JPC_WSIG),
          1) - JAS_MIN(((f & (JPC_ESIG | JPC_ESGN)) == (JPC_ESIG | JPC_ESGN)) +
          ((f & (JPC_WSIG | JPC_WSGN)) == (JPC_WSIG | JPC_WSGN)), 1);
        vc = JAS_MIN(((f & (JPC_NSIG | JPC_NSGN)) == JPC_NSIG) + ((f & (JPC_SSIG | JPC_SSGN)) == JPC_SSIG),
          1) - JAS_MIN(((f & (JPC_NSIG | JPC_NSGN)) == (JPC_NSIG | JPC_NSGN)) +
          ((f & (JPC_SSIG | JPC_SSGN)) == (JPC_SSIG | JPC_SSGN)), 1);
        assert(hc >= -1 && hc <= 1 && vc >= -1 && vc <= 1);
        if (hc < 0) {
                hc = -hc;
                vc = -vc;
        }
        if (!hc) {
                if (vc == -1) {
                        n = 1;
                } else if (!vc) {
                        n = 0;
                } else {
                        n = 1;
                }
        } else if (hc == 1) {
                if (vc == -1) {
                        n = 2;
                } else if (!vc) {
                        n = 3;
                } else {
                        n = 4;
                }
        }
        assert(n < JPC_NUMSCCTXS);
        return JPC_SCCTXNO + n;
}

int jpc_getmagctxno(int f)
{
        int n;

        if (!(f & JPC_REFINE)) {
                n = (f & (JPC_OTHSIGMSK)) ? 1 : 0;
        } else {
                n = 2;
        }

        assert(n < JPC_NUMMAGCTXS);
        return JPC_MAGCTXNO + n;
}

void jpc_initctxs(jpc_mqctx_t *ctxs)
{
        jpc_mqctx_t *ctx;
        int i;

        ctx = ctxs;
        for (i = 0; i < JPC_NUMCTXS; ++i) {
                ctx->mps = 0;
                switch (i) {
                case JPC_UCTXNO:
                        ctx->ind = 46;
                        break;
                case JPC_ZCCTXNO:
                        ctx->ind = 4;
                        break;
                case JPC_AGGCTXNO:
                        ctx->ind = 3;
                        break;
                default:
                        ctx->ind = 0;
                        break;
                }
                ++ctx;
        }
}

/* Calculate the real quantity exp2(n), where x is an integer. */
double jpc_pow2i(int n)
{
        double x;
        double a;

        x = 1.0;
        if (n < 0) {
                a = 0.5;
                n = -n;
        } else {
                a = 2.0;
        }
        while (--n >= 0) {
                x *= a;
        }
        return x;
}
