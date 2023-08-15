SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)

build: $(OBJECTS)
	$(CC) $(SOURCES) -o soulck -Wall -Wextra -pedantic -std=c99
