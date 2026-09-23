#include "ft_ls.h"
#include <stdio.h>

static int	classify_paths(const t_entries *paths, const t_options *options,
		t_entries *files, t_entries *directories)
{
	size_t		i;
	struct stat	info;
	struct stat	target;
	int			status;
	t_entries	*destination;

	i = 0;
	status = 0;
	while (i < paths->count)
	{
		if (lstat(paths->names[i], &info) == -1)
		{
			perror(paths->names[i++]);
			status = 2;
			continue ;
		}
		if (!options->long_format && S_ISLNK(info.st_mode)
			&& stat(paths->names[i], &target) == 0 && S_ISDIR(target.st_mode))
			info = target;
		destination = files;
		if (S_ISDIR(info.st_mode))
			destination = directories;
		if (add_entry(destination, paths->names[i++]) != 0)
			return (perror("ft_ls: paths allocation"), 2);
	}
	return (status);
}

static int	list_directory(const char *path, const t_options *options)
{
	t_entries	entries;
	int			status;

	entries = (t_entries){0};
	status = read_entries(path, options, &entries);
	sort_entries(&entries);
	if (options->long_format && status == 0
		&& print_total(&entries, path) != 0)
		status = 1;
	if (print_entries(&entries, path, options) != 0)
		status = 1;
	free_entries(&entries);
	return (status);
}

int	list_paths(t_entries *paths, const t_options *options)
{
	t_entries	files;
	t_entries	directories;
	size_t		i;
	int			status;

	files = (t_entries){0};
	directories = (t_entries){0};
	sort_entries(paths);
	status = classify_paths(paths, options, &files, &directories);
	if (print_entries(&files, NULL, options) != 0 && status == 0)
		status = 1;
	i = 0;
	while (i < directories.count)
	{
		if (i > 0 || files.count > 0)
			putchar('\n');
		if (paths->count > 1)
			printf("%s:\n", directories.names[i]);
		if (list_directory(directories.names[i], options) != 0 && status == 0)
			status = 1;
		i++;
	}
	free_entries(&files);
	free_entries(&directories);
	return (status);
}
