#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
build=$(mktemp -d)
trap 'rm -rf "$build"' EXIT HUP INT TERM
compiler=${CC:-cc}
make CC="$compiler"
"$compiler" -Wall -Wextra -Werror -I. tests/regression.c libft.a -o "$build/regression"
"$build/regression"
# Compile the library itself with instrumentation, not only the test driver.
"$compiler" -Wall -Wextra -Werror -g -fno-omit-frame-pointer \
    -fsanitize=address,undefined -fno-pie -no-pie -I. \
    tests/regression.c ft_*.c -o "$build/regression-sanitized"
"$build/regression-sanitized"
