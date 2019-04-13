#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "McGoogles.h"

// Feel free to play with these numbers! This is a great way to
// test your implementation.
#define MCGOOGLES_SIZE 100
#define NUM_CUSTOMERS 3
#define NUM_COOKS 3
#define ORDERS_PER_CUSTOMER 10
#define EXPECTED_NUM_ORDERS NUM_CUSTOMERS * ORDERS_PER_CUSTOMER

// Global variable for the restaurant.
McGoogles *mcg;

/**
 * Thread funtion that represents a customer. A customer should:
 *  - (1) allocate space (memory) for an order.

 *  - (2) select a menu item.
 * 
 *  - (3) populate the order with their menu item and their customer ID.
 * 
 *  - (4) add their order to the restaurant.
 */
void* McGooglesCustomer(void* tid) {
    
    for(int i = 0; i<ORDERS_PER_CUSTOMER; i++){
        
        int customer_id = (int)(long)tid;
        
//      (1)
//      I am casting cust_order to be an Order pointer, because that is the type
//      of the variable that is allocating space for the order
        Order* order_allocated_space = malloc(sizeof(order_allocated_space));
        
//      (2)
        // Uses helper funcion to select a menu item
        order_allocated_space->menu_item = PickRandomMenuItem();  
        
//      (3) populates the order with their menu item and their customer ID.
        order_allocated_space->customer_id = customer_id;
        
//      (4) add their order to the restaurant.
        int order_num = AddOrder(mcg, order_allocated_space);
    }
    
    return NULL;
}

/**
 * Thread function that represents a cook in the restaurant. A cook should:
 *  - (1) get an order from the restaurant.
 *  - (2) if the order is valid, it should fulfill the order, and then
 *    free the space taken by the order.
 * The cook should take orders from the restaurants until it does not
 * receive an order.
 */

void* McGooglesCook(void* tid) {
    int cook_id = (int)(long) tid;
	int orders_fulfilled = 0;
    
//  (1)
    Order* new_order = GetOrder(mcg);
 
//  (2)
//  validates order
    while(new_order != NULL){ 
        orders_fulfilled++;
        free(new_order);
        new_order = GetOrder(mcg);
   }
	printf("Cook #%d fulfilled %d orders\n", cook_id, orders_fulfilled);
	return NULL;
}
    
  

/**
 * Runs when the program begins executing. This program should:
 *  - (1) open the restaurant
 *  - (2) create customers and cooks
 *  - (3) wait for all customers and cooks to be done
 *  - (4) close the restaurant.
 */
int main() {

    // initiates thread types for customers and cooks
    srand(time(0));
    pthread_t McgCustomers[NUM_CUSTOMERS];
    pthread_t McgCooks[NUM_COOKS];
    int i;
    int j;

    
    // (1)
    // opens restauraunt with desired size and expected order numbers
    mcg = OpenRestaurant(MCGOOGLES_SIZE, EXPECTED_NUM_ORDERS);
    
    // (2)                             
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_create(&(McgCustomers[i]), NULL, McGooglesCustomer, NULL);
    }

    for (int j = 0; j < NUM_COOKS; j++) {
        pthread_create(&McgCooks[j], NULL, McGooglesCook, NULL);
    }

    // (3)
    // Wait for all customers and cooks to be done.   
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(McgCustomers[i], NULL);
    }
    for (int j = 0; j < NUM_COOKS; j++) {
        pthread_join(McgCooks[j], NULL);
    }
    
    
    // (4)
    CloseRestaurant(mcg);  
    return 0;
}
