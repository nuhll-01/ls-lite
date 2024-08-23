minils: main.c minils.c
	gcc main.c minils.c -o minils -std=c99 -Wall

clean:
	rm -f minils