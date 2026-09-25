#ifndef FT_LS_H
# define FT_LS_H

# include <stdbool.h>
# include <stddef.h>
# include <sys/stat.h>

typedef struct s_options
{
	bool	show_all;
	bool	long_format;
	bool	recursive;
	bool	reverse;
	bool	sort_by_time;
}	t_options;

typedef struct s_entry
{
	char		*name;
	char		*path;
	struct stat	info;
	bool		valid;
}	t_entry;

typedef struct s_entries
{
	t_entry	*items;
	size_t	count;
	size_t	capacity;
}	t_entries;

typedef struct s_widths
{
	int	links;
	int	owner;
	int	group;
	int	size;
	int	date;
}	t_widths;

int	text_width(const char *text);
void	format_date(time_t modified, char text[128]);
void	format_size(const struct stat *info, char text[64]);
void	measure_entries(const t_entries *entries, t_widths *widths);
int	add_entry(t_entries *entries, const char *name);
void	free_entries(t_entries *entries);
int	move_entry(t_entries *entries, t_entry *entry);
int	load_entries(t_entries *entries, const char *directory,
		const t_options *options);
void	sort_entries(t_entries *entries, const t_options *options);
int	parse_args(int argc, char **argv, t_options *options, t_entries *paths);
int	list_directory(const char *path, const t_options *options,
		bool heading, bool *printed);
int	list_paths(t_entries *paths, const t_options *options);
char	*entry_path(const char *directory, const char *name);
void	file_mode(mode_t mode, char text[11]);
int	print_long(const char *path, const char *name, const struct stat *info,
		const t_widths *widths);
int	print_total(const t_entries *entries);
int	print_entries(const t_entries *entries, const t_options *options);
int	read_entries(const char *path, const t_options *options,
		t_entries *entries);

#endif
