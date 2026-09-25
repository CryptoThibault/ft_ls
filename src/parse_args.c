#include "ft_ls.h"
#include <stdio.h>
#include <string.h>

static int	parse_options(const char *argument, t_options *options)
{
	size_t	i;

	i = 1;
	while (argument[i])
	{
		if (argument[i] == 'l')
			options->long_format = true;
		else if (argument[i] == 't')
			options->sort_by_time = true;
		else if (argument[i] == 'r')
			options->reverse = true;
		else if (argument[i] == 'R')
			options->recursive = true;
		else if (argument[i] == 'a')
			options->show_all = true;
		else
		{
			fprintf(stderr, "ft_ls: unsupported option -- '%c'\n", argument[i]);
			return (2);
		}
		i++;
	}
	return (0);
}

int	parse_args(int argc, char **argv, t_options *options, t_entries *paths)
{
	int	i;
	bool	end_options;

	end_options = false;
	i = 1;
	while (i < argc)
	{
		if (!end_options && strcmp(argv[i], "--") == 0)
			end_options = true;
		else if (!end_options && argv[i][0] == '-' && argv[i][1])
		{
			if (parse_options(argv[i], options) != 0)
				return (2);
		}
		else if (add_entry(paths, argv[i]) != 0)
			return (perror("ft_ls: arguments"), 2);
		i++;
	}
	if (paths->count == 0 && add_entry(paths, ".") != 0)
		return (perror("ft_ls: arguments"), 2);
	return (0);
}
