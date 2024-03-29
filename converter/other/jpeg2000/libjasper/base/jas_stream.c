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

#define _DEFAULT_SOURCE /* New name for SVID & BSD source defines */
#define _SVID_SOURCE
    /* Make sure P_tmpdir is defined in GNU libc 2.0.7 (_XOPEN_SOURCE 500
       does it in other libc's).  pm_config.h defines TMPDIR as P_tmpdir
       in some environments.
    */
#define _XOPEN_SOURCE 500    /* Make sure P_tmpdir is defined */

#include "pm_config.h"
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif
#if HAVE_IO_H
#include <io.h>
#endif

#include "netpbm/pm.h"

#include "jasper/jas_types.h"
#include "jasper/jas_stream.h"
#include "jasper/jas_malloc.h"
#include "jasper/jas_math.h"

/****************************************************************************\
* Local function prototypes.
\****************************************************************************/

static int jas_strtoopenmode(const char *s);
static void jas_stream_destroy(jas_stream_t *stream);
static jas_stream_t *jas_stream_create(void);
static void jas_stream_initbuf(jas_stream_t *stream, int bufmode, char *buf,
  int bufsize);

static int mem_read(jas_stream_obj_t *obj, char *buf, int cnt);
static int mem_write(jas_stream_obj_t *obj, char *buf, int cnt);
static long mem_seek(jas_stream_obj_t *obj, long offset, int origin);
static int mem_close(jas_stream_obj_t *obj);

static int sfile_read(jas_stream_obj_t *obj, char *buf, int cnt);
static int sfile_write(jas_stream_obj_t *obj, char *buf, int cnt);
static long sfile_seek(jas_stream_obj_t *obj, long offset, int origin);
static int sfile_close(jas_stream_obj_t *obj);

static int file_read(jas_stream_obj_t *obj, char *buf, int cnt);
static int file_write(jas_stream_obj_t *obj, char *buf, int cnt);
static long file_seek(jas_stream_obj_t *obj, long offset, int origin);
static int file_close(jas_stream_obj_t *obj);

/******************************************************************************\
* Local data.
\******************************************************************************/

static jas_stream_ops_t jas_stream_fileops = {
        file_read,
        file_write,
        file_seek,
        file_close
};

static jas_stream_ops_t jas_stream_sfileops = {
        sfile_read,
        sfile_write,
        sfile_seek,
        sfile_close
};

static jas_stream_ops_t jas_stream_memops = {
        mem_read,
        mem_write,
        mem_seek,
        mem_close
};

/******************************************************************************\
* Code for opening and closing streams.
\******************************************************************************/

static jas_stream_t *jas_stream_create()
{
        jas_stream_t *stream;

        if (!(stream = jas_malloc(sizeof(jas_stream_t)))) {
                return 0;
        }
        stream->openmode_ = 0;
        stream->bufmode_ = 0;
        stream->flags_ = 0;
        stream->bufbase_ = 0;
        stream->bufstart_ = 0;
        stream->bufsize_ = 0;
        stream->ptr_ = 0;
        stream->cnt_ = 0;
        stream->ops_ = 0;
        stream->obj_ = 0;
        stream->rwcnt_ = 0;
        stream->rwlimit_ = -1;

        return stream;
}

jas_stream_t *jas_stream_memopen(char *buf, int bufsize)
{
        jas_stream_t *stream;
        jas_stream_memobj_t *obj;

        if (!(stream = jas_stream_create())) {
                return 0;
        }

        /* A stream associated with a memory buffer is always opened
        for both reading and writing in binary mode. */
        stream->openmode_ = JAS_STREAM_READ | JAS_STREAM_WRITE | JAS_STREAM_BINARY;

        /* Since the stream data is already resident in memory, buffering
        is not necessary. */
        /* But... It still may be faster to use buffering anyways. */
        jas_stream_initbuf(stream, JAS_STREAM_FULLBUF, 0, 0);

        /* Select the operations for a memory stream. */
        stream->ops_ = &jas_stream_memops;

        /* Allocate memory for the underlying memory stream object. */
        if (!(obj = jas_malloc(sizeof(jas_stream_memobj_t)))) {
                jas_stream_destroy(stream);
                return 0;
        }
        stream->obj_ = (void *) obj;

        /* Initialize a few important members of the memory stream object. */
        obj->myalloc_ = 0;
        obj->buf_ = 0;

        /* If the buffer size specified is nonpositive, then the buffer
        is allocated internally and automatically grown as needed. */
        if (bufsize <= 0) {
                obj->bufsize_ = 1024;
                obj->growable_ = 1;
        } else {
                obj->bufsize_ = bufsize;
                obj->growable_ = 0;
        }
        if (buf) {
                obj->buf_ = (unsigned char *) buf;
        } else {
                obj->buf_ = jas_malloc(obj->bufsize_ * sizeof(char));
                obj->myalloc_ = 1;
        }
        if (!obj->buf_) {
                jas_stream_close(stream);
                return 0;
        }

        if (bufsize > 0 && buf) {
                /* If a buffer was supplied by the caller and its length is positive,
                  make the associated buffer data appear in the stream initially. */
                obj->len_ = bufsize;
        } else {
                /* The stream is initially empty. */
                obj->len_ = 0;
        }
        obj->pos_ = 0;

        return stream;
}

jas_stream_t *jas_stream_fopen(const char *filename, const char *mode)
{
        jas_stream_t *stream;
        int *obj;
        int openflags;

        /* Allocate a stream object. */
        if (!(stream = jas_stream_create())) {
                return 0;
        }

        /* Parse the mode string. */
        stream->openmode_ = jas_strtoopenmode(mode);

        /* Determine the correct flags to use for opening the file. */
        if ((stream->openmode_ & JAS_STREAM_READ) &&
          (stream->openmode_ & JAS_STREAM_WRITE)) {
                openflags = O_RDWR;
        } else if (stream->openmode_ & JAS_STREAM_READ) {
                openflags = O_RDONLY;
        } else if (stream->openmode_ & JAS_STREAM_WRITE) {
                openflags = O_WRONLY;
        } else {
                openflags = 0;
        }
        if (stream->openmode_ & JAS_STREAM_APPEND) {
                openflags |= O_APPEND;
        }
        if (stream->openmode_ & JAS_STREAM_BINARY) {
                openflags |= O_BINARY;
        }
        if (stream->openmode_ & JAS_STREAM_CREATE) {
                openflags |= O_CREAT | O_TRUNC;
        }

        /* Allocate space for the underlying file stream object. */
        if (!(obj = jas_malloc(sizeof(int)))) {
                jas_stream_destroy(stream);
                return 0;
        }
        stream->obj_ = (void *) obj;

        /* Select the operations for a file stream object. */
        stream->ops_ = &jas_stream_fileops;

        /* Open the underlying file. */
        if ((*obj = open(filename, openflags, JAS_STREAM_PERMS)) < 0) {
                jas_stream_destroy(stream);
                return 0;
        }

        /* By default, use full buffering for this type of stream. */
        jas_stream_initbuf(stream, JAS_STREAM_FULLBUF, 0, 0);

        return stream;
}

jas_stream_t *jas_stream_freopen(const char *path, const char *mode, FILE *fp)
{
        jas_stream_t *stream;
        int openflags;

        /* Allocate a stream object. */
        if (!(stream = jas_stream_create())) {
                return 0;
        }

        /* Parse the mode string. */
        stream->openmode_ = jas_strtoopenmode(mode);

        /* Determine the correct flags to use for opening the file. */
        if ((stream->openmode_ & JAS_STREAM_READ) &&
          (stream->openmode_ & JAS_STREAM_WRITE)) {
                openflags = O_RDWR;
        } else if (stream->openmode_ & JAS_STREAM_READ) {
                openflags = O_RDONLY;
        } else if (stream->openmode_ & JAS_STREAM_WRITE) {
                openflags = O_WRONLY;
        } else {
                openflags = 0;
        }
        if (stream->openmode_ & JAS_STREAM_APPEND) {
                openflags |= O_APPEND;
        }
        if (stream->openmode_ & JAS_STREAM_BINARY) {
                openflags |= O_BINARY;
        }
        if (stream->openmode_ & JAS_STREAM_CREATE) {
                openflags |= O_CREAT | O_TRUNC;
        }

        stream->obj_ = JAS_CAST(void *, fp);

        /* Select the operations for a file stream object. */
        stream->ops_ = &jas_stream_sfileops;

        /* By default, use full buffering for this type of stream. */
        jas_stream_initbuf(stream, JAS_STREAM_FULLBUF, 0, 0);

        return stream;
}


jas_stream_t *jas_stream_tmpfile()
{
        jas_stream_t *stream;
        int *obj;

        if (!(stream = jas_stream_create())) {
                return 0;
        }

        /* A temporary file stream is always opened for both reading and
        writing in binary mode. */
        stream->openmode_ = JAS_STREAM_READ | JAS_STREAM_WRITE | JAS_STREAM_BINARY;

        /* Allocate memory for the underlying temporary file object. */
        if (!(obj = jas_malloc(sizeof(int)))) {
                jas_stream_destroy(stream);
                return 0;
        }
        stream->obj_ = obj;

    {
        /* This is a Netpbm enhancement.  Original Jasper library uses
           tmpnam(), which is unsafe.
        */
        jmp_buf jmpbuf;
        int rc;

        rc = setjmp(jmpbuf);
        if (rc == 0) {
            pm_setjmpbuf(&jmpbuf);
            *obj = pm_tmpfile_fd();
            pm_setjmpbuf(NULL);
        } else {
            /* pm_tmpfile_fd() threw an error */
            jas_stream_destroy(stream);
            pm_setjmpbuf(NULL);
            return 0;
        }
    }
        /* Use full buffering. */
        jas_stream_initbuf(stream, JAS_STREAM_FULLBUF, 0, 0);

        stream->ops_ = &jas_stream_fileops;

        return stream;
}

jas_stream_t *jas_stream_fdopen(int fd, const char *mode)
{
        jas_stream_t *stream;
        int *obj;

        /* Allocate a stream object. */
        if (!(stream = jas_stream_create())) {
                return 0;
        }

        /* Parse the mode string. */
        stream->openmode_ = jas_strtoopenmode(mode);

#if defined(HAVE_SETMODE) && defined(O_BINARY)
        /* Argh!!!  Someone ought to banish text mode (i.e., O_TEXT) to the
          greatest depths of purgatory! */
        /* Ensure that the file descriptor is in binary mode, if the caller
          has specified the binary mode flag.  Arguably, the caller ought to
          take care of this, but text mode is a ugly wart anyways, so we save
          the caller some grief by handling this within the stream library. */
        /* This ugliness is mainly for the benefit of those who run the
          JasPer software under Windows from shells that insist on opening
          files in text mode.  For example, in the Cygwin environment,
          shells often open files in text mode when I/O redirection is
          used.  Grr... */
        if (stream->openmode_ & JAS_STREAM_BINARY) {
                setmode(fd, O_BINARY);
        }
#endif

        /* Allocate space for the underlying file stream object. */
        if (!(obj = jas_malloc(sizeof(int)))) {
                jas_stream_destroy(stream);
                return 0;
        }
        stream->obj_ = (void *) obj;
        *obj = fd;

        /* By default, use full buffering for this type of stream. */
        jas_stream_initbuf(stream, JAS_STREAM_FULLBUF, 0, 0);

        /* Select the operations for a file stream object. */
        stream->ops_ = &jas_stream_fileops;

/* Do not close the underlying file descriptor when the stream is closed. */
        stream->openmode_ |= JAS_STREAM_NOCLOSE;

        return stream;
}

static void jas_stream_destroy(jas_stream_t *stream)
{
        /* If the memory for the buffer was allocated with malloc, free
        this memory. */
        if ((stream->bufmode_ & JAS_STREAM_FREEBUF) && stream->bufbase_) {
                jas_free(stream->bufbase_);
                stream->bufbase_ = 0;
        }
        jas_free(stream);
}

int jas_stream_close(jas_stream_t *stream)
{
        /* Flush buffer if necessary. */
        jas_stream_flush(stream);

        /* Close the underlying stream object. */
        if (!(stream->openmode_ & JAS_STREAM_NOCLOSE)) {
                (*stream->ops_->close_)(stream->obj_);
        }

        jas_stream_destroy(stream);

        return 0;
}

/******************************************************************************\
* Code for reading and writing streams.
\******************************************************************************/

int jas_stream_getc_func(jas_stream_t *stream)
{
        assert(stream->ptr_ - stream->bufbase_ <= stream->bufsize_ +
          JAS_STREAM_MAXPUTBACK);
        return jas_stream_getc_macro(stream);
}

int jas_stream_putc_func(jas_stream_t *stream, int c)
{
        assert(stream->ptr_ - stream->bufstart_ <= stream->bufsize_);
        return jas_stream_putc_macro(stream, c);
}

int jas_stream_ungetc(jas_stream_t *stream, int c)
{
        if (!stream->ptr_ || stream->ptr_ == stream->bufbase_) {
                return -1;
        }

        /* Reset the EOF indicator (since we now have at least one character
          to read). */
        stream->flags_ &= ~JAS_STREAM_EOF;

        --stream->rwcnt_;
        --stream->ptr_;
        ++stream->cnt_;
        *stream->ptr_ = c;
        return 0;
}

int jas_stream_read(jas_stream_t *stream, void *buf, int cnt)
{
        int n;
        int c;
        char *bufptr;

        bufptr = buf;

        n = 0;
        while (n < cnt) {
                if ((c = jas_stream_getc(stream)) == EOF) {
                        return n;
                }
                *bufptr++ = c;
                ++n;
        }

        return n;
}

int jas_stream_write(jas_stream_t *stream, const void *buf, int cnt)
{
        int n;
        const char *bufptr;

        bufptr = buf;

        n = 0;
        while (n < cnt) {
                if (jas_stream_putc(stream, *bufptr) == EOF) {
                        return n;
                }
                ++bufptr;
                ++n;
        }

        return n;
}

/* Note: This function uses a fixed size buffer.  Therefore, it cannot
  handle invocations that will produce more output than can be held
  by the buffer. */
int jas_stream_printf(jas_stream_t *stream, const char *fmt, ...)
{
        va_list ap;
        char buf[4096];
        int ret;

        va_start(ap, fmt);
        ret = vsprintf(buf, fmt, ap);
        jas_stream_puts(stream, buf);
        va_end(ap);
        return ret;
}

int jas_stream_puts(jas_stream_t *stream, const char *s)
{
        while (*s != '\0') {
                if (jas_stream_putc_macro(stream, *s) == EOF) {
                        return -1;
                }
                ++s;
        }
        return 0;
}

char *jas_stream_gets(jas_stream_t *stream, char *buf, int bufsize)
{
        int c;
        char *bufptr;
        assert(bufsize > 0);

        bufptr = buf;
        while (bufsize > 1) {
                if ((c = jas_stream_getc(stream)) == EOF) {
                        break;
                }
                *bufptr++ = c;
                --bufsize;
                if (c == '\n') {
                        break;
                }
        }
        *bufptr = '\0';
        return buf;
}

int jas_stream_gobble(jas_stream_t *stream, int n)
{
        int m;
        m = n;
        for (m = n; m > 0; --m) {
                if (jas_stream_getc(stream) == EOF) {
                        return n - m;
                }
        }
        return n;
}

/******************************************************************************\
* Code for getting and setting the stream position.
\******************************************************************************/

int jas_stream_isseekable(jas_stream_t *stream)
{
        if (stream->ops_ == &jas_stream_memops) {
                return 1;
        } else if (stream->ops_ == &jas_stream_fileops) {
                if ((*stream->ops_->seek_)(stream->obj_, 0, SEEK_CUR) < 0) {
                        return 0;
                }
                return 1;
        } else {
                return 0;
        }
}

int jas_stream_rewind(jas_stream_t *stream)
{
        return jas_stream_seek(stream, 0, SEEK_SET);
}

long jas_stream_seek(jas_stream_t *stream, long offset, int origin)
{
        long newpos;

        /* The buffer cannot be in use for both reading and writing. */
        assert(!((stream->bufmode_ & JAS_STREAM_RDBUF) && (stream->bufmode_ &
          JAS_STREAM_WRBUF)));

        /* Reset the EOF indicator (since we may not be at the EOF anymore). */
        stream->flags_ &= ~JAS_STREAM_EOF;

        if (stream->bufmode_ & JAS_STREAM_RDBUF) {
                if (origin == SEEK_CUR) {
                        offset -= stream->cnt_;
                }
        } else if (stream->bufmode_ & JAS_STREAM_WRBUF) {
                if (jas_stream_flush(stream)) {
                        return -1;
                }
        }
        stream->cnt_ = 0;
        stream->ptr_ = stream->bufstart_;
        stream->bufmode_ &= ~(JAS_STREAM_RDBUF | JAS_STREAM_WRBUF);

        if ((newpos = (*stream->ops_->seek_)(stream->obj_, offset, origin))
          < 0) {
                return -1;
        }

        return newpos;
}

long jas_stream_tell(jas_stream_t *stream)
{
        int adjust;
        int offset;

        if (stream->bufmode_ & JAS_STREAM_RDBUF) {
                adjust = -stream->cnt_;
        } else if (stream->bufmode_ & JAS_STREAM_WRBUF) {
                adjust = stream->ptr_ - stream->bufstart_;
        } else {
                adjust = 0;
        }

        if ((offset = (*stream->ops_->seek_)(stream->obj_, 0, SEEK_CUR)) < 0) {
                return -1;
        }

        return offset + adjust;
}

/******************************************************************************\
* Buffer initialization code.
\******************************************************************************/

static void jas_stream_initbuf(jas_stream_t *stream, int bufmode, char *buf,
  int bufsize)
{
        /* If this function is being called, the buffer should not have been
          initialized yet. */
        assert(!stream->bufbase_);

        if (bufmode != JAS_STREAM_UNBUF) {
                /* The full- or line-buffered mode is being employed. */
                if (!buf) {
                        /* The caller has not specified a buffer to employ, so allocate
                          one. */
                        if ((stream->bufbase_ = jas_malloc(JAS_STREAM_BUFSIZE +
                          JAS_STREAM_MAXPUTBACK))) {
                                stream->bufmode_ |= JAS_STREAM_FREEBUF;
                                stream->bufsize_ = JAS_STREAM_BUFSIZE;
                        } else {
                                /* The buffer allocation has failed.  Resort to unbuffered
                                  operation. */
                                stream->bufbase_ = stream->tinybuf_;
                                stream->bufsize_ = 1;
                        }
                } else {
                        /* The caller has specified a buffer to employ. */
                        /* The buffer must be large enough to accommodate maximum
                          putback. */
                        assert(bufsize > JAS_STREAM_MAXPUTBACK);
                        stream->bufbase_ = JAS_CAST(unsigned char *, buf);
                        stream->bufsize_ = bufsize - JAS_STREAM_MAXPUTBACK;
                }
        } else {
                /* The unbuffered mode is being employed. */
                /* A buffer should not have been supplied by the caller. */
                assert(!buf);
                /* Use a trivial one-character buffer. */
                stream->bufbase_ = stream->tinybuf_;
                stream->bufsize_ = 1;
        }
        stream->bufstart_ = &stream->bufbase_[JAS_STREAM_MAXPUTBACK];
        stream->ptr_ = stream->bufstart_;
        stream->cnt_ = 0;
        stream->bufmode_ |= bufmode & JAS_STREAM_BUFMODEMASK;
}

/******************************************************************************\
* Buffer filling and flushing code.
\******************************************************************************/

int jas_stream_flush(jas_stream_t *stream)
{
        if (stream->bufmode_ & JAS_STREAM_RDBUF) {
                return 0;
        }
        return jas_stream_flushbuf(stream, EOF);
}

int jas_stream_fillbuf(jas_stream_t *stream, int getflag)
{
        int c;

        /* The stream must not be in an error or EOF state. */
        if ((stream->flags_ & (JAS_STREAM_ERRMASK)) != 0) {
                return EOF;
        }

        /* The stream must be open for reading. */
        if ((stream->openmode_ & JAS_STREAM_READ) == 0) {
                return EOF;
        }

        /* Make a half-hearted attempt to confirm that the buffer is not
        currently being used for writing.  This check is not intended
        to be foolproof! */
        assert((stream->bufmode_ & JAS_STREAM_WRBUF) == 0);

        assert(stream->ptr_ - stream->bufstart_ <= stream->bufsize_);

        /* Mark the buffer as being used for reading. */
        stream->bufmode_ |= JAS_STREAM_RDBUF;

        /* Read new data into the buffer. */
        stream->ptr_ = stream->bufstart_;
        if ((stream->cnt_ = (*stream->ops_->read_)(stream->obj_,
          (char *) stream->bufstart_, stream->bufsize_)) <= 0) {
                if (stream->cnt_ < 0) {
                        stream->flags_ |= JAS_STREAM_ERR;
                } else {
                        stream->flags_ |= JAS_STREAM_EOF;
                }
                stream->cnt_ = 0;
                return EOF;
        }

        assert(stream->cnt_ > 0);
        /* Get or peek at the first character in the buffer. */
        c = (getflag) ? jas_stream_getc2(stream) : (*stream->ptr_);

        return c;
}

int jas_stream_flushbuf(jas_stream_t *stream, int c)
{
        int len;
        int n;

        /* The stream should not be in an error or EOF state. */
        if ((stream->flags_ & (JAS_STREAM_ERRMASK)) != 0) {
                return EOF;
        }

        /* The stream must be open for writing. */
        if ((stream->openmode_ & (JAS_STREAM_WRITE | JAS_STREAM_APPEND)) == 0) {
                return EOF;
        }

        /* The buffer should not currently be in use for reading. */
        assert(!(stream->bufmode_ & JAS_STREAM_RDBUF));

        /* Note: Do not use the quantity stream->cnt to determine the number
        of characters in the buffer!  Depending on how this function was
        called, the stream->cnt value may be "off-by-one". */
        len = stream->ptr_ - stream->bufstart_;
        if (len > 0) {
                n = (*stream->ops_->write_)(stream->obj_, (char *)
                  stream->bufstart_, len);
                if (n != len) {
                        stream->flags_ |= JAS_STREAM_ERR;
                        return EOF;
                }
        }
        stream->cnt_ = stream->bufsize_;
        stream->ptr_ = stream->bufstart_;

        stream->bufmode_ |= JAS_STREAM_WRBUF;

        if (c != EOF) {
                assert(stream->cnt_ > 0);
                jas_stream_putc2(stream, c);
        }

        return 0;
}

/******************************************************************************\
* Miscellaneous code.
\******************************************************************************/

static int jas_strtoopenmode(const char *s)
{
        int openmode = 0;
        while (*s != '\0') {
                switch (*s) {
                case 'r':
                        openmode |= JAS_STREAM_READ;
                        break;
                case 'w':
                        openmode |= JAS_STREAM_WRITE | JAS_STREAM_CREATE;
                        break;
                case 'b':
                        openmode |= JAS_STREAM_BINARY;
                        break;
                case 'a':
                        openmode |= JAS_STREAM_APPEND;
                        break;
                case '+':
                        openmode |= JAS_STREAM_READ | JAS_STREAM_WRITE;
                        break;
                default:
                        break;
                }
                ++s;
        }
        return openmode;
}

int jas_stream_copy(jas_stream_t *out, jas_stream_t *in, int n)
{
        int all;
        int c;
        int m;

        all = (n < 0) ? 1 : 0;

        m = n;
        while (all || m > 0) {
                if ((c = jas_stream_getc_macro(in)) == EOF) {
                        /* The next character of input could not be read. */
                        /* Return with an error if an I/O error occurred
                          (not including EOF) or if an explicit copy count
                          was specified. */
                        return (!all || jas_stream_error(in)) ? (-1) : 0;
                }
                if (jas_stream_putc_macro(out, c) == EOF) {
                        return -1;
                }
                --m;
        }
        return 0;
}

long jas_stream_setrwcount(jas_stream_t *stream, long rwcnt)
{
        int old;

        old = stream->rwcnt_;
        stream->rwcnt_ = rwcnt;
        return old;
}

int jas_stream_display(jas_stream_t *stream, FILE *fp, int n)
{
        unsigned char buf[16];
        int i;
        int j;
        int m;
        int c;
        int display;
        int cnt;

        cnt = n - (n % 16);
        display = 1;

        for (i = 0; i < n; i += 16) {
                if (n > 16 && i > 0) {
                        display = (i >= cnt) ? 1 : 0;
                }
                if (display) {
                        fprintf(fp, "%08x:", i);
                }
                m = JAS_MIN(n - i, 16);
                for (j = 0; j < m; ++j) {
                        if ((c = jas_stream_getc(stream)) == EOF) {
                                abort();
                                return -1;
                        }
                        buf[j] = c;
                }
                if (display) {
                        for (j = 0; j < m; ++j) {
                                fprintf(fp, " %02x", buf[j]);
                        }
                        fputc(' ', fp);
                        for (; j < 16; ++j) {
                                fprintf(fp, "   ");
                        }
                        for (j = 0; j < m; ++j) {
                                if (isprint(buf[j])) {
                                        fputc(buf[j], fp);
                                } else {
                                        fputc(' ', fp);
                                }
                        }
                        fprintf(fp, "\n");
                }


        }
        return 0;
}

long jas_stream_length(jas_stream_t *stream)
{
        long oldpos;
        long pos;
        if ((oldpos = jas_stream_tell(stream)) < 0) {
                return -1;
        }
        if (jas_stream_seek(stream, 0, SEEK_END) < 0) {
                return -1;
        }
        if ((pos = jas_stream_tell(stream)) < 0) {
                return -1;
        }
        if (jas_stream_seek(stream, oldpos, SEEK_SET) < 0) {
                return -1;
        }
        return pos;
}

/******************************************************************************\
* Memory stream object.
\******************************************************************************/

static int mem_read(jas_stream_obj_t *obj, char *buf, int cnt)
{
        int n;
        jas_stream_memobj_t *m = (jas_stream_memobj_t *)obj;
        n = m->len_ - m->pos_;
        cnt = JAS_MIN(n, cnt);
        memcpy(buf, &m->buf_[m->pos_], cnt);
        m->pos_ += cnt;
        return cnt;
}

static int mem_resize(jas_stream_memobj_t *m, int bufsize)
{
        unsigned char *buf;

        assert(m->buf_);
        if (!(buf = jas_realloc(m->buf_, bufsize * sizeof(unsigned char)))) {
                return -1;
        }
        m->buf_ = buf;
        m->bufsize_ = bufsize;
        return 0;
}

static int mem_write(jas_stream_obj_t *obj, char *buf, int cnt)
{
        int n;
        int ret;
        jas_stream_memobj_t *m = (jas_stream_memobj_t *)obj;
        long newbufsize;
        long newpos;

        newpos = m->pos_ + cnt;
        if (newpos > m->bufsize_ && m->growable_) {
                newbufsize = m->bufsize_;
                while (newbufsize < newpos) {
                        newbufsize <<= 1;
                        assert(newbufsize >= 0);
                }
                if (mem_resize(m, newbufsize)) {
                        return -1;
                }
        }
        if (m->pos_ > m->len_) {
                /* The current position is beyond the end of the file, so
                  pad the file to the current position with zeros. */
                n = JAS_MIN(m->pos_, m->bufsize_) - m->len_;
                if (n > 0) {
                        memset(&m->buf_[m->len_], 0, n);
                        m->len_ += n;
                }
                if (m->pos_ != m->len_) {
                        /* The buffer is not big enough. */
                        return 0;
                }
        }
        n = m->bufsize_ - m->pos_;
        ret = JAS_MIN(n, cnt);
        if (ret > 0) {
                memcpy(&m->buf_[m->pos_], buf, ret);
                m->pos_ += ret;
        }
        if (m->pos_ > m->len_) {
                m->len_ = m->pos_;
        }
assert(ret == cnt);
        return ret;
}

static long mem_seek(jas_stream_obj_t *obj, long offset, int origin)
{
        jas_stream_memobj_t *m = (jas_stream_memobj_t *)obj;
        long newpos;

        switch (origin) {
        case SEEK_SET:
                newpos = offset;
                break;
        case SEEK_END:
                newpos = m->len_ - offset;
                break;
        case SEEK_CUR:
                newpos = m->pos_ + offset;
                break;
        default:
                abort();
                break;
        }
        if (newpos < 0) {
                return -1;
        }
        m->pos_ = newpos;

        return m->pos_;
}

static int mem_close(jas_stream_obj_t *obj)
{
        jas_stream_memobj_t *m = (jas_stream_memobj_t *)obj;
        if (m->myalloc_ && m->buf_) {
                jas_free(m->buf_);
                m->buf_ = 0;
        }
        jas_free(obj);
        return 0;
}

/******************************************************************************\
* File stream object.
\******************************************************************************/

static int file_read(jas_stream_obj_t *obj, char *buf, int cnt)
{
        int fd;
        fd = *((int *)obj);
        return read(fd, buf, cnt);
}

static int file_write(jas_stream_obj_t *obj, char *buf, int cnt)
{
        int fd;
        fd = *((int *)obj);
        return write(fd, buf, cnt);
}

static long file_seek(jas_stream_obj_t *obj, long offset, int origin)
{
        int fd;
        fd = *((int *)obj);
        return lseek(fd, offset, origin);
}

static int file_close(jas_stream_obj_t *obj)
{
        int fd;
        fd = *((int *)obj);
        jas_free(obj);
        return close(fd);
}

/******************************************************************************\
* Stdio file stream object.
\******************************************************************************/

static int sfile_read(jas_stream_obj_t *obj, char *buf, int cnt)
{
        FILE *fp;
        fp = JAS_CAST(FILE *, obj);
        return fread(buf, 1, cnt, fp);
}

static int sfile_write(jas_stream_obj_t *obj, char *buf, int cnt)
{
        FILE *fp;
        fp = JAS_CAST(FILE *, obj);
        return fwrite(buf, 1, cnt, fp);
}

static long sfile_seek(jas_stream_obj_t *obj, long offset, int origin)
{
        FILE *fp;
        fp = JAS_CAST(FILE *, obj);
        return fseek(fp, offset, origin);
}

static int sfile_close(jas_stream_obj_t *obj)
{
        FILE *fp;
        fp = JAS_CAST(FILE *, obj);
        return fclose(fp);
}
