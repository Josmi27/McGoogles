# McGoogles
Users of this McGoogles implementation can safely access and modify the restaurant's components, such as the number of customers, number of cooks, the maximum capacity of the restaurant, and more! 

One file (Main.c) handles all of the threading code (to handle the accessible, modifiable objects), and another file (McGoogles.c) that handles all of the locking and synchronization. The locking and synchronization file ensures that all variables are modified correctly and safely, and that the restaurant is never above capacity. It also guarantees that all orders were fulfilled (number of orders == number of expected orders). 


The output of this McGoogles implementation will print:
* How many orders each cook fulfilled
* If the restaurant fulfilled the expected number of orders
* What order number each customer received 
