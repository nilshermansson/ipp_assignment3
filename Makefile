EX1=sieve
EX2=Game_Of_Life
EX3=matrixmult


all: Game_Of_Life sieve matrixmult matrixmult2 matrixmult3

sieve:
	g++ -std=c++11 -Wall -g -fopenmp sieve.cpp -o sieve

Game_Of_Life:
	gcc -std=c11 -Wall -g -fopenmp Game_Of_Life.c -o Game_Of_Life

matrixmult:
	g++ -std=c++11 -Wall -g -fopenmp matrixmult.cpp -o matrixmult

matrixmult2:
	g++ -std=c++11 -Wall -g -fopenmp matrixmult2.cpp -o matrixmult2

matrixmult3:
	g++ -std=c++11 -Wall -g -fopenmp matrixmult3.cpp -o matrixmult3

clean:
	$(RM) Game_Of_Life sieve matrixmult matrixmult2 matrixmult3

clean_test:
	rm result1.txt result2.txt result3.txt

test: matrixmult matrixmult2 matrixmult3
	for num in 32 16 8 4 2 1; do \
		export OMP_NUM_THREADS=$$num ; \
		./matrixmult 1000 >> result1.txt ; \
		./matrixmult2 1000 >> result2.txt ; \
		./matrixmult3 1000 >> result3.txt ; \
	done
