all:
	$(CC) src/main.c -o dining-philosophers -Iinclude -lpthread

clean:
	rm dining-philosophers

.PHONY: all clean
