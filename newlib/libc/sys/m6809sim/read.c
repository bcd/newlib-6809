

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
	int _len = len;

	if (fd != 0)
		return (-1);

	while (len-- > 0)
	{
		*buf++ = inbyte ();
		_len--;
	}
	return len;
}


