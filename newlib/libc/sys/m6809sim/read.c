

/**
 * inbyte : Low-level function that reads a byte from the console.
 */
inline unsigned char inbyte (void)
{
	return *(unsigned char *)0xFF02;
}


/**
 * read : Trivial implementation of the read() system call.
 * This only supports reading from the console.
 */
int read (int fd, char *buf, int len)
{
	if (fd == 0)
	{
		while (len-- > 0)
			*buf++ = inbyte ();
		return (0);
	}
	else
	{
		return (-1);
	}
}


