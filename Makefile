CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror

SRCS = usp.c file.c
OBJS = $(SRCS:.c=.o)
EXEC = USPassignment

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
   


