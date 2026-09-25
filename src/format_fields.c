#define _XOPEN_SOURCE 700
#include "ft_ls.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <sys/sysmacros.h>
#include <time.h>
#include <wchar.h>

int	text_width(const char *text)
{
	mbstate_t	state;
	wchar_t		character;
	size_t		length;
	size_t		remaining;
	int			width;
	int			columns;

	state = (mbstate_t){0};
	width = 0;
	remaining = strlen(text);
	while (*text)
	{
		length = mbrtowc(&character, text, remaining, &state);
		if (length == (size_t)-1 || length == (size_t)-2)
			return (width + remaining);
		columns = wcwidth(character);
		if (columns > 0)
			width += columns;
		text += length;
		remaining -= length;
	}
	return (width);
}

void	format_size(const struct stat *info, char text[64])
{
	if (S_ISCHR(info->st_mode) || S_ISBLK(info->st_mode))
		snprintf(text, 64, "%u, %u", major(info->st_rdev), minor(info->st_rdev));
	else
		snprintf(text, 64, "%jd", (intmax_t)info->st_size);
}

void	format_date(time_t modified, char text[128])
{
	struct tm	*date;
	const char	*format;
	double		age;

	age = difftime(time(NULL), modified);
	format = "%b %e %H:%M";
	if (age > 31556952.0 / 2 || age < 0)
		format = "%b %e  %Y";
	date = localtime(&modified);
	if (date && strftime(text, 128, format, date))
		return ;
	else
		snprintf(text, 128, "%jd", (intmax_t)modified);
}

