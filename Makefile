build:
	g++ main.cpp -Wall -o sa

run:
	./sa

test:
	valgrind ./sa 

clean: 
	rm -rf sa