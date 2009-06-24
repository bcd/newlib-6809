
#ifndef __M6809_REGS_H
#define __M6809_REGS_H

enum cc_flags
{
	CC_CARRY = 0x1,
	CC_OVERFLOW = 0x2,
	CC_ZERO = 0x4,
	CC_NEGATIVE = 0x8,
	CC_IRQ = 0x10,
	CC_HALF = 0x20,
	CC_FIRQ = 0x40,
	CC_E = 0x80,
};

register unsigned char __cc asm ("cc");


extern inline void setstack (const unsigned int s)
{
	asm __volatile__ ("lds\t%0" :: "g" (s) : "d");
}


extern inline unsigned int getstack (void)
{
	unsigned int result;
	asm __volatile__ ("lea%0\t,s" : "=a" (result));
	return result;
}


extern inline void setdp (const unsigned short dp)
{
	asm __volatile__ ("tfr\tb, dp" :: "q" (dp));
}


#endif /* __M6809_REGS_H */
