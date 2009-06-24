

/**
 * outbyte : Low-level function to write a character to
 * the console.
 */
inline void outbyte (unsigned char c)
{
	*(unsigned char *)0xFF00 = c;
}


/**
 * write : Trivial implementation of the write() system call.
 * This only supports the console device.
 */
int write (int fd, const char *buf, int len)
{
	if ((fd == 1) || (fd == 2))
	{
		while (len-- > 0)
			outbyte (*buf++);
		return (0);
	}
	else
	{
		return (-1);
	}
}


