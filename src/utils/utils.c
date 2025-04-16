#include "commons.h"

int	is_whitespace(char c)
{
	return (c == ' ' || ('\t' <= c && c <= '\r'));
}