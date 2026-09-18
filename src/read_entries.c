#include "ft_ls.h"
#include <dirent.h>
#include <errno.h>
#include <stdio.h>

static int	collect_entries(DIR *directory, const char *path,
		const t_options *options, t_entries *entries)
{
	struct dirent	*entry;

	while (1)
	{
		errno = 0;
		entry = readdir(directory);
		if (entry == NULL)
			break ;
		if (entry->d_name[0] == '.' && !options->show_all)
			continue ;
		if (add_entry(entries, entry->d_name) != 0)
		{
			perror("ft_ls: entry allocation");
			return (1);
		}
	}
	if (errno != 0)
	{
		perror(path);
		return (1);
	}
	return (0);
}

int	read_entries(const char *path, const t_options *options,
		t_entries *entries)
{
	DIR			*directory;
	int			status;

	directory = opendir(path);
	if (directory == NULL)
	{
		perror(path);
		return (1);
	}
	status = collect_entries(directory, path, options, entries);
	if (closedir(directory) == -1)
	{
		perror(path);
		status = 1;
	}
	return (status);
}
