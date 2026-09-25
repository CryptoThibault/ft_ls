#include "ft_ls.h"
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int	grow_entries(t_entries *entries)
{
	t_entry	*items;
	size_t	capacity;

	if (entries->capacity > SIZE_MAX / sizeof(*items) / 2)
	{
		errno = ENOMEM;
		return (1);
	}
	capacity = 16;
	if (entries->capacity != 0)
		capacity = entries->capacity * 2;
	items = realloc(entries->items, capacity * sizeof(*items));
	if (items == NULL)
		return (1);
	entries->items = items;
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
	entries->items[entries->count] = (t_entry){0};
	entries->items[entries->count++].name = copy;
	return (0);
}

void	free_entries(t_entries *entries)
{
	size_t	i;

	i = 0;
	while (i < entries->count)
	{
		free(entries->items[i].name);
		free(entries->items[i].path);
		i++;
	}
	free(entries->items);
	*entries = (t_entries){0};
}

int	move_entry(t_entries *entries, t_entry *entry)
{
	if (entries->count == entries->capacity && grow_entries(entries) != 0)
		return (1);
	entries->items[entries->count++] = *entry;
	*entry = (t_entry){0};
	return (0);
}
