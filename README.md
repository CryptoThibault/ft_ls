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
line, sorted by name using the environment's locale. `-l` enables long listings:
file type, permissions (including special bits), hard-link count, owner, group,
size, modification date and symbolic-link target. Device files show major and
minor numbers instead of a size. Directory listings include a `total` line in
1 KiB units. Numeric IDs are used when owner or group names cannot be resolved.
Column widths are measured for each listing before display, aligning metadata,
dates and filenames. Text widths account for multibyte characters.

Arguments may contain files and directories, with options before or after paths.
`-a` includes all hidden entries, including `.` and `..`. It combines with `-l`
as `-la`, `-al` or separate arguments. Repeated options are accepted. `--` ends option
parsing, so `./ft_ls -l -- -example` lists a name beginning with a dash. Without
paths, the program uses `.`. Unsupported options (including the planned
`-r` and `-t`) produce an error before any listing.

`-R` lists each directory before recursively visiting its subdirectories in sorted
order. It combines with `-a` and `-l` (for example `-laR`). Each directory gets a
path heading; `-l` also prints its own total and aligned columns. Hidden directories
are visited only with `-a`. The entries `.` and `..` are displayed with `-a` but
never traversed. Symbolic links found inside directories are never followed.

File operands are displayed before directory contents; both groups are sorted.
Multiple paths enable directory headings. With `-l`, symbolic-link operands are
shown as links; without `-l`, links to directories are traversed. Errors do not
prevent processing other valid paths and produce a nonzero exit status.

```sh
./ft_ls -l
./ft_ls -l src Makefile
./ft_ls -laR src
./ft_ls src -l
./ft_ls -l -- -example
```

GNU-specific environment overrides such as `TIME_STYLE`, `QUOTING_STYLE` and
block-size variables are not implemented. Names are printed literally. Dates use
the locale's abbreviated month, day and time (or year for old or future dates).

## Project structure

- `src/main.c` initializes the locale, invokes argument parsing and listing,
  frees arguments and checks output errors.
- `src/parse_args.c` validates options and collects path operands.
- `src/list_paths.c` separates files and directories and coordinates each listing.
- `src/list_directory.c` displays directories and performs recursive traversal.
- `src/read_entries.c` opens, reads and closes a directory and filters hidden
  entries.
- `src/store_entries.c` grows the entry array, copies names and frees storage.
- `src/sort_entries.c` sorts names with `strcoll`, falling back to `strcmp` for
  names that compare equally under the active locale.
- `src/entry_path.c` constructs paths for directory entries.
- `src/print_entries.c` selects simple or long display and computes directory totals.
- `src/print_long.c` displays metadata, resolves owner/group names and reads links.
- `src/measure_entries.c` computes column widths for each listing.
- `src/format_fields.c` formats dates and sizes and measures text display widths.
- `src/file_mode.c` formats file types and permission bits.
- `include/ft_ls.h` declares the shared interfaces and the `t_options` structure,
  with fields for `-a`, `-l`, `-R`, `-r` and `-t`.

Each source file owns one focused feature. Directory traversal uses depth-first
recursion and closes each directory stream before descending. Active ancestor
device/inode pairs detect directory cycles without suppressing repeated operands.
Unreadable directories are reported and skipped while other directories continue.
Directory reading accepts a path and shared options.
Names are copied because `readdir` may reuse its storage on the next call. The
array grows geometrically; its caller frees it even when reading fails.

Long display uses `lstat` to preserve symbolic-link metadata, then `readlink` for
the target. Target buffers grow as needed. Directory totals and rows currently
read metadata separately; caching metadata can be introduced alongside time
sorting. As with any live directory listing, files can change during traversal.

## Build

```sh
make
```

The Makefile also provides `clean`, `fclean`, and `re` targets. Compilation must remain compatible with `-Wall -Wextra -Werror`.
