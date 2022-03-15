#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    Trainer(int t_capacity);
    int getCapacity() const;
    int getCurr_capacity()const;
    int getSalary() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    bool isOpen();
    virtual  ~Trainer(); //destructor
    Trainer(const Trainer &other); //copy constructor
    Trainer(Trainer && other);
    Trainer& operator= (const Trainer &other); //copy assignment operator;
    Trainer& operator= (Trainer &&other); //move assignment operator
    void setWasOpen();
    bool getWasOpen();

private:

    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
    int salary;
    int curr_capacity;
    bool wasOpen;

};


#endif