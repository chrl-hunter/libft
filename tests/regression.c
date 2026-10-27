/* Development tests: intentionally separate from the submitted ft_*.c files. */
#include "libft.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

static void test_memmove(void)
{
    unsigned char actual[32], expected[32];
    unsigned char source[32];
    size_t src, dst, n, i;

    for (src = 0; src < 32; ++src)
        for (dst = 0; dst < 32; ++dst)
            for (n = 0; n <= 32 - src && n <= 32 - dst; ++n) {
                for (i = 0; i < 32; ++i)
                    actual[i] = expected[i] = (unsigned char)(i * 17);
                assert(ft_memmove(actual + dst, actual + src, n) == actual + dst);
                memmove(expected + dst, expected + src, n);
                assert(memcmp(actual, expected, 32) == 0);
            }
    for (i = 0; i < 32; ++i)
        source[i] = (unsigned char)i;
    assert(ft_memmove(actual, source, 32) == actual);
    assert(memcmp(actual, source, 32) == 0);
    assert(ft_memmove(source, actual, 32) == source);
}

static void test_strlcat(void)
{
    size_t dl, sl, cap, available, copied, want;
    char src[17], original[17], actual[40], expected[40];
    char *tight;

    for (dl = 0; dl <= 16; ++dl)
        for (sl = 0; sl <= 16; ++sl)
            for (cap = 0; cap <= 20; ++cap) {
                memset(src, 's', sl);
                src[sl] = 0;
                memset(original, 'd', dl);
                original[dl] = 0;
                memset(actual, 0x55, sizeof(actual));
                memcpy(actual, original, dl + 1);
                memcpy(expected, actual, sizeof(actual));
                want = (dl < cap ? dl : cap) + sl;
                if (dl < cap) {
                    available = cap - dl - 1;
                    copied = sl < available ? sl : available;
                    memcpy(expected + dl, src, copied);
                    expected[dl + copied] = 0;
                }
                assert(ft_strlcat(actual, src, cap) == want);
                assert(memcmp(actual, expected, sizeof(actual)) == 0);
            }
    /* Exactly cap bytes and no NUL: ASan detects an unbounded dest scan. */
    for (cap = 1; cap <= 20; ++cap) {
        tight = malloc(cap);
        assert(tight);
        memset(tight, 'x', cap);
        assert(ft_strlcat(tight, "abc", cap) == cap + 3);
        for (dl = 0; dl < cap; ++dl)
            assert(tight[dl] == 'x');
        free(tight);
    }
}

static void test_strlcpy(void)
{
    char src[17], actual[24], expected[24];
    size_t len, cap, copied;

    for (len = 0; len <= 16; ++len)
        for (cap = 0; cap <= 20; ++cap) {
            memset(src, 's', len);
            src[len] = 0;
            memset(actual, 0x55, sizeof(actual));
            memcpy(expected, actual, sizeof(actual));
            if (cap) {
                copied = len < cap - 1 ? len : cap - 1;
                memcpy(expected, src, copied);
                expected[copied] = 0;
            }
            assert(ft_strlcpy(actual, src, cap) == len);
            assert(memcmp(actual, expected, sizeof(actual)) == 0);
        }
}

static int sign_of(int n)
{
    return (n > 0) - (n < 0);
}

static void test_other_functions(void)
{
    const char *strings[] = {"", "a", "abc", "abcd", "abce", "\200\377a"};
    unsigned char bytes[] = {0, 128, 255, 128, 42};
    unsigned char a[5], b[5];
    size_t i, j, n;
    int c;
    char *one, *two;

    for (c = -1; c <= 255; ++c) {
        assert(!!ft_isalpha(c) == !!isalpha(c));
        assert(!!ft_isdigit(c) == !!isdigit(c));
        assert(!!ft_isalnum(c) == !!isalnum(c));
        assert(!!ft_isprint(c) == !!isprint(c));
        assert(!!ft_isascii(c) == (c >= 0 && c <= 127));
        assert(ft_toupper(c) == toupper(c));
        assert(ft_tolower(c) == tolower(c));
    }
    for (i = 0; i < sizeof(strings) / sizeof(*strings); ++i) {
        assert(ft_strlen(strings[i]) == strlen(strings[i]));
        for (c = -256; c <= 511; ++c) {
            assert(ft_strchr(strings[i], c) == strchr(strings[i], c));
            assert(ft_strrchr(strings[i], c) == strrchr(strings[i], c));
        }
        for (j = 0; j < sizeof(strings) / sizeof(*strings); ++j)
            for (n = 0; n <= 7; ++n)
                assert(sign_of(ft_strncmp(strings[i], strings[j], n)) ==
                       sign_of(strncmp(strings[i], strings[j], n)));
    }
    for (n = 0; n <= sizeof(bytes); ++n) {
        for (c = -256; c <= 511; ++c) {
            assert(ft_memchr(bytes, c, n) == memchr(bytes, c, n));
            memset(a, 42, sizeof(a));
            memset(b, 42, sizeof(b));
            assert(ft_memset(a, c, n) == a);
            memset(b, c, n);
            assert(memcmp(a, b, sizeof(a)) == 0);
        }
        memset(a, 42, sizeof(a));
        memset(b, 42, sizeof(b));
        assert(ft_memcpy(a, bytes, n) == a);
        memcpy(b, bytes, n);
        assert(memcmp(a, b, sizeof(a)) == 0);
        ft_bzero(a, n);
        bzero(b, n);
        assert(memcmp(a, b, sizeof(a)) == 0);
    }
    /* No terminator within n; zero length must not read one-past-end. */
    one = malloc(1);
    two = malloc(1);
    assert(one && two);
    *one = *two = 'a';
    assert(ft_strncmp(one, two, 1) == 0);
    assert(ft_strncmp(one + 1, two + 1, 0) == 0);
    assert(ft_memchr(one + 1, 'a', 0) == NULL);
    assert(ft_strlcat(one + 1, "abc", 0) == 3);
    assert(ft_strlcpy(one + 1, "abc", 0) == 3);
    free(one);
    free(two);
}

int main(void)
{
    test_memmove();
    test_strlcat();
    test_strlcpy();
    test_other_functions();
    puts("PASS: 18 functions, overlap matrix, empty strings, byte values and boundaries");
    return 0;
}
