#include "ft_ls.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

int	print_entries(const t_entries *entries, const char *directory,
		const t_options *options)
{
	size_t		i;
	char		*path;
	struct stat	info;
	int			status;
	t_widths	widths;

	widths = (t_widths){0};
	if (options->long_format && measure_entries(entries, directory, &widths))
		return (1);
	i = 0;
	status = 0;
	while (i < entries->count)
	{
		if (!options->long_format)
		{
			if (puts(entries->names[i]) == EOF)
				return (perror("ft_ls: stdout"), 1);
		}
		else
		{
			path = entry_path(directory, entries->names[i]);
			if (path == NULL)
				return (perror("ft_ls: path allocation"), 1);
			if (lstat(path, &info) == -1)
				status = (perror(path), 1);
			else if (print_long(path, entries->names[i], &info, &widths) != 0)
				status = 1;
			free(path);
		}
		i++;
	}
	return (status);
}

int	print_total(const t_entries *entries, const char *directory)
{
	size_t		i;
	char		*path;
	struct stat	info;
	uintmax_t	blocks;

	i = 0;
	blocks = 0;
	while (i < entries->count)
	{
		path = entry_path(directory, entries->names[i++]);
		if (path == NULL)
			return (perror("ft_ls: path allocation"), 1);
		if (lstat(path, &info) == 0)
			blocks += info.st_blocks;
		free(path);
	}
	if (printf("total %ju\n", (blocks + 1) / 2) < 0)
		return (perror("ft_ls: stdout"), 1);
	return (0);
}
