all: mean_manhatten

mean_manhatten: mean_manhatten.c mean_manhatten.h databuffer.h buffered_input.h
	gcc -O2 mean_manhatten.c -o mean_manhatten

.PHONY: clean
clean:
	rm -f mean_manhatten
