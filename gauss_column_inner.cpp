#include <iostream>
#include <chrono>
#include <omp.h>
#include <cstring>
#include <cstdlib>
#include <chrono>
#include <string>
#include <atomic>
#include <time.h>
#include <math.h>

const int MIN_ELEMENT_VALUE = 1;
const int MAX_ELEMENT_VALUE = 1000;
const int RANDOM_SEED = 42;


void deallocate_matrix(int **m, const int dim)
{
    for(int i = 0; i < dim; i++)
    {
        delete m[i];
    }
    delete m;
}

int** init_matrix(int dim)
{

    int **m = new int*[dim];
    //allocate
    for(int i = 0; i < dim; i++)
    {
        m[i] = new int[dim];
    }

    //populate
    for(int i = 0; i < dim; i++)
    {
        for(int j = 0; j < dim; j++)
        {
            m[i][j] = rand() % MAX_ELEMENT_VALUE + MIN_ELEMENT_VALUE;
        }
    }
    return m;
}

int* init_vector(int dim)
{
    int *v = new int[dim];
	for (int i = 0; i < dim; i++) 
    {
        v[i] = rand() % MAX_ELEMENT_VALUE + MIN_ELEMENT_VALUE;
    }
    return v;
}

void print_matrix(int **m, int dim)
{
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0;  j < dim;  j++)
        {
            std::cout << m[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void print_vector(int *v, int dim)
{
    for(int i = 0; i < dim; i++)
    {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
}

// column-oriented
// code from exercise 4
void gauss_elim(int** A, int *b, int *x, int n) 
{
    int row, col; 
    for(row = 0; row < n; row++) 
        x[row] = b[row];
    for(col = n-1; col >= 0; col--) 
    {
        x[col] /= A[col][col];
        #pragma omp parallel for private(row)
        for (row = 0; row < col; row++) 
        {
            x[row] -= A[row][col] * x[col]; 
        }
	}
}

void usage(char *program_name)
{
    std::cout << "Usage:" << std::endl;
    std::cout << program_name << " <D>" << std::endl;
    std::cout << "where D is the dimensions of the (square) matrix." << std::endl;
    std::cout << "Use the OMP_NUM_THREADS environment variable to set the number of threads to use." << std::endl;
    exit(1);
}

int main(int argc, char *argv[]) {
    
    int dim;
    bool should_print = false;
    srand(RANDOM_SEED);
    
    if (argc != 2 && argc != 3) 
    {
		usage(argv[0]);
		exit(-1);
    }
    else if(argc == 3 && strcmp(argv[2], "-p") == 0)
    {
        should_print = true;
        dim = atoi(argv[1]); 
    }
    else 
    {
        dim = atoi(argv[1]); 
    }

    int **A; // A matrix
    int *b; //right-hand side
    int *x; //solution vector
    int *x_ref;

    A = init_matrix(dim); // initialize random square matrix
    b = init_vector(dim); // initialize random right-hand side vector
    x = new int[dim]; // solution vector
    x_ref = new int[dim];

    auto start_time = std::chrono::high_resolution_clock::now();

    gauss_elim(A, b, x, dim);

    auto end_time = std::chrono::high_resolution_clock().now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    std::cout << "Execution time(s): " << elapsed.count() * 1e-9 << std::endl;

    //for reference
    omp_set_num_threads(1);
    gauss_elim(A, b, x_ref, dim);

    for(int i = 0; i < dim; i++)
    {
        if(x_ref[i] != x[i])
        {
            std::cout << "THERE WAS AN ERROR" << std::endl;
            exit(-1);
        }
    }

    if (should_print)
    {
        print_vector(x, dim);
    }

    deallocate_matrix(A, dim);
    delete b;
    delete x;

    return 0;
}