#include "libft.h"
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

static size_t checks;
#define CHECK(x) do { assert(x); checks++; } while (0)

static int signum(int n) { return ((n > 0) - (n < 0)); }
static char map_char(unsigned int i, char c) { return (c + i); }
static void iter_char(unsigned int i, char *c) { *c += i; }
static void inc(void *p) { (*(int *)p)++; }
static void *dup_int(void *p)
{
	int *r = malloc(sizeof(*r));
	assert(r);
	*r = *(int *)p * 2;
	return (r);
}

static void test_chars(void)
{
	int c;
	for (c = -1; c <= 255; c++)
	{
		CHECK(!!ft_isalpha(c) == !!isalpha(c));
		CHECK(!!ft_isdigit(c) == !!isdigit(c));
		CHECK(!!ft_isalnum(c) == !!isalnum(c));
		CHECK(ft_isascii(c) == (c >= 0 && c <= 127));
		CHECK(!!ft_isprint(c) == !!isprint(c));
		CHECK(ft_toupper(c) == toupper(c));
		CHECK(ft_tolower(c) == tolower(c));
	}
}

static void test_memory(void)
{
	unsigned char a[48], b[48], src[48];
	size_t d, s, n, i;
	for (i = 0; i < sizeof(src); i++) src[i] = (unsigned char)(i * 7);
	for (d = 0; d < 12; d++)
		for (s = 0; s < 12; s++)
			for (n = 0; n <= 24; n++)
			{
				memcpy(a, src, sizeof(a)); memcpy(b, src, sizeof(b));
				CHECK(ft_memmove(a + d, a + s, n) == a + d);
				memmove(b + d, b + s, n);
				CHECK(memcmp(a, b, sizeof(a)) == 0);
			}
	for (n = 0; n <= sizeof(a); n++)
	{
		memset(a, 0x5a, sizeof(a)); memset(b, 0x5a, sizeof(b));
		CHECK(ft_memset(a, 0x180, n) == a);
		memset(b, 0x180, n); CHECK(memcmp(a, b, sizeof(a)) == 0);
		ft_bzero(a, n); memset(b, 0, n); CHECK(memcmp(a, b, sizeof(a)) == 0);
		CHECK(ft_memcpy(a, src, n) == a); memcpy(b, src, n);
		CHECK(memcmp(a, b, sizeof(a)) == 0);
		CHECK(signum(ft_memcmp(a, src, n)) == signum(memcmp(a, src, n)));
		for (i = 0; i < 256; i++)
			CHECK(ft_memchr(src, (int)i, n) == memchr(src, (int)i, n));
	}
	CHECK(ft_memcmp("\xff", "\x01", 1) > 0);
	CHECK(ft_memcmp("a", "b", 1) < 0);
	CHECK(ft_memmove(a, src, sizeof(a)) == a);
	CHECK(memcmp(a, src, sizeof(a)) == 0);
}

static void test_strings(void)
{
	const char *overlap = "aaab", *abc = "abc";
	const char *strings[] = {"", "a", "abc", "abcxyz", "\x80\xff"};
	size_t i, j, n;
	int c;
	char a[16], expected[16], raw[3] = {'a', 'b', 'c'};
	for (i = 0; i < sizeof(strings) / sizeof(*strings); i++)
	{
		CHECK(ft_strlen(strings[i]) == strlen(strings[i]));
		for (c = -256; c <= 511; c++)
		{
			CHECK(ft_strchr(strings[i], c) == strchr(strings[i], c));
			CHECK(ft_strrchr(strings[i], c) == strrchr(strings[i], c));
		}
		for (j = 0; j < sizeof(strings) / sizeof(*strings); j++)
			for (n = 0; n < 10; n++)
				CHECK(signum(ft_strncmp(strings[i], strings[j], n)) ==
					signum(strncmp(strings[i], strings[j], n)));
		for (n = 0; n < 10; n++)
		{
			memset(a, '!', sizeof(a)); memset(expected, '!', sizeof(expected));
			if (n) { size_t k = strlen(strings[i]); if (k >= n) k = n - 1;
				memcpy(expected, strings[i], k); expected[k] = 0; }
			CHECK(ft_strlcpy(a, strings[i], n) == strlen(strings[i]));
			CHECK(memcmp(a, expected, sizeof(a)) == 0);
			memset(a, '!', sizeof(a)); memcpy(a, "ab", 3);
			memcpy(expected, a, sizeof(a));
			if (n > 2) { size_t k = strlen(strings[i]); if (k > n - 3) k = n - 3;
				memcpy(expected + 2, strings[i], k); expected[2 + k] = 0; }
			CHECK(ft_strlcat(a, strings[i], n) == (n < 2 ? n : 2) + strlen(strings[i]));
			CHECK(memcmp(a, expected, sizeof(a)) == 0);
		}
	}
	CHECK(ft_strlcat(raw, "xyz", sizeof(raw)) == 6);
	CHECK(memcmp(raw, "abc", 3) == 0);
	CHECK(ft_strlcat(NULL, "abc", 0) == 3);
	CHECK(ft_strlcpy(NULL, "abc", 0) == 3);
	CHECK(ft_strncmp(NULL, NULL, 0) == 0);
	CHECK(ft_strnstr("abxby", "aby", 5) == NULL);
	CHECK(ft_strnstr(overlap, "aab", 4) == overlap + 1);
	CHECK(ft_strnstr("abc", "bc", 2) == NULL);
	CHECK(ft_strnstr(abc, "", 0) == abc);
	CHECK(ft_strnstr(NULL, "a", 0) == NULL);
	CHECK(ft_strnstr(raw, "z", sizeof(raw)) == NULL);
}

static void check_owned(char *p, const char *expected)
{
	CHECK(p != NULL); CHECK(strcmp(p, expected) == 0); free(p);
}

static void check_split(const char *s, char c, const char **words, size_t count)
{
	char **p = ft_split(s, c);
	size_t i;
	CHECK(p != NULL);
	for (i = 0; i < count; i++) { CHECK(p[i] != NULL); CHECK(strcmp(p[i], words[i]) == 0); free(p[i]); }
	CHECK(p[count] == NULL); free(p);
}

static void test_allocations(void)
{
	int values[] = {INT_MIN, INT_MIN + 1, -123456789, -10, -1, 0, 1, 9, 10, 100, INT_MAX};
	const char *numbers[] = {"", "0", " 42", "\t\n\v\f\r -42x", "--1", "+-1", "++1", "-+1", "-2147483648", "2147483647", "-0", " +12"};
	const char *words[] = {"hello", "world"};
	const char *whole[] = {"abc"};
	size_t i, j;
	char buf[64], input[] = "abc";
	unsigned char *p;
	for (i = 0; i < sizeof(numbers) / sizeof(*numbers); i++) CHECK(ft_atoi(numbers[i]) == atoi(numbers[i]));
	for (i = 0; i < sizeof(values) / sizeof(*values); i++)
	{ snprintf(buf, sizeof(buf), "%d", values[i]); check_owned(ft_itoa(values[i]), buf); CHECK(ft_atoi(buf) == values[i]); }
	for (i = 0; i < 10000; i++)
	{ int v = (int)(i * 21397) - 100000000; snprintf(buf, sizeof(buf), "%d", v); check_owned(ft_itoa(v), buf); }
	for (i = 0; i < 16; i++) for (j = 0; j < 16; j++)
	{ p = ft_calloc(i, j); CHECK(p != NULL); for (size_t k = 0; k < i * j; k++) CHECK(p[k] == 0); free(p); }
	CHECK(ft_calloc(SIZE_MAX, 2) == NULL);
	check_owned(ft_strdup(""), ""); check_owned(ft_strdup("abc"), "abc");
	check_owned(ft_substr("abc", 1, SIZE_MAX), "bc");
	check_owned(ft_substr("abc", UINT_MAX, 2), "");
	check_owned(ft_strjoin("abc", "def"), "abcdef");
	check_owned(ft_strjoin("", ""), "");
	check_owned(ft_strtrim("xy abxcy xy", "xy "), "abxc");
	check_owned(ft_strtrim("", "abc"), "");
	check_owned(ft_strtrim("xxx", "x"), "");
	check_owned(ft_strtrim("abc", ""), "abc");
	check_owned(ft_strmapi("abc", map_char), "ace");
	ft_striteri(input, iter_char); CHECK(strcmp(input, "ace") == 0);
	check_split("  hello  world  ", ' ', words, 2);
	check_split("abc", '\0', whole, 1); check_split("", '\0', NULL, 0);
	check_split("   ", ' ', NULL, 0); check_split("", ' ', NULL, 0);
}

static void test_lists(void)
{
	int a = 1, b = 2, c = 3;
	t_list *list = NULL, *mapped, *node;
	CHECK(ft_lstsize(NULL) == 0); CHECK(ft_lstlast(NULL) == NULL);
	ft_lstadd_back(&list, ft_lstnew(&b)); ft_lstadd_front(&list, ft_lstnew(&a));
	ft_lstadd_back(&list, ft_lstnew(&c)); CHECK(ft_lstsize(list) == 3);
	CHECK(ft_lstlast(list)->content == &c);
	ft_lstiter(list, inc); CHECK(a == 2 && b == 3 && c == 4);
	mapped = ft_lstmap(list, dup_int, free); CHECK(ft_lstsize(mapped) == 3);
	CHECK(*(int *)mapped->content == 4); CHECK(*(int *)ft_lstlast(mapped)->content == 8);
	ft_lstclear(&mapped, free); CHECK(mapped == NULL);
	while (list) { node = list->next; ft_lstdelone(list, NULL); list = node; }
}

static void test_output(void)
{
	int fds[2];
	char buf[64];
	const char *expected = "Xhelloend\n-2147483648,0,2147483647";
	ssize_t n;
	CHECK(pipe(fds) == 0);
	ft_putchar_fd('X', fds[1]); ft_putstr_fd("hello", fds[1]);
	ft_putendl_fd("end", fds[1]); ft_putnbr_fd(INT_MIN, fds[1]);
	ft_putchar_fd(',', fds[1]); ft_putnbr_fd(0, fds[1]);
	ft_putchar_fd(',', fds[1]); ft_putnbr_fd(INT_MAX, fds[1]);
	close(fds[1]); n = read(fds[0], buf, sizeof(buf)); close(fds[0]);
	CHECK(n == (ssize_t)strlen(expected)); CHECK(memcmp(buf, expected, (size_t)n) == 0);
}

int main(void)
{
	test_chars(); test_memory(); test_strings(); test_allocations(); test_lists(); test_output();
	printf("PASS: %zu checks, all 43 functions exercised\n", checks);
	return (0);
}
