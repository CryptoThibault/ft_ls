#include "ft_ls.h"
#include <grp.h>
#include <inttypes.h>
#include <pwd.h>
#include <stdio.h>

static void	widen(int *width, int length)
{
	if (length > *width)
		*width = length;
}

static void	measure_info(const struct stat *info, t_widths *widths)
{
	struct passwd	*owner;
	struct group	*group;
	char			text[128];

	widen(&widths->links, snprintf(text, sizeof(text), "%ju",
			(uintmax_t)info->st_nlink));
	owner = getpwuid(info->st_uid);
	if (owner)
		widen(&widths->owner, text_width(owner->pw_name));
	else
		widen(&widths->owner, snprintf(text, sizeof(text), "%ju",
				(uintmax_t)info->st_uid));
	group = getgrgid(info->st_gid);
	if (group)
		widen(&widths->group, text_width(group->gr_name));
	else
		widen(&widths->group, snprintf(text, sizeof(text), "%ju",
				(uintmax_t)info->st_gid));
	format_size(info, text);
	widen(&widths->size, text_width(text));
	format_date(info->st_mtime, text);
	widen(&widths->date, text_width(text));
}

void	measure_entries(const t_entries *entries, t_widths *widths)
{
	size_t	i;

	*widths = (t_widths){0};
	i = 0;
	while (i < entries->count)
	{
		if (entries->items[i].valid)
			measure_info(&entries->items[i].info, widths);
		i++;
	}
}
