#include "ft_ls.h"
#include <stdio.h>

int	print_entries(const t_entries *entries)
{
	size_t	i;

	i = 0;
	while (i < entries->count)
	{
		if (puts(entries->names[i]) == EOF)
		{
			perror("ft_ls: stdout");
			return (1);
		}
		i++;
	}
	return (0);
}
