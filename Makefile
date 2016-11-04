TARGET   =  program
SOURCES  =  $(wildcard *.c)
OBJECTS  =  $(patsubst %.c, %, $(SOURCES))
CC       =  gcc
CFLAGS   = -std=c99 -Wall -Wextra -g
INCLUDES = -lm -lglut -lGL -lGLU

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(INCLUDES)

.PHONY: clean

clean:
	rm -f $(OBJECTS) *.o
