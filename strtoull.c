#include <climits>

int xisdigit(const int ch)
{
	return (ch >= '0') && (ch <= '9');
}

int isxdigit(const int ch)
{
	if (xisdigit(ch))
		return true;

	if ((ch >= 'a') && (ch <= 'f'))
		return true;

	return (ch >= 'A') && (ch <= 'F');
}

char xtolower(const char c)
{
	return c | 0x20;
}

const char *parse_integer_fixup_radix(const char *s, unsigned int *base)
{
	if (*base == 0) {
		if (s[0] == '0') {
			if (xtolower(s[1]) == 'x' && isxdigit(s[2]))
				*base = 16;
			else
				*base = 8;
		} else
			*base = 10;
	}
	if (*base == 16 && s[0] == '0' && xtolower(s[1]) == 'x')
		s += 2;
	return s;
}

unsigned long long xstrtoull(const char *nptr, char **endptr, unsigned int base) {
    const char *s = parse_integer_fixup_radix(nptr, &base);
    unsigned long long acc;
    const unsigned long long cutoff = ULLONG_MAX / base;
    const unsigned long long cutlim = ULLONG_MAX % base;
    int any = 0;

    for (acc = 0;; s++) {
        unsigned char c = xtolower(*s);
        if (!isxdigit(c))
            break;
        if (xisdigit(c))
            c -= '0';
        else
            c -= 'a' - 10;
        if (c >= base)
            break;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc *= base;
            acc += c;
        }
    }

    if (any < 0) {
        acc = ULLONG_MAX;
    }

    if (nullptr != endptr)
        *endptr = const_cast<char*>(any ? s : nptr);

    return acc;
}
