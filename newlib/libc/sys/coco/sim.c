/*
 * Copyright 2006 by Brian Dominy <brian@oddchange.com>
 *
 * System calls for m6809.
 */


/*
 * The main implementations of the routines are given by the
 * _r versions, which are intended to be reentrant.
 * Reentrancy is not yet implemented however, so the reent_ptr
 * is not passed along.
 */


int isatty (int fd)
{
	return 1;
}


int fstat (int fd, void *buf)
{
	return -1;
}


int lseek (int fd, int offset, int whence)
{
	return -1;
}


int close (int fd)
{
	return -1;
}


int getpid (void)
{
	return 1;
}


