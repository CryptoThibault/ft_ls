#include "ft_ls.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_ancestor
{
	dev_t					device;
	ino_t					inode;
	const struct s_ancestor	*parent;
}	t_ancestor;

static int	walk_directory(const char *path, const t_options *options,
		const t_ancestor *parent, bool heading, bool *printed);

static int	visit_child(const char *path, const t_options *options,
		const t_ancestor *parent, bool *printed)
{
	struct stat	info;

	if (lstat(path, &info) == -1)
		return (perror(path), 1);
	if (!S_ISDIR(info.st_mode))
		return (0);
	return (walk_directory(path, options, parent, true, printed));
}

static int	visit_children(const char *path, const t_entries *entries,
		const t_options *options, const t_ancestor *parent, bool *printed)
{
	size_t	i;
	char	*child;
	int		status;

	i = 0;
	status = 0;
	while (i < entries->count)
	{
		if (strcmp(entries->names[i], ".") == 0
			|| strcmp(entries->names[i], "..") == 0)
		{
			i++;
			continue ;
		}
		child = entry_path(path, entries->names[i++]);
		if (child == NULL)
			return (perror("ft_ls: path allocation"), 1);
		if (visit_child(child, options, parent, printed) != 0)
			status = 1;
		free(child);
		if (ferror(stdout))
			break ;
	}
	return (status);
}

static int	check_ancestor(const char *path, t_ancestor *current,
		const t_ancestor *parent)
{
	struct stat	info;

	if (stat(path, &info) == -1)
		return (perror(path), 1);
	current->device = info.st_dev;
	current->inode = info.st_ino;
	current->parent = parent;
	while (parent)
	{
		if (parent->device == info.st_dev && parent->inode == info.st_ino)
		{
			fprintf(stderr, "ft_ls: %s: recursive directory loop\n", path);
			return (1);
		}
		parent = parent->parent;
	}
	return (0);
}

static int	walk_directory(const char *path, const t_options *options,
		const t_ancestor *parent, bool heading, bool *printed)
{
	t_entries	entries;
	t_ancestor	current;
	int			status;

	if (check_ancestor(path, &current, parent) != 0)
		return (1 + (parent == NULL));
	entries = (t_entries){0};
	status = read_entries(path, options, &entries);
	if (status != 0)
	{
		free_entries(&entries);
		return (1 + (parent == NULL));
	}
	if (*printed)
		putchar('\n');
	if (heading)
		printf("%s:\n", path);
	*printed = true;
	sort_entries(&entries);
	if (options->long_format && status == 0
		&& print_total(&entries, path) != 0)
		status = 1;
	if (print_entries(&entries, path, options) != 0 && status == 0)
		status = 1;
	if (options->recursive && !ferror(stdout)
		&& visit_children(path, &entries, options, &current, printed) != 0 && status == 0)
		status = 1;
	free_entries(&entries);
	return (status);
}

int	list_directory(const char *path, const t_options *options,
		bool heading, bool *printed)
{
	return (walk_directory(path, options, NULL, heading, printed));
}
