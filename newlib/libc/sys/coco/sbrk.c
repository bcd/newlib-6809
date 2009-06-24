

extern void *_data_start asm ("s_.data");

extern void *_data_end asm ("l_.data");


void *sbrk (int val)
{
	static char *heap_ptr = 0;
	char *base;

	if (heap_ptr == 0)
		heap_ptr = (char *)&_data_start + (unsigned int)&_data_end;

	base = heap_ptr;
	heap_ptr += val;
	return (base);
}

