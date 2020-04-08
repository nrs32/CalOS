/**
 * Use threads to contol customer and baker threads.
 * Using semaphores we can control 
 * 		- how many customers are in the store
 * 		- customer - baker interactions
 * 		- and the baker's work
 * 
 * @author Nikita Sietsema
 * @date 4 April, 2020
 */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h> // For srand
#include <time.h>   // For time
#include <string.h> // For strcmp
#include <stdint.h> // For intptr_t
#include <fcntl.h>  // For O_CREAT
#include <unistd.h> // For usleep

// Bread we have
#define NUM_BREADS 6
char* breads[NUM_BREADS] = { "Rye", "Sourdough", "Pumpkin", "French", "Zucchini", "Wheat"};

// Global semaphores
#define REMAINING_ROOM_FOR_CUSTOMERS "/REMAINING_ROOM_FOR_CUSTOMERS"
#define BUSY_MUTEX "/BUSY_MUTEX"
#define LOAF_MUTEX "/LOAF_MUTEX"
#define MONEY_MUTEX "/MONEY_MUTEX"
#define RECIEPT_MUTEX "/RECIEPT_MUTEX"
#define REGISTER_MUTEX "/REGISTER_MUTEX"
#define CUSTOMER_READY_MUTEX "/CUSTOMER_READY_MUTEX"
sem_t *remaining_room_for_customers, *busy_mutex, 
	  *loaf_mutex, *money_mutex, *reciept_mutex, 
	  *register_mutex, *customer_ready_mutex;
int num_loaves_on_shelf = 0; // protected by busy_mutex

///////////////////////////////
/* This is a customer thread */
void *customer(void *arg)
{
    // Wait for room in the bakery so I can enter
	sem_wait(remaining_room_for_customers);
    fprintf(stderr, "Customer %ld: *enters bakery\n", (intptr_t) arg);
	
	// Wait to get to the register
	sem_wait(register_mutex);

	// Tell cashier we are ready
	sem_post(customer_ready_mutex);

	// Wait for cashier to give us bread
	sem_wait(loaf_mutex);
    fprintf(stderr, "Customer %ld: \"Thanks for the bread!\"\n", (intptr_t) arg);

	// Get money and give to cashier
	usleep(1000000);
	fprintf(stderr, "Customer %ld: *hands over money\n", (intptr_t) arg);
	sem_post(money_mutex);

	// Wait for cashier to give us reciept
	sem_wait(reciept_mutex);
	fprintf(stderr, "Customer %ld: \"Thanks for my receipt!\"\n", (intptr_t) arg);

	// Done at register
	sem_post(register_mutex);
	fprintf(stderr, "Customer %ld: *leaves\n", (intptr_t) arg);
	usleep(1000000);

	// Once customer exits the bakery
	// Notify those waiting outside that there is room for someone to enter
	sem_post(remaining_room_for_customers);

	return NULL;
}
///////////////////////////////////////////
/* This thread is responsible for baking */
void *baker(void *arg)
{
	srand(time(0)); // Seed rand number generator

	// We only bake 10 loaves of bread a day
	for (int i = 0; i < 10; i++) {
		// Obtain mutex for busy (we are not with a customer)
		sem_wait(busy_mutex);

		// Bake the bread
		usleep(1000000);

		// Add to loaf to bread shelf (protected by busy semiphore)
		num_loaves_on_shelf++;

		fprintf(stderr, "Baker: *places fresh %s bread on shelf\n", breads[rand() % (NUM_BREADS - 1)]);

		// Finished all baking
		if (i == 9) { 
			fprintf(stderr, "Baker: \"Finished baking for today! :)\"\n"); 
		}

		// Release semaphore for busy (we are not baking)
		sem_post(busy_mutex);

		// Delay between baking each loaf of bread
		usleep(1000000);
	}

	return NULL;
}

///////////////////////////////////////////////
/* This thread is responsible for cashiering */
void *cashier(void *arg)
{
	// Since only bake 10 loaves of bread a day
	// We can only serve 10 customers
	for (int i = 0; i < 10; i++) {

		// Spin lock. Before we can be busy cashiering
		// we must have loaves to give customer
		while (num_loaves_on_shelf == 0);

		// Wait for customer to be ready
		sem_wait(customer_ready_mutex);

		// Obtain semaphore for busy (we are not baking)
		sem_wait(busy_mutex);

		// Remove loaf from bread shelf (protected by busy semiphore)
		num_loaves_on_shelf--;
		fprintf(stderr, "Baker (at register): \"Here is your loaf!\"\n");
		sem_post(loaf_mutex);

		// Wait for customer to pay and process payment
		sem_wait(money_mutex);
		fprintf(stderr, "Baker (at register): *takes payment\n");
		usleep(1000000);

		// Give customer reciept
		fprintf(stderr, "Baker (at register): \"Let me get your reciept...\"\n");
		usleep(1000000);
		fprintf(stderr, "Baker (at register): *hands customer reciept\n");
		sem_post(reciept_mutex);

		// Release semaphore for busy (we are not with a customer)
		sem_post(busy_mutex);
	}

	return NULL;
}

//////////////////////////////////
/* Create semapores and threads. /
 * Open and close bakery.       */
int main()
{
    fprintf(stderr, "Time for the Bakery to open!\n"); 

	// Counting Semaphore ensuring a max of 5 customers in bakery at a time
	remaining_room_for_customers = sem_open(REMAINING_ROOM_FOR_CUSTOMERS, O_CREAT, NULL, 5);
	if (remaining_room_for_customers == NULL) {
		perror("Creating remaining_room_for_customers semaphore failed"); 
	}

	// Counting Mutex semaphores
	busy_mutex = sem_open(BUSY_MUTEX, O_CREAT, NULL, 1);
	loaf_mutex = sem_open(LOAF_MUTEX, O_CREAT, NULL, 0);
	money_mutex = sem_open(MONEY_MUTEX, O_CREAT, NULL, 0);
	reciept_mutex = sem_open(RECIEPT_MUTEX, O_CREAT, NULL, 0);
	register_mutex = sem_open(REGISTER_MUTEX, O_CREAT, NULL, 1);
	customer_ready_mutex = sem_open(CUSTOMER_READY_MUTEX, O_CREAT, NULL, 0);
	if (busy_mutex == NULL || loaf_mutex == NULL || money_mutex == NULL || reciept_mutex == NULL || register_mutex == NULL || customer_ready_mutex == NULL) {
		perror("Creating mutex semaphores failed"); 
	}
	
	// Create baker
	// We always start baking first
	pthread_t bakerThread;
    pthread_create(&bakerThread, 0, baker, NULL);

	pthread_t cashierThread;
    pthread_create(&cashierThread, 0, cashier, NULL);
	
    // Create customers
    pthread_t ts[10];

   	for (int i = 0; i < 10; i++)
		pthread_create(&ts[i], NULL, customer, (void *) (intptr_t) i);

	// Wait for all threads to exit before continuing in main
	for (int i = 0; i < 10; i++)
		pthread_join(ts[i], NULL);

	pthread_join(cashierThread, NULL);
	pthread_join(bakerThread, NULL);

    fprintf(stderr, "Bakery closed.\n"); 

	// Unlink the semaphores we used
	sem_unlink(REMAINING_ROOM_FOR_CUSTOMERS);
	sem_unlink(BUSY_MUTEX);
	sem_unlink(LOAF_MUTEX);
	sem_unlink(MONEY_MUTEX);
	sem_unlink(RECIEPT_MUTEX);
	sem_unlink(REGISTER_MUTEX);
	sem_unlink(CUSTOMER_READY_MUTEX);

    // Wait for last thread to terminate and then exit main
	// *Instead of pthread_exit() we use pthread_join so we can unlink semaphores
    pthread_exit(NULL);

    return 0;
}