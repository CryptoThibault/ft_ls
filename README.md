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
paths, the program uses `.`. Unsupported options produce an error before any listing.

`-R` lists each directory before recursively visiting its subdirectories in sorted
order. It combines with `-a` and `-l` (for example `-laR`). Each directory gets a
path heading; `-l` also prints its own total and aligned columns. Hidden directories
are visited only with `-a`. The entries `.` and `..` are displayed with `-a` but
never traversed. Symbolic links found inside directories are never followed.

`-r` reverses the sorted order of entries and path operands. Files still appear
before directories; each group is reversed independently. With `-R`, subdirectories
are visited in reverse order too. Repeated `-r` keeps reverse sorting enabled.

`-t` sorts by modification time, newest first, including nanoseconds. Equal
timestamps are ordered by name using the active locale. `-tr` reverses the whole
order (oldest first, with reversed name ordering for ties). Recursive traversal
uses the same ordering. Sorting reuses cached metadata and performs no filesystem calls. Directory entries use
`lstat`; command-line links to directories use the target's time when traversed
without `-l`.

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
- `src/store_entries.c` owns entry storage, grows arrays, transfers entries and
  frees their names and paths.
- `src/load_entries.c` loads metadata once and applies operand symlink rules.
- `src/sort_entries.c` sorts cached entries by name or timestamp, uses locale-aware
  name comparison for ties, and applies reverse ordering.
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

Each `t_entry` owns a name, an optional full path, cached `struct stat` metadata
and a validity flag. Collection, metadata loading, sorting and display are separate
steps. `load_entries` performs one `lstat` per entry when metadata is needed for
`-l`, `-t`, `-R` or operand classification. Simple directory listings without these
options avoid metadata calls and full-path allocation entirely.

The total, column widths, long display, time sorting and child-directory selection
reuse this snapshot. The temporary timestamp array is no longer needed. Operand
classification transfers ownership into file/directory arrays instead of copying
names and reloading metadata. Source slots are cleared after transfer, and
`free_entries` releases every owned name and path, including on errors.

Long display still reads symbolic-link targets with `readlink`; traversed directories
are checked again for ancestor cycles. Owner/group lookups and date formatting remain
in the presentation layer. This cache uses more memory per entry in exchange for
fewer filesystem calls and consistent metadata across output columns. It is not an
atomic filesystem snapshot: files can still change or disappear during traversal.

## Validation

`python3 tests/test_ls.py` compares supported behavior with system `ls`, including
all 32 option combinations, nested directories, symlinks, errors and nanosecond
timestamp ties. Tests are local (the existing `.gitignore` excludes `tests/`).
`FT_LS_BINARY` can select an alternate executable, for example a sanitizer build.

## Build

```sh
make
```

The Makefile also provides `clean`, `fclean`, and `re` targets. Compilation must remain compatible with `-Wall -Wextra -Werror`.
