#include "ft_ls.h"
#include <stdio.h>
#include <string.h>

int	parse_args(int argc, char **argv, t_options *options, t_entries *paths)
{
	int	i;
	int	j;
	bool	end_options;

	end_options = false;
	i = 1;
	while (i < argc)
	{
		if (!end_options && strcmp(argv[i], "--") == 0)
			end_options = true;
		else if (!end_options && argv[i][0] == '-' && argv[i][1])
		{
			j = 1;
			while (argv[i][j])
			{
				if (argv[i][j] == 'l')
					options->long_format = true;
				else if (argv[i][j] == 'a')
					options->show_all = true;
				else
				{
					fprintf(stderr, "ft_ls: unsupported option -- '%c'\n",
						argv[i][j]);
					return (2);
				}
				j++;
			}
		}
		else if (add_entry(paths, argv[i]) != 0)
			return (perror("ft_ls: arguments"), 2);
		i++;
	}
	if (paths->count == 0 && add_entry(paths, ".") != 0)
		return (perror("ft_ls: arguments"), 2);
	return (0);
}
