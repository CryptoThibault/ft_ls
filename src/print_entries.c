#include "ft_ls.h"
#include <inttypes.h>
#include <stdio.h>

int	print_entries(const t_entries *entries, const t_options *options)
{
	size_t		i;
	const t_entry	*entry;
	int			status;
	t_widths	widths;

	widths = (t_widths){0};
	if (options->long_format)
		measure_entries(entries, &widths);
	i = 0;
	status = 0;
	while (i < entries->count)
	{
		entry = &entries->items[i++];
		if (!options->long_format)
		{
			if (puts(entry->name) == EOF)
				return (perror("ft_ls: stdout"), 1);
		}
		else if (entry->valid
			&& print_long(entry->path, entry->name, &entry->info, &widths) != 0)
			status = 1;
	}
	return (status);
}

int	print_total(const t_entries *entries)
{
	size_t		i;
	uintmax_t	blocks;

	i = 0;
	blocks = 0;
	while (i < entries->count)
	{
		if (entries->items[i].valid)
			blocks += entries->items[i].info.st_blocks;
		i++;
	}
	if (printf("total %ju\n", (blocks + 1) / 2) < 0)
		return (perror("ft_ls: stdout"), 1);
	return (0);
}
