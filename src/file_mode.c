#include "ft_ls.h"

void	file_mode(mode_t mode, char text[11])
{
	const char	*permissions = "rwxrwxrwx";
	int			i;

	text[0] = '-';
	if (S_ISDIR(mode))
		text[0] = 'd';
	else if (S_ISLNK(mode))
		text[0] = 'l';
	else if (S_ISCHR(mode))
		text[0] = 'c';
	else if (S_ISBLK(mode))
		text[0] = 'b';
	else if (S_ISFIFO(mode))
		text[0] = 'p';
	else if (S_ISSOCK(mode))
		text[0] = 's';
	i = 0;
	while (i < 9)
	{
		text[i + 1] = '-';
		if (mode & (0400 >> i))
			text[i + 1] = permissions[i];
		i++;
	}
	if (mode & S_ISUID)
		text[3] = "Ss"[(mode & S_IXUSR) != 0];
	if (mode & S_ISGID)
		text[6] = "Ss"[(mode & S_IXGRP) != 0];
	if (mode & S_ISVTX)
		text[9] = "Tt"[(mode & S_IXOTH) != 0];
	text[10] = '\0';
}
