

/**
 * Trivial implementation of the kill() system call.
 * Since there is no concept of process within the
 * simple simulator architecture, only kill of self
 * does anything.
 */
int kill (int pid, int sig)
{
	if (pid == 1)
		exit (sig);
	return 0;
}


