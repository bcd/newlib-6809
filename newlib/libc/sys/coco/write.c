

int write (int fd, const char *buf, int len)
{
	while (len > 0)
	{
		asm __volatile__ ("lda\t,%0" :: "a"(buf++));
		asm __volatile__ ("jsr\t[0xA002]");
		len--;
	}
}


