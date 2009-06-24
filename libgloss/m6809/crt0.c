/*
 * Copyright 2006 by Brian Dominy <brian@oddchange.com>
 *
 * This file is part of GCC.
 *
 * GCC is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * GCC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GCC; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */



/** At present, the only supported system is the simulator,
 * identified by TARGET_SIM being defined.  If this isn't
 * defined for some reason, then abort here. */
#ifndef TARGET_SIM
#define TARGET_SIM
#endif

/** Defines for the simulator */
#ifdef TARGET_SIM

#define SIM_OUTPUT_REG 0xE000
#define SIM_EXIT_REG 0xE001
#define SIM_INPUT_REG 0xE002

#define __reset _start
#define __irq __simulator_irq

#endif

/** Declare prototype for main() to avoid a warning */
extern int main (void);


/** Declare prototypes for the following functions defined
 * here, to override any preexisting notion of what these
 * functions should do */
void __simulator_irq_install (void (*handler) (void));
void _exit (int rc);
static void _start (void);


/** Ensure that there is at least one variable in each of
 * the .data and .bss sections to avoid linker errors.
 * aslink barfs if you try to define a section with nothing
 * in it. */
char _crt0_force_data_decl = 0;
const char _crt0_force_bss_decl;

/** An interrupt handler function */
static void (*__simulator_irq_handler) (void);


/** The _exit function, which terminates the program.
 * This is system dependent. */
void _exit (int rc)
{
#ifdef TARGET_SIM
	while (1)
		*(char *)SIM_EXIT_REG = rc;
#endif
}


#ifdef TARGET_SIM
static __attribute__((interrupt)) void __simulator_irq (void)
{
	(*__simulator_irq_handler) ();
}

static void __simulator_nop (void)
{
}

void __simulator_irq_install (void (*handler) (void))
{
	__simulator_irq_handler = handler;
}
#endif


static __attribute__((naked)) void _start (void)
{
	/* Initialize the stack to a safe place first.
	 * This needs to be done before any local variables
	 * are used. */
#ifdef TARGET_SIM
	asm ("lds\t#0x1FFE");
#endif
	{
		int rc;

		/* Zero the BSS section */

		/* If the _DATA section is in ROM, then copy it
		 * into RAM so that it is modifiable */

		/* Initialize the heap */

		/* Call any "initializer" functions */

		/* Enable interrupts on the simulator */
#if defined(TARGET_SIM) && defined(__irq)
		__simulator_irq_handler = __simulator_nop;
		asm ("andcc\t#~(0x10|0x40)");
#endif

		/* Set up the environment */

		/* Set up argc/argv arrays */

		/* Call the main function */
		rc = main ();

		/* Call any finalizer functions */

		/* If main returns, then invoke _exit()
		 * to stop the program */
		_exit (rc);
	}
}


/* When building for the simulator, we include a
 * declaration of the vector table, since the simulator
 * will always start at the reset vector.  Reset is
 * setup here to point to _start.
 */
#ifdef TARGET_SIM

/** The 6809 vector table structure */
typedef struct
{
	void (*unused) (void);
	void (*swi3) (void);
	void (*swi2) (void);
	void (*firq) (void);
	void (*irq) (void);
	void (*swi) (void);
	void (*nmi) (void);
	void (*reset) (void);
} m6809_vector_table_t;

#ifndef __exit
#define __exit __unused_vector
#define __NEED_UNUSED_VECTOR
#endif
#ifndef __swi
#define __swi __unused_vector
#define __NEED_UNUSED_VECTOR
#endif
#ifndef __swi2
#define __swi2 __unused_vector
#define __NEED_UNUSED_VECTOR
#endif
#ifndef __swi3
#define __swi3 __unused_vector
#define __NEED_UNUSED_VECTOR
#endif
#ifndef __irq
#define __irq __unused_vector
#define __NEED_UNUSED_VECTOR
#endif
#ifndef __firq
#define __firq __unused_vector
#define __NEED_UNUSED_VECTOR
#endif
#ifndef __nmi
#define __nmi __unused_vector
#define __NEED_UNUSED_VECTOR
#endif
#ifndef __reset
#define __reset __unused_vector
#define __NEED_UNUSED_VECTOR
#endif


/* Only define this empty function if it is
 * actually needed. */
#ifdef __NEED_UNUSED_VECTOR
static void __unused_vector (void) {}
#endif


extern void __exit (void);
extern void __swi3 (void);
extern void __swi2 (void);
extern void __firq (void);
extern void __irq (void);
extern void __swi (void);
extern void __nmi (void);
extern void __reset (void);

/** Declare the interrupt vector table */
__attribute__((section("vector"))) volatile m6809_vector_table_t vectors = {
	.unused = __exit,
	.swi3 = __swi3,
	.swi2 = __swi2,
	.firq = __firq,
	.irq = __irq,
	.swi = __swi,
	.nmi = __nmi,
	.reset = __reset,
};

#endif /* TARGET_SIM */

