#include "McGoogles.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>

bool IsEmpty(McGoogles* mcg);
bool IsFull(McGoogles* mcg);
void AddOrderToBack(Order **orders, Order *order);

MenuItem McGooglesMenu[] = { 
    "GoogMac", 
    "GoogDouble", 
    "GoogChicken", 
    "GoogQuarterPounder", 
    "GoogNuggets",
    "GoogCrispyChicken",
    "GoogHamburger",
    "GoogCheeseBurger",
    "GoogSmokeHouseBurger",
    "GoogFiletOFish",
};
int McGooglesMenuLength = 10;

MenuItem PickRandomMenuItem() {

    int random_item_number = rand() % 10;
    char* random_item = McGooglesMenu[random_item_number];
    return random_item;
}

McGoogles* OpenRestaurant(int max_size, int expected_num_orders) {
    McGoogles* mcg;
    mcg = (McGoogles*)malloc(sizeof(McGoogles));
    
    if(mcg != NULL){
        mcg -> orders = NULL;
        mcg -> current_size = 0;
        mcg -> max_size = max_size;
        mcg -> next_order_number = 0;
        mcg -> orders_handled = 0;
        mcg -> expected_num_orders = expected_num_orders;
        
        if((pthread_mutex_init(&mcg -> mutex, NULL)) != 0){
            printf("mutex init failed\n");
            return NULL;
        };
        
        if((pthread_cond_init(&mcg -> can_add_orders, NULL)) != 0){
            printf("add order init failed\n");
            return NULL;
        };
        
        if((pthread_cond_init(&mcg -> can_get_orders, NULL)) != 0){
            printf("get order init failed\n");
            return NULL;
        };
    }
    printf("Restaurant is open!\n");
    return mcg;
}

void CloseRestaurant(McGoogles* mcg) {
    if(mcg->orders != NULL){
        printf("There are still orders to fulfill!");
        return;
    }
    else if(mcg->orders_handled != mcg->expected_num_orders){
        printf("Keep going! We haven't reached our expected number of orders yet.");
        return;
    }
    else{
        pthread_mutex_destroy(&mcg->mutex);
        pthread_cond_destroy(&mcg->can_add_orders);
        pthread_cond_destroy(&mcg->can_get_orders);
        free(mcg);
        printf("Restaurant is closed!\n");
    }
}

int AddOrder(McGoogles* mcg, Order* order) {
    pthread_mutex_lock(&mcg->mutex);
 
    while(IsFull(mcg)){
        
        pthread_cond_wait(&mcg->can_add_orders, &mcg->mutex);
    }
    
    order->order_number = mcg->next_order_number;
    
    AddOrderToBack(&mcg->orders, order);
    
    mcg->next_order_number++;
    
    mcg->current_size++;
    
    pthread_cond_signal(&mcg->can_get_orders);
    
    pthread_mutex_unlock(&mcg->mutex);
 
    return order->order_number;
}

Order *GetOrder(McGoogles* mcg) {
    pthread_mutex_lock(&mcg->mutex);
 
    while(IsEmpty(mcg)){
        
        if(mcg->orders_handled == mcg->expected_num_orders){
            pthread_cond_broadcast(&mcg->can_get_orders);
            pthread_mutex_unlock(&mcg->mutex);
            return NULL;
        }
        
        pthread_cond_wait(&mcg->can_get_orders, &mcg->mutex);
        
    }
    
    assert(!IsEmpty(mcg));
    
    Order* temp = mcg->orders;
    
    mcg->orders = temp->next;
    
    mcg->orders_handled++;
    
    mcg->current_size--;
    
    pthread_cond_signal(&mcg->can_add_orders);
    
    pthread_mutex_unlock(&mcg->mutex);
    
    return temp;
}

// Optional helper functions (you can implement if you think they would be useful)
bool IsEmpty(McGoogles* mcg){
  if (mcg->current_size > 0){
      return false;
  }
  else{
      return true;
  }
}

bool IsFull(McGoogles* mcg) {
  if (mcg->current_size == mcg->max_size){
      return true;
  }
  else{
      return false;
  }
}

void AddOrderToBack(Order **orders, Order *order) {
    if (*orders == NULL){
         order->next = NULL;
         *orders = order;
    }
    
    else{
         Order* current_order = *orders;
         while(current_order->next != NULL){
             current_order = current_order->next;
         }
        
         current_order->next = order;
         current_order->next->next = NULL;
    }
}