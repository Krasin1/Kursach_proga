all: main.c
	gcc main.c -o main -ggdb

clean:
	rm main massiv_t.txt massiv_Uvx.txt massiv_Uvix.txt
