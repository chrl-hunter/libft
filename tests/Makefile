CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address,undefined -fno-omit-frame-pointer
SOURCES = $(wildcard ../ft_*.c)
BUILD = .build

check: $(BUILD)/regression $(BUILD)/fail_malloc
	./$(BUILD)/regression
	./$(BUILD)/fail_malloc

$(BUILD):
	mkdir -p $@

$(BUILD)/regression: regression.c $(SOURCES) ../libft.h | $(BUILD)
	$(CC) $(CFLAGS) -I.. regression.c $(SOURCES) -o $@

$(BUILD)/fail_malloc: fail_malloc.c $(SOURCES) ../libft.h | $(BUILD)
	$(CC) $(CFLAGS) -I.. fail_malloc.c $(SOURCES) -Wl,--wrap=malloc -Wl,--wrap=free -o $@

clean:
	rm -rf $(BUILD)

.PHONY: check clean
