all: main
main: main.o void.o
	gcc -o main main.o void.o
	
main.o: src/main.c
	gcc -c src/main.c -o main.o -I ./include



void.o: src/void.c
	gcc -c src/void.c -o void.o -I ./include

clean:
	rm -rf *.o main 

run: all
	./main

# - комментарий
# main - цель (таргет), то что хотим получ
# : main.o functions.o - зависимости, то что нам нужно, чтобы получить цель
# gcc ... - команда для получения цели
