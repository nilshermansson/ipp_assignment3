#include <iostream>
#include <chrono>
#include <omp.h>

int **a;
int **b;
int **c;

void allocate_matrices(const int dimensions)
{
    a = new int*[dimensions];
    b = new int*[dimensions];
    c = new int*[dimensions];

    for(int i = 0; i < dimensions; i++)
    {
        a[i] = new int[dimensions];
        b[i] = new int[dimensions];
        c[i] = new int[dimensions];
    }
}

// initialize as identity matrices for easier verification of program correctness
void init_matrices(const int dimensions)
{
    for(int i = 0; i < dimensions; i++)
    {
        for(int j = 0; j < dimensions; j++)
        {
            c[i][j] = 0;
            if(i == j)
            {
                a[i][j] = 1;
                b[i][j] = 1;
            }
            else
            {
                a[i][j] = 0;
                b[i][j] = 0;
            }
        }
    }
}

void deallocate_matrices(const int dimensions)
{
    for(int i = 0; i < dimensions; i++)
    {
        delete a[i];
        delete b[i];
        delete c[i];
    }
    delete a;
    delete b;
    delete c;
}

void print_matrix(int **m, int dim)
{
    for(int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            std::cout << m[i][j] << " ";
        }
        std::cout << std::endl;
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

int main(int argc, char *argv[])
{
    int dim, nr_of_threads;

    if(argc != 2)
    {
        usage(argv[0]);
    }
    else
    {
        dim = atoi(argv[1]);
    }

    //setup the matrices
    allocate_matrices(dim);
    init_matrices(dim);

    auto start_time = std::chrono::high_resolution_clock::now();

    // NOTE: we set c[i][j] = 0 when initializing the other matrices
    #pragma omp parallel default(none) shared(a, b, c, dim, nr_of_threads)
    {
        #pragma omp single
        nr_of_threads = omp_get_num_threads();

        #pragma omp for collapse(2) schedule(static)
        for(int i = 0; i < dim; i++)
        {
            for(int j = 0; j < dim; j++)
            {
                for(int k = 0; k < dim; k++)
                {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
    }
    auto end_time = std::chrono::high_resolution_clock().now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);

    //print_matrix(c, dim);

    std::cout << "Execution time(s): " << elapsed.count() * 1e-9 << std::endl;
    std::cout << "Threads used: " << nr_of_threads << std::endl;

    deallocate_matrices(dim);

    return 0;
}