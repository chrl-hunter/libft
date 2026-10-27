#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
build=$(mktemp -d)
trap 'rm -rf "$build"' EXIT HUP INT TERM
compiler=${CC:-cc}
make CC="$compiler"
for suite in regression new_functions; do
    "$compiler" -Wall -Wextra -Werror -I. "tests/$suite.c" libft.a -o "$build/$suite"
    "$build/$suite"
    # Instrument the library itself, not only the test driver.
    "$compiler" -Wall -Wextra -Werror -g -fno-omit-frame-pointer \
        -fsanitize=address,undefined -fno-sanitize-recover=all -fno-pie -no-pie -I. \
        "tests/$suite.c" ft_*.c -o "$build/$suite-sanitized"
    "$build/$suite-sanitized"
done
# Requires a linker supporting GNU --wrap (Linux development check).
"$compiler" -Wall -Wextra -Werror -I. tests/allocation_failure.c libft.a \
    -Wl,--wrap=malloc -o "$build/allocation-failure"
"$build/allocation-failure"
