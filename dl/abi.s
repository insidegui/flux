; Copyright (C) 2009, 2010 Nick Johnson <nickbjohnson4224 at gmail.com>
; 
; Permission to use, copy, modify, and distribute this software for any
; purpose with or without fee is hereby granted, provided that the above
; copyright notice and this permission notice appear in all copies.
; 
; THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
; WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
; MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
; ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
; WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
; ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
; OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

[bits 32]

section .text

global _dl_page:function _dl_page.end-_dl_page
global _dl_when:function _dl_when.end-_dl_when

_dl_page:
	push ebx
	push edi
	push esi

	mov ebx, [esp+16]
	mov ecx, [esp+20]
	mov edx, [esp+24]
	mov esi, [esp+28]
	mov edi, [esp+32]
	int 0x46

	pop esi
	pop edi
	pop ebx
	ret
.end:

_dl_when:
	mov ecx, [esp+4]
	int 0x42
	ret
.end:
