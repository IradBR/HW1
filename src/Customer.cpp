#include "../include/Customer.h"
#include <bits/stdc++.h>


//customer
Customer::Customer(std::string c_name, int c_id):name(c_name), id(c_id), ordered(false){}

std::string Customer::getName() const{
    return name;
}
int Customer::getId() const{
    return id;
}
bool Customer::getOrdered() const{
    return ordered;
}
void Customer::setOrdered(){
    ordered=true;
}



//SweatyCustomer
SweatyCustomer::SweatyCustomer(std::string name, int id):Customer(name,id){}

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options){
    std::vector <int> swt;
    for(size_t i=0; i<workout_options.size(); i++){
        if( workout_options[i].getType()== CARDIO){
            int id= workout_options[i].getId();
            swt.push_back(id);
        }
    }
    setOrdered();
    return  swt;

}
Customer* SweatyCustomer::clone(){
    Customer* toAdd=new SweatyCustomer(getName(), getId());
    if(getOrdered()) toAdd->setOrdered();
    return toAdd;

}


std::string SweatyCustomer::toString() const{
    return getName()+",swt";
}

//CheapCustomer
CheapCustomer::CheapCustomer(std::string name, int id):Customer(name,id){}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options){
    int cheapest=workout_options[0].getPrice();
    int cheapest_id=workout_options[0].getId();
    std::vector<int> chp;
    for(size_t i=1; i<workout_options.size(); i++){
        if( workout_options[i].getPrice()<cheapest){
            cheapest=workout_options[i].getPrice();
            cheapest_id=workout_options[i].getId();
        }
    }
    chp.push_back(cheapest_id);
    setOrdered();
    return chp;
}

Customer* CheapCustomer::clone(){
    Customer* toAdd=new CheapCustomer(getName(), getId());
    if(getOrdered()) toAdd->setOrdered();
    return toAdd;

}


std::string CheapCustomer::toString() const{
    return getName()+",chp";
}


//HeavyMuscleCustomer
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id):Customer(name,id){}
std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options){
    std::vector <int> mcl_id;
    for(size_t i=0; i<workout_options.size(); i++) {
        if (workout_options[i].getType() == ANAEROBIC)
            mcl_id.push_back(workout_options[i].getId());
    }

    std::stable_sort(mcl_id.begin(), mcl_id.end(), [&workout_options](int WK1_id, int WK2_id){
        return (workout_options[WK1_id].getPrice()>workout_options[WK2_id].getPrice());
    });
    setOrdered();
    return mcl_id;
}

Customer* HeavyMuscleCustomer::clone(){
    Customer* toAdd=new HeavyMuscleCustomer(getName(), getId());
    if(getOrdered()) toAdd->setOrdered();
    return toAdd;

}

std::string HeavyMuscleCustomer::toString() const{
    return getName()+",mcl";
}


//FullBodyCustomer
FullBodyCustomer::FullBodyCustomer(std::string name, int id):Customer(name,id){}
std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options){
    int chp_cardio=INT32_MAX;
    int chp_cardio_id;
    int exp_min=-1;
    int exp_min_id;
    int chp_anaerobic=INT32_MAX;
    int chp_anaerobic_id;
    std::vector<int> fbc;

    for(size_t i=0; i<workout_options.size(); i++){
        if((workout_options[i].getType()==CARDIO) && (workout_options[i].getPrice()<chp_cardio)){
            chp_cardio=workout_options[i].getPrice();
            chp_cardio_id=workout_options[i].getId();
        }

        else if((workout_options[i].getType()==MIXED) && (workout_options[i].getPrice()>exp_min)){
            exp_min=workout_options[i].getPrice();
            exp_min_id=workout_options[i].getId();
        }

        else if((workout_options[i].getType()==ANAEROBIC)&& (workout_options[i].getPrice()<chp_anaerobic)){
            chp_anaerobic=workout_options[i].getPrice();
            chp_anaerobic_id=workout_options[i].getId();
        }

    }

    fbc.push_back(chp_cardio_id);
    fbc.push_back(exp_min_id);
    fbc.push_back(chp_anaerobic_id);

    setOrdered();
    return fbc;
}
Customer* FullBodyCustomer::clone(){
    Customer* toAdd=new FullBodyCustomer(getName(), getId());
    if(getOrdered()) toAdd->setOrdered();
    return toAdd;
}



std::string FullBodyCustomer::toString() const{
    return getName()+",fbd";
}