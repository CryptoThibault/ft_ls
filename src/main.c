#include "ft_ls.h"
#include <locale.h>
#include <stdio.h>

int	main(int argc, char **argv)
{
	t_options	options;
	t_entries	entries;
	int			status;

	if (argc != 1)
	{
		fprintf(stderr, "%s: arguments are not supported yet\n", argv[0]);
		return (2);
	}
	options = (t_options){0};
	setlocale(LC_ALL, "");
	entries = (t_entries){0};
	status = read_entries(".", &options, &entries);
	if (status == 0)
	{
		sort_entries(&entries);
		status = print_entries(&entries);
	}
	free_entries(&entries);
	if (fflush(stdout) == EOF)
	{
		perror("ft_ls: stdout");
		return (1);
	}
	return (status);
}
