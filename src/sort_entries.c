#include "ft_ls.h"
#include <stdlib.h>
#include <string.h>

static int	compare_names(const void *left, const void *right)
{
	const char	*first;
	const char	*second;
	int			order;

	first = *(const char *const *)left;
	second = *(const char *const *)right;
	order = strcoll(first, second);
	if (order == 0)
		order = strcmp(first, second);
	return (order);
}

void	sort_entries(t_entries *entries)
{
	if (entries->count > 1)
		qsort(entries->names, entries->count, sizeof(*entries->names),
			compare_names);
}
