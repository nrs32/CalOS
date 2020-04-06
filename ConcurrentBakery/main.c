
#include <stdio.h>
#include <pthread.h>
#include <stdint.h> // For intptr_t

/* This is a customer thread */
void *customer(void *arg)
{
    fprintf(stderr, "Customer %ld\n", (intptr_t) arg);
	return NULL;
}

/* This thread is responsible for baking */
void *baker(void *arg)
{
    fprintf(stderr, "Baking...\n");
	return NULL;
}

/* This thread is responsible for cashiering */
void *cashier(void *arg)
{
    fprintf(stderr, "Cashiering...\n");
	return NULL;
}

int main()
{
    fprintf(stderr, "Time for the Bakery to open!\n"); 

    // TODO: Create the necessary semaphores -- stored in global variables.

	// Create baker
	pthread_t bakerThread;
    pthread_create(&bakerThread, 0, baker, NULL);

	pthread_t cashierThread;
    pthread_create(&cashierThread, 0, cashier, NULL);

    // Create customers
    pthread_t ts[10];

   	for (int i = 0; i < 10; i++)
		pthread_create(&ts[i], NULL, customer, (void *) (intptr_t) i + 1);
	

	// TODO: we don't need both join and exit

	/* Wait for all customers to exit before continuing in main */
	for (int i = 0; i < 10; i++)
		pthread_join(ts[i], NULL);

    // Wait for last thread to terminate and then exit main
    pthread_exit(NULL);

    return 0;
}