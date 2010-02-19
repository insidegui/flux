/* Copyright 2010 Nick Johnson */

#ifndef ARCH_H
#define ARCH_H

#define ARCH_X86

/***************************
 *                         *
 * Intel x86 configuration *
 *                         *
 ***************************/
 
#ifdef ARCH_X86

/*** Integer and Pointer Types ***/

#define NULL ((void*) 0)

#define INTBITS 32
#define PTRBITS 32

#ifndef ASM

/* Precise size integers */
typedef unsigned char			uint8_t;
typedef unsigned short			uint16_t;
typedef unsigned int			uint32_t;
typedef unsigned long long int	uint64_t;
typedef signed char				int8_t;
typedef signed short			int16_t;
typedef signed int				int32_t;
typedef signed long long int	int64_t;

/* Minimum size integers */
typedef uint32_t		uint_least8_t;
typedef uint32_t		uint_least16_t;
typedef uint32_t		uint_least32_t;
typedef uint64_t		uint_least64_t;
typedef int32_t			int_least8_t;
typedef int32_t			int_least16_t;
typedef int32_t			int_least32_t;
typedef int64_t			int_least64_t;

/* Fast integers */
typedef uint32_t		uint_fast8_t;
typedef uint32_t		uint_fast16_t;
typedef uint32_t		uint_fast32_t;
typedef uint64_t		uint_fast64_t;
typedef int32_t			int_fast8_t;
typedef int32_t			int_fast16_t;
typedef int32_t			int_fast32_t;
typedef int64_t			int_fast64_t;

/* Pointer equivalents */
typedef uint32_t		uintptr_t;
typedef int32_t			intptr_t;

/* Maximum size integers */
typedef uint64_t		uintmax_t;
typedef int64_t			intmax_t;

/* Boolean values */
typedef uint8_t 		bool;
#define true 1
#define false 0

/* Various integer types */
typedef uint32_t		size_t;

/*** Architecture Optimized Functions ***/

void x86_memcpy(void *dest, void *src, size_t size);
void x86_memclr(void *s, size_t size);

#endif/*ASM*/

/*** Address Space Information ***/

#define PAGESZ 0x1000

/* Address space layout */

#define EXEC_START	0x00400000
#define HEAP_START	0x10000000
#define HEAP_MXBRK	0x20000000

/* These must remain in order */
#define ESPACE	0xFC000000	/* Temporary space for exec data */
#define SSPACE	0xFCF00000	/* Stack space (grows down from next address) */
#define LSPACE	0xFD000000	/* Space for exec bootstrap */
#define KSPACE 	0xFE000000	/* Kernel space */

#endif/*ARCH_X86*/

#endif/*ARCH_H*/
