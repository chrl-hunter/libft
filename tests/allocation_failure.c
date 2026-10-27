/* GNU ld --wrap keeps simulated malloc failures local to library calls. */
#include "libft.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static size_t calls;

void *__wrap_malloc(size_t size)
{
    (void)size;
    ++calls;
    return NULL;
}

int main(void)
{
    assert(ft_calloc(SIZE_MAX, 2) == NULL);
    assert(calls == 0); /* Overflow must be rejected before malloc. */
    assert(ft_calloc(2, 4) == NULL);
    assert(ft_strdup("abc") == NULL);
    assert(ft_substr("abc", 1, 2) == NULL);
    assert(ft_substr("abc", 100, 2) == NULL);
    assert(ft_strjoin("ab", "cd") == NULL);
    assert(calls == 5);
    puts("PASS: malloc failure propagation and calloc overflow rejection");
    return 0;
}
