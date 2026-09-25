#include "ft_ls.h"
#include <stdlib.h>
#include <string.h>

char	*entry_path(const char *directory, const char *name)
{
	char	*path;
	size_t	length;

	if (directory == NULL)
		length = 0;
	else
		length = strlen(directory);
	path = malloc(length + strlen(name) + 2);
	if (path == NULL)
		return (NULL);
	if (length)
	{
		memcpy(path, directory, length);
		if (path[length - 1] != '/')
			path[length++] = '/';
	}
	strcpy(path + length, name);
	return (path);
}
