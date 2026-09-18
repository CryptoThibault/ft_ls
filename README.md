# ft_ls

`ft_ls` is a C reimplementation of the system `ls` command.

## Scope

The program must reproduce the behavior and output of the system `ls` command as closely as possible, including these options:

- `-a` — show entries whose names begin with `.`.
- `-l` — use the long listing format.
- `-R` — list directory contents recursively.
- `-r` — reverse the sorting order.
- `-t` — sort entries by modification time.

The interaction of combined options must match the system command. Recursive traversal is a core design concern and should be accounted for from the start.

## Output requirements

- Match the system `ls` display as closely as possible for every supported option.
- Long listings must include all relevant information.
- Exact padding and pagination may differ slightly, but no information may be omitted.
- The multi-column layout used by `ls` when `-l` is absent is not required.
- ACL and extended-attribute indicators are not required.

## Current development stage

Running `./ft_ls` lists non-hidden entries in the current directory, one per
line, sorted by name using the environment's locale. Command-line paths and option
parsing are not implemented yet; arguments are rejected explicitly.

## Project structure

- `src/main.c` initializes the options and coordinates reading, sorting, display
  and cleanup before returning the listing status.
- `src/read_entries.c` opens, reads and closes a directory and filters hidden
  entries.
- `src/store_entries.c` grows the entry array, copies names and frees storage.
- `src/sort_entries.c` sorts names with `strcoll`, falling back to `strcmp` for
  names that compare equally under the active locale.
- `src/print_entries.c` displays entries and reports output errors.
- `include/ft_ls.h` declares the shared interfaces and the `t_options` structure,
  with fields for `-a`, `-l`, `-R`, `-r` and `-t`.

Each source file owns one focused feature. Small startup operations stay in
`main`; option parsing and recursive traversal will get dedicated files
as they are implemented. Directory reading accepts a path and shared options so
it can later be reused for command-line paths and recursive traversal. Names are
copied because `readdir` may reuse its storage on the next call. The array grows
geometrically and is freed by `main` after each listing, including on errors.
`read_entries` receives a zero-initialized array; the caller owns its cleanup
even when reading fails. `main` loads
the environment's locale so sorting follows the same locale settings as `ls`.

## Build

```sh
make
```

The Makefile also provides `clean`, `fclean`, and `re` targets. Compilation must remain compatible with `-Wall -Wextra -Werror`.
