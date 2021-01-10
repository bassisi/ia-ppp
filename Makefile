build:
	g++ main.cpp -Wall -o sa

run1:
	./sa 1 ./Configuraciones/config1.txt

run2:
	./sa 2 ./Configuraciones/config2.txt	

run3:
	./sa 3 ./Configuraciones/config3.txt	

run4:
	./sa 4 ./Configuraciones/config4.txt

run5:
	./sa 5 ./Configuraciones/config5.txt	

run6:
	./sa 6 ./Configuraciones/config6.txt	

clean: 
	rm -rf sa
	rm -rf ./Resultados/*
