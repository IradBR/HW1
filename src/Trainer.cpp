#include "../include/Trainer.h"


Trainer::Trainer(int t_capacity) :capacity(t_capacity),open(false), salary(0), curr_capacity(t_capacity), wasOpen(
        false){}
void Trainer::setWasOpen(){
    wasOpen= true;
}
bool Trainer::getWasOpen(){
    return wasOpen;
}

void Trainer::addCustomer(Customer* customer) {
    customersList.push_back(customer);
    curr_capacity=curr_capacity-1; //the salary will update at the order function
    open=true;
}

void Trainer::removeCustomer(int id){
    int index=-1;
    for (size_t i=0; i<customersList.size() && index==-1; i++) {
            if (customersList[i]->getId() == id) {
                index = i;
                curr_capacity = curr_capacity + 1;
            }
    }
    if(index!=-1){   //remove from the order list
        customersList.erase(customersList.begin()+index);
        std::vector<OrderPair> tempOrderList;
        int curr_salary=0;
        for(size_t i=0; i<orderList.size(); i++){
            if(orderList[i].first!=id) {
                curr_salary = curr_salary + orderList[i].second.getPrice();
                tempOrderList.push_back(orderList[i]);
            }
        }
        orderList.clear();
        for(size_t i=0; i<tempOrderList.size();i++){
            orderList.push_back(tempOrderList[i]);
        }
        tempOrderList.clear();
        salary=curr_salary; //update the salary after removing orders
    }
}

Customer* Trainer::getCustomer(int id){
    int index=-1;
    for (size_t i=0; i<customersList.size() && index==-1; i++) {
            if (customersList[i]->getId() == id) {
                index = i;
            }
        }

    if(index==-1){
        return nullptr;
    }

    return customersList[index];
}

std::vector<Customer*>& Trainer::getCustomers(){
    return customersList;
}


std::vector<OrderPair>& Trainer::getOrders(){
    return orderList;

}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options) {
    for (size_t i = 0; i < workout_ids.size(); i++) {
        Workout toAdd=workout_options[workout_ids[i]];
        OrderPair pair(customer_id, toAdd);
        orderList.push_back(pair);
        salary=salary+toAdd.getPrice();
    }
}

void Trainer::openTrainer(){
    open=true;
}

void Trainer::closeTrainer(){
    curr_capacity=capacity;
    open=false;
    for(size_t i=0; i<customersList.size();i++){
        if(customersList[i]) delete customersList[i];
    }
    customersList.clear();
    orderList.clear();

}


bool Trainer::isOpen(){
    if (open)
        return open;
    return false;

}

int Trainer::getCapacity() const{
    return capacity;

}
int Trainer::getCurr_capacity()const{
    return curr_capacity;
}
int Trainer::getSalary() const{
    return salary;
}

//destructor
Trainer::~Trainer(){
    for (size_t i=0; i<customersList.size();i++){
        if (customersList[i]) delete customersList[i];
    }
    customersList.clear();
}



Trainer::Trainer(const Trainer &other){//copy constructor
    capacity=other.capacity;
    wasOpen=other.wasOpen;
    salary=other.salary;
    curr_capacity=other.curr_capacity;
    open=other.open;

    for(size_t i=0; i<other.customersList.size(); i++){
        Customer* toAdd=other.customersList[i]->clone();
        customersList.push_back(toAdd);
    }

    for(size_t i=0; i<other.orderList.size(); i++){
        int curr_id=other.orderList[i].first;
        Workout curr_workout= other.orderList[i].second;
        orderList.push_back(OrderPair(curr_id, curr_workout));
    }
}

Trainer::Trainer(Trainer && other){ //move constructor
    wasOpen=other.wasOpen;
    capacity=other.capacity;
    salary=other.salary;
    curr_capacity=other.curr_capacity;
    open=other.open;

    for(size_t i=0; i<other.customersList.size();i++){
        customersList.push_back(other.customersList[i]);
        other.customersList[i]= nullptr;
    }
    for (size_t i=0; i<other.orderList.size(); i++){
        int curr_id=other.orderList[i].first;
        Workout curr_workout= other.orderList[i].second;
        orderList.push_back(OrderPair(curr_id, curr_workout));
    }
}
Trainer& Trainer::operator= (const Trainer &other) {//copy assignment operator
    if(this!=&other) {
        wasOpen=other.wasOpen;
        capacity=other.capacity;
        salary=other.salary;
        curr_capacity=other.curr_capacity;
        open=other.open;

        for(size_t i=0; i<customersList.size(); i++){
            if(customersList[i]) delete customersList[i];
        }
        customersList.clear();
        orderList.clear();

        for (size_t i=0; i<other.customersList.size(); i++){
            customersList.push_back(other.customersList[i]);
        }
        for (size_t i=0; i<other.orderList.size(); i++){
            orderList.push_back(other.orderList[i]);
        }
    }
    return *this;
}

Trainer& Trainer::operator= (Trainer &&other) { //move assignment operator
    if (this!=&other) {
        wasOpen=other.wasOpen;
        capacity=other.capacity;
        salary=other.salary;
        curr_capacity=other.curr_capacity;
        open=other.open;
        customersList.clear();
        for(size_t i=0; i<customersList.size(); i++){
            if(customersList[i]) delete customersList[i];
        }

        for(size_t i=0; i<other.customersList.size(); i++){
            customersList.push_back(other.customersList[i]);
            other.customersList[i]= nullptr;
        }
        orderList.clear();
        for(size_t i=0; i<other.orderList.size();i++){
            orderList.push_back(other.orderList[i]);
        }

        other.customersList.clear();
        other.orderList.clear();
    }
    return *this;
}