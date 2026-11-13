CC = gcc
CFLAGS = -std=gnu11 -Wall -Wextra -Iinclude
LDFLAGS=
OBJS = http.o server.o mime.o files.o main.o
DEPS = http.h server.h mime.h files.h 
SRCDIR = src
INCDIR = include
TARGET = minihttpd
SRCDEPS = $(addprefix $(INCDIR)/,$(DEPS))

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(OBJS): %.o: $(SRCDIR)/%.c $(SRCDEPS)
	$(CC) -c -o $@ $(CFLAGS) $<

.PHONY: clean
clean:
	rm -rf $(OBJS) $(TARGET)