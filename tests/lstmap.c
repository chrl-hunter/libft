#include "libft.h"
#include <assert.h>
#include <stdio.h>

void *__real_malloc(size_t size);
void __real_free(void *ptr);
t_list *__real_ft_lstlast(t_list *lst);

static size_t live, calls, fail_at, mapped_calls, deleted, last_calls;

void *__wrap_malloc(size_t size)
{
	void *p;

	calls++;
	if (fail_at && calls == fail_at)
		return (NULL);
	p = __real_malloc(size);
	if (p)
		live++;
	return (p);
}

void __wrap_free(void *ptr)
{
	if (ptr)
	{
		assert(live > 0);
		live--;
	}
	__real_free(ptr);
}

t_list *__wrap_ft_lstlast(t_list *lst)
{
	last_calls++;
	return (__real_ft_lstlast(lst));
}

static void *transform(void *content)
{
	int *value;

	mapped_calls++;
	value = malloc(sizeof(*value));
	if (value)
		*value = *(int *)content + 1;
	return (value);
}

static void *borrow(void *content)
{
	mapped_calls++;
	return (content);
}

static void dispose(void *content)
{
	deleted++;
	free(content);
}

static void reset(size_t failure)
{
	assert(live == 0);
	calls = 0;
	fail_at = failure;
	mapped_calls = 0;
	deleted = 0;
	last_calls = 0;
}

static void verify_source(t_list *source, int *values, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++)
	{
		assert(source[i].content == &values[i]);
		assert(values[i] == (int)i);
		assert(source[i].next == (i + 1 < n ? &source[i + 1] : NULL));
	}
}

/* missing is 1-based; zero means all content allocations succeeded. */
static void verify_map(t_list *result, t_list *source, size_t n, size_t missing)
{
	size_t i;

	for (i = 0; i < n; i++)
	{
		assert(result && result != &source[i]);
		if (i + 1 == missing)
			assert(result->content == NULL);
		else
		{
			assert(result->content != source[i].content);
			assert(*(int *)result->content == (int)i + 1);
		}
		result = result->next;
	}
	assert(result == NULL);
}

static void exercise(size_t n, int inject)
{
	t_list *source, *result;
	int *values;
	size_t i, failure, limit;

	/* Fixture allocations bypass injection and the live-allocation counter. */
	source = __real_malloc(n * sizeof(*source));
	values = __real_malloc(n * sizeof(*values));
	assert(source && values);
	for (i = 0; i < n; i++)
	{
		values[i] = (int)i;
		source[i].content = &values[i];
		source[i].next = i + 1 < n ? &source[i + 1] : NULL;
	}
	limit = inject ? 2 * n : 0;
	for (failure = 0; failure <= limit; failure++)
	{
		reset(failure);
		result = ft_lstmap(source, transform, dispose);
		if (failure && failure % 2 == 0)
		{
			assert(result == NULL);
			assert(calls == failure);
			assert(mapped_calls == failure / 2);
			assert(deleted == failure / 2);
		}
		else
		{
			/* NULL from f is valid content, preserving the existing contract. */
			verify_map(result, source, n, (failure + 1) / 2);
			assert(mapped_calls == n && calls == 2 * n);
			ft_lstclear(&result, dispose);
			assert(result == NULL && deleted == n);
		}
		assert(live == 0);
		/* Deterministic regression guard against repeated tail searches. */
		assert(last_calls == 0);
		verify_source(source, values, n);
	}
	__real_free(source);
	__real_free(values);
}

int main(void)
{
	int value = 7;
	t_list source = {&value, NULL};
	t_list *result;

	reset(0);
	assert(ft_lstmap(NULL, transform, dispose) == NULL);
	assert(ft_lstmap(NULL, NULL, NULL) == NULL);
	assert(ft_lstmap(&source, NULL, dispose) == NULL);
	assert(calls == 0 && mapped_calls == 0 && deleted == 0);
	reset(0);
	result = ft_lstmap(&source, borrow, NULL);
	assert(result && result->content == &value && result->next == NULL);
	ft_lstclear(&result, NULL);
	assert(live == 0);
	reset(1);
	assert(ft_lstmap(&source, borrow, NULL) == NULL);
	assert(live == 0 && mapped_calls == 1);
	exercise(1, 1);
	exercise(64, 1);
	exercise(100000, 0);
	exercise(200000, 0);
	puts("PASS: lstmap empty/NULL callbacks, 1/64/100000/200000 nodes,");
	puts("      130 allocation failures, exact cleanup, unchanged input, no tail searches");
	return (0);
}
