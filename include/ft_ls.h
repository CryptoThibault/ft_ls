#ifndef FT_LS_H
# define FT_LS_H

# include <stdbool.h>
# include <stddef.h>

typedef struct s_options
{
	bool	show_all;
	bool	long_format;
	bool	recursive;
	bool	reverse;
	bool	sort_by_time;
}	t_options;

typedef struct s_entries
{
	char	**names;
	size_t	count;
	size_t	capacity;
}	t_entries;

int	add_entry(t_entries *entries, const char *name);
void	free_entries(t_entries *entries);
void	sort_entries(t_entries *entries);
int	print_entries(const t_entries *entries);
int	read_entries(const char *path, const t_options *options,
		t_entries *entries);

#endif
