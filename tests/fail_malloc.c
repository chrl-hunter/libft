#include "libft.h"
#include <assert.h>
#include <stdio.h>

void *__real_malloc(size_t size);
void __real_free(void *ptr);
static size_t live;
static size_t calls;
static size_t fail_at;

void *__wrap_malloc(size_t size)
{
	calls++;
	if (fail_at && calls == fail_at) return (NULL);
	void *p = __real_malloc(size);
	if (p) live++;
	return (p);
}

void __wrap_free(void *ptr)
{
	if (ptr) { assert(live > 0); live--; }
	__real_free(ptr);
}

static char identity(unsigned int i, char c) { (void)i; return (c); }
static void *copy(void *p)
{
	int *r = malloc(sizeof(*r));
	if (r) *r = *(int *)p;
	return (r);
}

int main(void)
{
	char **parts;
	t_list *mapped;
	int v = 7;
	t_list third = {&v, NULL}, second = {&v, &third}, first = {&v, &second};
	size_t i;
	for (i = 1; i <= 4; i++)
	{
		calls = 0; fail_at = i;
		parts = ft_split("one two three", ' ');
		assert(parts == NULL);
		assert(live == 0);
	}
	/* f() succeeds, then the node allocation fails at each position. */
	for (i = 2; i <= 6; i += 2)
	{
		calls = 0; fail_at = i;
		mapped = ft_lstmap(&first, copy, free);
		assert(mapped == NULL);
		assert(live == 0);
	}
	fail_at = 1;
	calls = 0; assert(ft_calloc(4, 4) == NULL);
	calls = 0; assert(ft_strdup("abc") == NULL);
	calls = 0; assert(ft_substr("abc", 0, 2) == NULL);
	calls = 0; assert(ft_strjoin("a", "b") == NULL);
	calls = 0; assert(ft_strtrim(" a ", " ") == NULL);
	calls = 0; assert(ft_strmapi("abc", identity) == NULL);
	calls = 0; assert(ft_itoa(-42) == NULL);
	calls = 0; assert(ft_lstnew(&v) == NULL);
	assert(live == 0);
	fail_at = 0;
	puts("PASS: 15 injected allocation failures");
	return (0);
}
