compile:
	g++ main.c++ -Wall -o main

run:
	./main

valgrind:
	valgrind ./main 

clean: 
	rm -rf main