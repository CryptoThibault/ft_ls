#include "ft_ls.h"
#include <grp.h>
#include <inttypes.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int	print_target(const char *path)
{
	char	*target;
	size_t	capacity;
	ssize_t	length;
	int		status;

	capacity = 128;
	while (1)
	{
		target = malloc(capacity + 1);
		if (target == NULL)
			return (perror("ft_ls: link allocation"), 1);
		length = readlink(path, target, capacity);
		if (length < 0)
			return (perror(path), free(target), 1);
		if ((size_t)length < capacity)
			break ;
		free(target);
		capacity *= 2;
	}
	target[length] = '\0';
	status = (printf(" -> %s", target) < 0);
	free(target);
	return (status);
}

static void	print_owner(const struct stat *info, const t_widths *widths)
{
	struct passwd	*owner;
	struct group	*group;

	owner = getpwuid(info->st_uid);
	if (owner)
		printf("%s%*s ", owner->pw_name,
			widths->owner - text_width(owner->pw_name), "");
	else
		printf("%-*ju ", widths->owner, (uintmax_t)info->st_uid);
	group = getgrgid(info->st_gid);
	if (group)
		printf("%s%*s ", group->gr_name,
			widths->group - text_width(group->gr_name), "");
	else
		printf("%-*ju ", widths->group, (uintmax_t)info->st_gid);
}

int	print_long(const char *path, const char *name, const struct stat *info,
		const t_widths *widths)
{
	char	date[128];
	char	size[64];
	char	mode[11];
	int		status;

	file_mode(info->st_mode, mode);
	printf("%s %*ju ", mode, widths->links, (uintmax_t)info->st_nlink);
	print_owner(info, widths);
	format_size(info, size);
	printf("%*s ", widths->size, size);
	format_date(info->st_mtime, date);
	printf("%s%*s ", date, widths->date - text_width(date), "");
	printf("%s", name);
	status = 0;
	if (S_ISLNK(info->st_mode))
		status = print_target(path);
	if (putchar('\n') == EOF || ferror(stdout))
		return (perror("ft_ls: stdout"), 1);
	return (status);
}
