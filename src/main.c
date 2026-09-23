#include "ft_ls.h"
#include <locale.h>
#include <stdio.h>

int	main(int argc, char **argv)
{
	t_options	options;
	t_entries	paths;
	int			status;

	options = (t_options){0};
	paths = (t_entries){0};
	setlocale(LC_ALL, "");
	status = parse_args(argc, argv, &options, &paths);
	if (status == 0)
		status = list_paths(&paths, &options);
	free_entries(&paths);
	if (fflush(stdout) == EOF || ferror(stdout))
	{
		perror("ft_ls: stdout");
		if (status == 0)
			status = 1;
	}
	return (status);
}
