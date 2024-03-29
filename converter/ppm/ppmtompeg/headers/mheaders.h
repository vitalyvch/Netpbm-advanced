/*===========================================================================*
 * mheaders.h                                                                *
 *                                                                           *
 *      MPEG headers                                                         *
 *                                                                           *
 *===========================================================================*/

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
 *  $Header: /n/picasso/project/mm/mpeg/mpeg_dist/mpeg_encode/headers/RCS/mheaders.h,v 1.4 1995/03/27 19:29:24 smoot Exp $
 *  $Log: mheaders.h,v $
 * Revision 1.4  1995/03/27  19:29:24  smoot
 * changed to remove mb_quant
 *
 * Revision 1.3  1995/01/19  23:54:56  eyhung
 * Changed copyrights
 *
 * Revision 1.2  1994/11/12  02:12:51  keving
 * nothing
 *
 * Revision 1.1  1993/07/22  22:24:23  keving
 * nothing
 *
 *
 */


#ifndef MHEADERS_INCLUDED
#define MHEADERS_INCLUDED


/*==============*
 * HEADER FILES *
 *==============*/

#include "general.h"
#include "bitio.h"


/*===============================*
 * EXTERNAL PROCEDURE prototypes *
 *===============================*/

void    SetGOPStartTime (int index);

void
Mhead_GenSequenceHeader(BitBucket *   const bbPtr,
                        uint32        const hsize,
                        uint32        const vsize,
                        int32         const pratio,
                        int32         const pict_rate,
                        int32         const bit_rate_arg,
                        int32         const buf_size_arg,
                        int32         const c_param_flag_arg,
                        const int32 * const iq_matrix,
                        const int32 * const niq_matrix,
                        uint8 *       const ext_data,
                        int32         const ext_data_size,
                        uint8 *       const user_data,
                        int32         const user_data_size);

void    Mhead_GenSequenceEnder (BitBucket *bbPtr);
void    Mhead_GenGOPHeader (BitBucket *bbPtr,
           int32 drop_frame_flag,
           int32 tc_hrs, int32 tc_min,
           int32 tc_sec, int32 tc_pict,
           int32 closed_gop, int32 broken_link,
           uint8 *ext_data, int32 ext_data_size,
           uint8 *user_data, int32 user_data_size);
void    Mhead_GenPictureHeader (BitBucket *bbPtr, int frameType,
                                            int pictCount, int f_code);
void    Mhead_GenSliceHeader (BitBucket *bbPtr, uint32 slicenum,
                                          uint32 qscale, uint8 *extra_info,
                                          uint32 extra_info_size);
void    Mhead_GenSliceEnder (BitBucket *bbPtr);
void    Mhead_GenMBHeader (BitBucket *bbPtr,
          uint32 pict_code_type, uint32 addr_incr,
          uint32 q_scale,
          uint32 forw_f_code, uint32 back_f_code,
          uint32 horiz_forw_r, uint32 vert_forw_r,
          uint32 horiz_back_r, uint32 vert_back_r,
          int32 motion_forw, int32 m_horiz_forw,
          int32 m_vert_forw, int32 motion_back,
          int32 m_horiz_back, int32 m_vert_back,
          uint32 mb_pattern, uint32 mb_intra);


#endif /* MHEADERS_INCLUDED */





