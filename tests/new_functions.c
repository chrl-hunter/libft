/* Development tests; not part of the Libft submission. */
#include "libft.h"
#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int sign_of(int n)
{
    return (n > 0) - (n < 0);
}

static void test_memcmp(void)
{
    unsigned char a[5] = {0, 128, 255, 0, 42}, b[5];
    size_t i, n;
    int c;

    for (i = 0; i < sizeof(a); ++i)
        for (c = 0; c <= 255; ++c) {
            memcpy(b, a, sizeof(a));
            b[i] = (unsigned char)c;
            for (n = 0; n <= sizeof(a); ++n)
                assert(sign_of(ft_memcmp(a, b, n)) == sign_of(memcmp(a, b, n)));
        }
    assert(ft_memcmp(a + sizeof(a), b + sizeof(b), 0) == 0);
}

/* Independent bounded-search oracle: only compare a complete fitting needle. */
static char *reference_strnstr(const char *big, const char *little, size_t n)
{
    size_t i, length = strlen(little);
    if (!length)
        return (char *)big;
    for (i = 0; i < n && big[i]; ++i)
        if (length <= n - i && strncmp(big + i, little, length) == 0)
            return (char *)big + i;
    return NULL;
}

static void test_strnstr(void)
{
    const char *strings[] = {"", "a", "aa", "aaa", "aab", "abac", "abababac",
                             "abcabc", "cab", "\200\377a"};
    size_t i, j, n;
    char *tight = malloc(3);
    assert(tight);
    memcpy(tight, "aab", 3); /* Deliberately lacks a terminator. */
    for (i = 0; i < sizeof(strings) / sizeof(*strings); ++i)
        for (j = 0; j < sizeof(strings) / sizeof(*strings); ++j)
            for (n = 0; n <= 12; ++n)
                assert(ft_strnstr(strings[i], strings[j], n) ==
                       reference_strnstr(strings[i], strings[j], n));
    assert(ft_strnstr("abac", "ab", 4) != NULL);
    assert(ft_strnstr("abac", "abc", 4) == NULL); /* Stale j gave a false match. */
    assert(ft_strnstr(tight, "ab", 3) == tight + 1);
    assert(ft_strnstr(tight, "aabc", 3) == NULL);
    assert(ft_strnstr(tight + 3, "a", 0) == NULL);
    assert(ft_strnstr(tight + 3, "", 0) == tight + 3);
    free(tight);
}

static void test_atoi(void)
{
    const char *cases[] = {"", " \t\n\v\f\r-42tail", "+42", "--1", "-+1",
                           "+-1", "++1", "  + 1", "word42", "0", "-0",
                           "123tail", "  000123", "\r\n+17"};
    size_t i;
    int value;
    char text[64];
    for (i = 0; i < sizeof(cases) / sizeof(*cases); ++i)
        assert(ft_atoi(cases[i]) == atoi(cases[i]));
    snprintf(text, sizeof(text), "%d", INT_MIN);
    assert(ft_atoi(text) == INT_MIN);
    snprintf(text, sizeof(text), "%d", INT_MAX);
    assert(ft_atoi(text) == INT_MAX);
    for (value = -10000; value <= 10000; ++value) {
        snprintf(text, sizeof(text), " \t%+drest", value);
        assert(ft_atoi(text) == value);
    }
}

static void test_allocations(void)
{
    const char *strings[] = {"", "a", "hello", "\200\377"};
    size_t i, j, count, size, k, start, len, wanted;
    unsigned char *memory;
    char *copy, *joined, *sub;
    char expected[32];

    assert(ft_calloc(SIZE_MAX / 2 + 1, 2) == NULL);
    assert(ft_calloc(2, SIZE_MAX) == NULL);
    for (count = 0; count <= 17; ++count)
        for (size = 0; size <= 17; ++size) {
            memory = ft_calloc(count, size);
            assert(memory || count * size == 0);
            for (k = 0; k < count * size; ++k)
                assert(memory[k] == 0);
            free(memory);
        }
    memory = ft_calloc(SIZE_MAX, 0);
    free(memory);
    memory = ft_calloc(0, SIZE_MAX);
    free(memory);
    for (i = 0; i < sizeof(strings) / sizeof(*strings); ++i) {
        copy = ft_strdup(strings[i]);
        assert(copy && copy != strings[i] && strcmp(copy, strings[i]) == 0);
        copy[0] = 'X';
        free(copy);
        for (j = 0; j < sizeof(strings) / sizeof(*strings); ++j) {
            strcpy(expected, strings[i]);
            strcat(expected, strings[j]);
            joined = ft_strjoin(strings[i], strings[j]);
            assert(joined && strcmp(joined, expected) == 0);
            free(joined);
        }
        for (start = 0; start <= strlen(strings[i]) + 2; ++start)
            for (len = 0; len <= 8; ++len) {
                wanted = start >= strlen(strings[i]) ? 0 : strlen(strings[i]) - start;
                if (wanted > len)
                    wanted = len;
                if (wanted)
                    memcpy(expected, strings[i] + start, wanted);
                expected[wanted] = 0;
                sub = ft_substr(strings[i], (unsigned int)start, len);
                assert(sub && strcmp(sub, expected) == 0);
                free(sub);
            }
    }
    sub = ft_substr("abc", UINT_MAX, SIZE_MAX);
    assert(sub && *sub == 0);
    free(sub);
    sub = ft_substr("abc", 1, SIZE_MAX);
    assert(sub && strcmp(sub, "bc") == 0);
    free(sub);
}

int main(void)
{
    test_memcmp();
    test_strnstr();
    test_atoi();
    test_allocations();
    puts("PASS: 7 new functions, bounded search, integer limits, allocations and overflow");
    return 0;
}
