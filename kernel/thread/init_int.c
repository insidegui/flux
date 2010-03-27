/* 
 * Copyright 2009, 2010 Nick Johnson
 * ISC Licensed, see LICENSE for details 
 */

#include <lib.h>
#include <int.h>
#include <task.h>
#include <init.h>

#ifdef KERNEL_GC
__attribute__ ((section(".itext"))) 
#endif
void init_fault() {

	/* Register all fault handlers */
	register_int(0,  fault_float);
	register_int(1,  fault_generic);
	register_int(2,  fault_generic);
	register_int(3,  fault_generic);
	register_int(4,  fault_generic);
	register_int(5,  fault_generic);
	register_int(6,  fault_generic);
	register_int(7,  fault_float);
	register_int(8,  fault_double);
	register_int(9,  fault_float);
	register_int(10, fault_generic);
	register_int(11, fault_generic);
	register_int(12, fault_generic);
	register_int(13, fault_generic);
	register_int(14, fault_page);
	register_int(15, fault_generic);
	register_int(16, fault_float);
	register_int(17, fault_generic);
	register_int(18, fault_generic);

}

#ifdef KERNEL_GC
__attribute__ ((section(".itext"))) 
#endif
void init_pit() {
	uint16_t divisor;
	printk("  Kernel: system timer @ 256Hz");

		/* Register IRQ handler (PIT == IRQ 0) */
		register_int(IRQ(0), pit_handler);

		/* Set the PIT frequency to 256Hz - a nice round number */
		divisor = 1193180 / 256;
		outb(0x43, 0x36);
		outb(0x40, (uint8_t) (divisor & 0xFF));
		outb(0x40, (uint8_t) (divisor >> 8));

	cursek(74, -1);
	printk("[done]");
}

/* IRQ redirection table */
uint16_t irq_holder[15];

#ifdef KERNEL_GC
__attribute__ ((section(".itext"))) 
#endif
void init_int() {
	extern handler_t int_handlers[128];
	extern void halt(void);

	printk("  Kernel: interrupts");

		/* Set up interupt descriptor table */
		init_idt();

		/* Clear IRQ redirection table */
		memclr(irq_holder, sizeof(pid_t) * 15);

	cursek(74, -1);
	printk("[done]");
	printk("  Kernel: system calls");

		/* Clear the interrupt handler table */
		memclr(int_handlers, sizeof(handler_t) * 96);

		/* Initialize the TSS */
		init_tss();

		/* Set the PIC to mask all bit IRQ 0 */
		pic_mask(0x0001);

		/* ABI 2 system calls */
		register_int(0x60, fire);
		register_int(0x61, drop);
		register_int(0x62, hand);
		register_int(0x63, ctrl);
		register_int(0x64, info);
		register_int(0x65, mmap);
		register_int(0x66, fork);
		register_int(0x67, exit);

		/* Register fault handlers */
		init_fault();

	cursek(74, -1);
	printk("[done]");
}