#include "ft_ls.h"
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int	grow_entries(t_entries *entries)
{
	char	**names;
	size_t	capacity;

	if (entries->capacity > SIZE_MAX / sizeof(*names) / 2)
	{
		errno = ENOMEM;
		return (1);
	}
	capacity = 16;
	if (entries->capacity != 0)
		capacity = entries->capacity * 2;
	names = realloc(entries->names, capacity * sizeof(*names));
	if (names == NULL)
		return (1);
	entries->names = names;
	entries->capacity = capacity;
	return (0);
}

int	add_entry(t_entries *entries, const char *name)
{
	char	*copy;

	if (entries->count == entries->capacity && grow_entries(entries) != 0)
		return (1);
	copy = malloc(strlen(name) + 1);
	if (copy == NULL)
		return (1);
	strcpy(copy, name);
	entries->names[entries->count++] = copy;
	return (0);
}

void	free_entries(t_entries *entries)
{
	size_t	i;

	i = 0;
	while (i < entries->count)
		free(entries->names[i++]);
	free(entries->names);
	*entries = (t_entries){0};
}
