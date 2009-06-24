

extern void *_data_start asm ("s_.data");
extern void *_data_end asm ("l_.data");
extern void *_bss_start asm ("s_.bss");
extern void *_bss_end asm ("l_.bss");


void *sbrk (int val)
{
	static char *heap_ptr = (char *)0xFFFF;
	char *base;

	if (heap_ptr == (char *)0xFFFF)
	{
		if (((char *)&_data_start) > ((char *)&_bss_start))
			heap_ptr = (char *)&_data_start + (unsigned int)&_data_end;
		else
			heap_ptr = (char *)&_bss_start + (unsigned int)&_bss_end;
	}

	if (heap_ptr + val > (char *)0xBFFF)
		return (void *)-1;
	base = heap_ptr;
	heap_ptr += val;
	return (base);
}

