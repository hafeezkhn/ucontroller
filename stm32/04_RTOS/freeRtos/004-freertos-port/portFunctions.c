void * memset(void *dest, int c, unsigned int n)
{
    unsigned char *p = dest;
    while (n--)
        *p++ = (unsigned char)c;
    return dest;
}