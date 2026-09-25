#include "ft_ls.h"
#include <stdio.h>

static int	classify_paths(t_entries *paths, t_entries *files,
		t_entries *directories)
{
	size_t		i;
	t_entries	*destination;

	i = 0;
	while (i < paths->count)
	{
		if (paths->items[i].valid)
		{
			destination = files;
			if (S_ISDIR(paths->items[i].info.st_mode))
				destination = directories;
			if (move_entry(destination, &paths->items[i]) != 0)
				return (perror("ft_ls: paths allocation"), 2);
		}
		i++;
	}
	return (0);
}

int	list_paths(t_entries *paths, const t_options *options)
{
	t_entries	files;
	t_entries	directories;
	size_t		i;
	int			status;
	int			directory_status;
	bool		printed;

	files = (t_entries){0};
	directories = (t_entries){0};
	status = 0;
	if (load_entries(paths, NULL, options) != 0)
		status = 2;
	if (classify_paths(paths, &files, &directories) != 0)
		status = 2;
	sort_entries(&files, options);
	sort_entries(&directories, options);
	if (print_entries(&files, options) != 0 && status == 0)
		status = 1;
	i = 0;
	printed = files.count > 0;
	while (i < directories.count)
	{
		directory_status = list_directory(directories.items[i].path, options,
				paths->count > 1 || options->recursive, &printed);
		if (directory_status > status)
			status = directory_status;
		i++;
	}
	free_entries(&files);
	free_entries(&directories);
	return (status);
}
