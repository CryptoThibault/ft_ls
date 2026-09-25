#include "ft_ls.h"
#include <stdio.h>

static int	load_entry(t_entry *entry, const char *directory,
		const t_options *options)
{
	struct stat	target;

	entry->path = entry_path(directory, entry->name);
	if (entry->path == NULL)
		return (perror("ft_ls: path allocation"), 1);
	if (lstat(entry->path, &entry->info) == -1)
		return (perror(entry->path), 1);
	if (directory == NULL && !options->long_format
		&& S_ISLNK(entry->info.st_mode)
		&& stat(entry->path, &target) == 0 && S_ISDIR(target.st_mode))
		entry->info = target;
	entry->valid = true;
	return (0);
}

int	load_entries(t_entries *entries, const char *directory,
		const t_options *options)
{
	size_t	i;
	int		status;

	if (directory != NULL && !options->long_format
		&& !options->sort_by_time && !options->recursive)
		return (0);
	i = 0;
	status = 0;
	while (i < entries->count)
	{
		if (entries->items[i].path == NULL
			&& load_entry(&entries->items[i], directory, options) != 0)
			status = 1;
		i++;
	}
	return (status);
}
