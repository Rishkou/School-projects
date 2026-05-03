#compiler
CC = gcc

#flags
CFLAGS = -std=gnu99 -Wall -Wextra -Werror -pedantic
#libraries
LDFLAGS = -pthread -lrt

TARGET = proj2
SRCS = proj2.c 
OBJS = $(SRCS:.c=.o)
# linking
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
#universal rule for compiling
%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@ 

#clean
clean:
	rm -f $(TARGET) $(OBJS) proj2.out

.PHONY: clean