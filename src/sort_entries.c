#include "ft_ls.h"
#include <stdlib.h>
#include <string.h>

static int	compare_names(const void *left, const void *right)
{
	const t_entry	*a;
	const t_entry	*b;
	int				order;

	a = left;
	b = right;
	order = strcoll(a->name, b->name);
	if (order == 0)
		order = strcmp(a->name, b->name);
	return (order);
}

static int	compare_times(const void *left, const void *right)
{
	const t_entry	*a;
	const t_entry	*b;

	a = left;
	b = right;
	if (a->valid != b->valid)
		return (b->valid - a->valid);
	if (a->info.st_mtim.tv_sec != b->info.st_mtim.tv_sec)
		return ((a->info.st_mtim.tv_sec < b->info.st_mtim.tv_sec) * 2 - 1);
	if (a->info.st_mtim.tv_nsec != b->info.st_mtim.tv_nsec)
		return ((a->info.st_mtim.tv_nsec < b->info.st_mtim.tv_nsec) * 2 - 1);
	return (compare_names(left, right));
}

void	sort_entries(t_entries *entries, const t_options *options)
{
	size_t	i;
	t_entry	entry;
	int		(*compare)(const void *, const void *);

	compare = compare_names;
	if (options->sort_by_time)
		compare = compare_times;
	if (entries->count > 1)
		qsort(entries->items, entries->count, sizeof(*entries->items), compare);
	if (!options->reverse)
		return ;
	i = 0;
	while (i < entries->count / 2)
	{
		entry = entries->items[i];
		entries->items[i] = entries->items[entries->count - 1 - i];
		entries->items[entries->count - 1 - i] = entry;
		i++;
	}
}
