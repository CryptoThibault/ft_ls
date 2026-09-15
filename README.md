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

## Build

```sh
make
```

The Makefile also provides `clean`, `fclean`, and `re` targets. Compilation must remain compatible with `-Wall -Wextra -Werror`.
