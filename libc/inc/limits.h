/*
 * Copyright (C) 2009, 2010 Nick Johnson <nickbjohnson4224 at gmail.com>
 * 
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef LIMITS_H
#define LIMITS_H

#define CHAR_BIT	8
#define SCHAR_MIN	0x80
#define SCHAR_MAX	0x7F
#define UCHAR_MAX	0xFF
#define CHAR_MIN	0x80
#define CHAR_MAX	0x7F

#define MB_LEN_MAX	4

#define SHRT_MIN	0x8000
#define SHRT_MAX	0x7FFF
#define USHRT_MAX	0xFFFF

#define INT_MIN		0x80000000
#define INT_MAX		0x7FFFFFFF
#define UINT_MAX	0xFFFFFFFF

#define LONG_MIN	0x80000000
#define LONG_MAX	0x7FFFFFFF
#define ULONG_MAX	0xFFFFFFFF

#define LLONG_MIN	0x8000000000000000LL
#define LLONG_MAX	0x7FFFFFFFFFFFFFFFLL
#define ULLONG_MAX	0xFFFFFFFFFFFFFFFFULL

#endif/*LIMITS_H*/