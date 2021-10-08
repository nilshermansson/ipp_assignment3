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

gauss_row:
	g++ -std=c++11 -wall -g -fopenmp matrixmult3.cpp -o gauss_row

gauss_column_inner:
	g++ -std=c++11 -wall -g -fopenmp gauss_column.cpp -o gauss_column_inner

clean:
	$(RM) Game_Of_Life sieve matrixmult matrixmult2 matrixmult3

clean_test:
	rm result1.txt result2.txt result3.txt

test1: sieve
	for num in 100 1000 10000 100000; do \
		for t in 1 2 4 8 16; do \
			./sieve $$t $$num >> result1.txt; \
		done ; \
	done



test3: matrixmult matrixmult2 matrixmult3
	for num in 32 16 8 4 2 1; do \
		export OMP_NUM_THREADS=$$num ; \
		./matrixmult 1000 >> result3_1.txt ; \
		./matrixmult2 1000 >> result3_2.txt ; \
		./matrixmult3 1000 >> result3_3.txt ; \
		echo "done with $$num" ; \
	done

test4: gauss_column_inner
	for num in 16 8 4 2 1; do \
        export OMP_NUM_THREADS=$$num ; \
        export OMP_SCHEDULE=DYNAMIC ; \
        echo "Dynamic scheduling with $$num threads: \n" >> result4_column.txt ; \
        ./gauss_column_inner 42000 >> result4_column.txt ; \
        export OMP_SCHEDULE=STATIC ; \
        echo "Static scheduling with $$num threads: \n" >> result4_column.txt ; \
        ./gauss_column_inner 42000 >> result4_column.txt ; \
        export OMP_SCHEDULE=RUNTIME ; \
        echo "Runtime scheduling with $$num threads: \n" >> result4_column.txt ; \
        ./gauss_column_inner 42000 >> result4_column.txt ; \
        echo "Done testing with $$num threads" ; \
    done
