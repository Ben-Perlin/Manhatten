all: mean_manhatten

mean_manhatten: mean_manhatten.c
	gcc -O2 -o mean_manhatten

.PHONY clean
clean:
	rm -f mean_manhatten
