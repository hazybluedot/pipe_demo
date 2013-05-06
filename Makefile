
all: main child

main: main.o
child: child.o

.PHONY: clean
clean:
	rm -f *.o
