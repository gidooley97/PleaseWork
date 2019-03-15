#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>

// Returns a random value between -1 and 1
double getRand(unsigned int *seed) {
    return (double) rand_r(seed) * 2 / (double) (RAND_MAX) - 1;
}

long double Calculate_Pi_Sequential(long long number_of_tosses) {
    unsigned int seed = (unsigned int) time(NULL);
    double number_in_circle = 0;

    for (int toss = 0; toss < number_of_tosses; toss++) {
        double x = getRand(&seed);
        double y = getRand(&seed);
        double distance_squared = x * x + y * y;
        if (distance_squared <= 1) {
            number_in_circle++;
        }
    } // End of for
    double pi_estimate = 0;
    pi_estimate = 4 * number_in_circle / ((double) number_of_tosses);
    return pi_estimate;
}


long double Calculate_Pi_Parallel(long long number_of_tosses) {
    unsigned int seed = (unsigned int) time(NULL);
    double number_in_circle = 0;
    //pragma statement modifies for loop
#pragma omp parallel for reduction(+:number_in_circle)
        for (int toss = 0; toss < number_of_tosses; toss++) {
            double x = getRand(&seed);
            double y = getRand(&seed);
            double distance_squared = x * x + y * y;
            if (distance_squared <= 1) {
                number_in_circle++;
            }
        } // End of for

double pi_estimate = 0;
pi_estimate = 4 * number_in_circle / ((double) number_of_tosses);
return pi_estimate;

}


int main() {
    struct timeval start, end;

    long long num_tosses = 10000000;
    printf("Timing sequential...\n");
    gettimeofday(&start, NULL);

    long double sequential_pi = Calculate_Pi_Sequential(num_tosses);
    gettimeofday(&end, NULL);
    printf("Took %f seconds\n\n", end.tv_sec - start.tv_sec + (double) (end.tv_usec - start.tv_usec) / 1000000);

    printf("Timing parallel...\n");
    gettimeofday(&start, NULL);
    long double parallel_pi = Calculate_Pi_Parallel(num_tosses);
    gettimeofday(&end, NULL);
    printf("Took %f seconds\n\n", end.tv_sec - start.tv_sec + (double) (end.tv_usec - start.tv_usec) / 1000000);

    // This will print the result to 10 decimal places
    printf("π = %.10Lf (sequential)\n", sequential_pi);
    printf("π = %.10Lf (parallel)", parallel_pi);

    return 0;
}
