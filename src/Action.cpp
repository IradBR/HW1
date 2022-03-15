#include "../include/Action.h"
#include "../include/Studio.h"
#include "../include/Trainer.h"
extern Studio* backup;

//BaseAction
BaseAction::BaseAction(){}
BaseAction::~BaseAction(){}
ActionStatus BaseAction::getStatus() const{
    return status;
}

std::string BaseAction::toStringStatus() const{
    if (status==COMPLETED)
        return "Completed";
    else {
        std::string s = "Error: " + getErrorMsg();
        return s;
    }
}

void BaseAction::complete(){
    status=COMPLETED;
}


void BaseAction::error(std::string errorMsg){
    status= ERROR;
    this->errorMsg=errorMsg;

}
void BaseAction::setStatus(ActionStatus Status){
    status=Status;
}
void BaseAction::setErrorMs(std::string  msg){
    errorMsg=msg;
}
void BaseAction::setString(std::string act){
    act_string=act;
}

std::string BaseAction::get_act_String() {
    return act_string;
}

std::string BaseAction::getErrorMsg() const{
    return errorMsg;

};


//OpenTrainer
OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList):BaseAction(),trainerId(id), customers(customersList){}

void OpenTrainer::act(Studio &studio){
    Trainer* trainer = studio.getTrainer(trainerId);
    if(trainer== nullptr||trainer->isOpen()){
        error("Workout session does not exist or is already open");
        std::cout<< "Error: "+getErrorMsg()<<std::endl;
    }
    else{
        trainer->setWasOpen();
        trainer->openTrainer();
        bool full= false;
        for(size_t i=0; i<customers.size() ;i++){
            if (!full) {
                if (trainer->getCurr_capacity() > 0)
                    trainer->addCustomer(customers[i]);
                else
                    full = true;
            }
        }
        complete();
    }
    setString(toString());
}

std::string OpenTrainer::toString() const{
    std:: string toString_OpenTrainer = "open "+std::to_string(trainerId);
    for (size_t i=0; i<customers.size(); i++)
        toString_OpenTrainer=toString_OpenTrainer+" "+customers[i]->toString();
    toString_OpenTrainer=toString_OpenTrainer+" "+toStringStatus();
    return toString_OpenTrainer;
}



BaseAction* OpenTrainer::clone(){

    OpenTrainer* open= new OpenTrainer(this->trainerId, customers);
    open->setErrorMs(this->getErrorMsg());
    open->setStatus(this->getStatus());
    open->setString(this->get_act_String());
    return open;
}

//Order
Order::Order(int id): BaseAction(),trainerId(id){}

void Order::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || !trainer->isOpen()) {
        error("Trainer dose not exist or is not open");
        std::cout<< "Error: "+getErrorMsg()<<std::endl;
    }
    else {
        std::cout << "order " << trainerId<<std::endl;
        std::vector<Customer *> customers = trainer->getCustomers();
        std::vector<Workout> WorkoutOptions= studio.getWorkoutOptions();

        for (size_t i = 0; i < customers.size(); i++) {
            if (!customers[i]->getOrdered()) {
                std::vector<int> customer_workouts = customers[i]->order(WorkoutOptions);
                trainer->order(customers[i]->getId(), customer_workouts, WorkoutOptions);
            }
        }

        std::vector<OrderPair> pairs= trainer->getOrders();
        for(size_t i=0; i<pairs.size(); i++){
            std::cout<< trainer->getCustomer(pairs[i].first)->getName()<< " Is Doing "<<pairs[i].second.getName()<<std::endl;
        }
        complete();
    }
    setString(toString());

}

BaseAction* Order::clone(){
    Order* order= new Order(this->trainerId);
    order->setErrorMs(this->getErrorMsg());
    order->setStatus(this->getStatus());
    order->setString(this->get_act_String());
    return order;

}



std::string Order::toString() const{
    std:: string toString_Order = "order "+std::to_string(trainerId);
    toString_Order=toString_Order+" "+toStringStatus();
    return toString_Order;
}

//MoveCustomer
MoveCustomer::MoveCustomer(int src, int dst, int customerId):BaseAction(),srcTrainer(src),dstTrainer(dst),id(customerId){}
void MoveCustomer::act(Studio &studio){
    Trainer* src_trainer= studio.getTrainer(srcTrainer);
    Trainer* dst_trainer= studio.getTrainer(dstTrainer);
    Customer* moving= src_trainer->getCustomer(id);
    if(src_trainer== nullptr|| dst_trainer== nullptr||moving==nullptr || !(src_trainer->isOpen())|| !(dst_trainer->isOpen())||  dst_trainer->getCurr_capacity()==0) {
        error("Cannot move customer");
        std::cout<< "Error: "+getErrorMsg()<<std::endl;
    }

    else{
        src_trainer->removeCustomer(id);
        dst_trainer->addCustomer(moving);

        if(moving->getOrdered()) { //if the customer was ordered, he needs to be order also at the dst trainer.
            dst_trainer->order(id,moving->order(studio.getWorkoutOptions()),studio.getWorkoutOptions());
        }

        complete();
    }
    setString(toString());
}
BaseAction* MoveCustomer::clone(){
    MoveCustomer* move= new MoveCustomer(this->srcTrainer, this->dstTrainer, this->id);
    move->setErrorMs(this->getErrorMsg());
    move->setStatus(this->getStatus());
    move->setString(this->get_act_String());
    return move;
}

std::string MoveCustomer::toString() const{
    std:: string toString_Move = "move "+std::to_string(srcTrainer)+" "+std::to_string(dstTrainer)+" "+std::to_string(id);
    toString_Move =toString_Move +" "+toStringStatus();
    return toString_Move ;
}

//Close
Close::Close(int id):BaseAction(),trainerId(id){}

void Close::act(Studio &studio){
    Trainer* trainer= studio.getTrainer(trainerId);
    if(trainer== nullptr || !trainer->isOpen()){
        error("Trainer dose not exist or is not open");
        std::cout<< "Error: "+getErrorMsg()<<std::endl;
    }
    else {
        trainer->getSalary();
        trainer->closeTrainer();
        std::cout<< "Trainer "<<trainerId<<" closed."<<" salary "<< trainer->getSalary()<<"NIS"<<std::endl;
        complete();
    }
    setString(toString());
}
BaseAction* Close::clone(){
    Close* close=new Close(this->trainerId);
    close->setErrorMs(this->getErrorMsg());
    close->setStatus(this->getStatus());
    close->setString(this->get_act_String());
    return close;
}


std::string Close::toString() const{
    std:: string toString_Close = "close "+std::to_string(trainerId);
    toString_Close=toString_Close+" "+toStringStatus();
    return toString_Close;
}


//CloseAll
CloseAll::CloseAll():BaseAction(){}
void CloseAll::act(Studio &studio){
    int num_of_trainers= studio.getNumOfTrainers();
    for(int i=0; i<num_of_trainers; i++){
        Trainer* trainer= studio.getTrainer(i);
        if(trainer->isOpen()) {
            std::cout << "Trainer "<<i<< " closed. Salary " <<trainer->getSalary()<<"NIS"<<std::endl;
            trainer->closeTrainer();
        }
    }
    complete();
    setString(toString());
}

BaseAction* CloseAll::clone(){
    CloseAll* closeAll= new CloseAll();
    closeAll->setErrorMs(this->getErrorMsg());
    closeAll->setStatus(this->getStatus());
    closeAll->setString(this->get_act_String());
    return closeAll;
}

std::string CloseAll::toString() const{
    return "closeall "+ toStringStatus();
}


//PrintWorkoutOptions
PrintWorkoutOptions::PrintWorkoutOptions():BaseAction(){}
void PrintWorkoutOptions::act(Studio &studio){
    std::vector<Workout> workoutOptions= studio.getWorkoutOptions();
    for(size_t i=0; i<workoutOptions.size(); i++){
        std::cout<<workoutOptions[i].toString()<<std::endl;
    }
    complete();
    setString(toString());
}

BaseAction* PrintWorkoutOptions::clone(){
    PrintWorkoutOptions* printWorkoutOptions=new PrintWorkoutOptions();
    printWorkoutOptions->setErrorMs(this->getErrorMsg());
    printWorkoutOptions->setStatus(this->getStatus());
    printWorkoutOptions->setString(this->get_act_String());
    return printWorkoutOptions;
}

std::string PrintWorkoutOptions::toString() const{
    return "workout_option " +toStringStatus();
}



//PrintTrainerStatus
PrintTrainerStatus::PrintTrainerStatus(int id):BaseAction(),trainerId(id){}
void PrintTrainerStatus::act(Studio &studio){
    Trainer* trainer=studio.getTrainer(trainerId);

    if(trainer== nullptr || !trainer->getWasOpen()) {
        complete();
    }

    else if(!trainer->isOpen() ){
        std::cout<<"Trainer "<<trainerId<<" status: closed"<<std::endl;
    }
    else{
        std::cout<<"Trainer "<<trainerId<<" status: open"<<"\n"<<"Customers:"<<std::endl;
        std::vector<OrderPair>pairs= trainer->getOrders();
        std::vector<Customer*>customers= trainer->getCustomers();

        for(size_t i=0; i<customers.size();i++){
            std::cout <<customers[i]->getId()<<" "<<customers[i]->getName()<<std::endl;
        }
        for(size_t i=0; i<pairs.size();i++){
            std::cout <<pairs[i].second.getName()<<" "<<pairs[i].second.getPrice()<<"NIS "<<pairs[i].first<<std::endl;
        }
        std::cout<< "Current Trainer's salary:  "<< trainer->getSalary()<<"NIS"<<std::endl;
        complete();
    }
    setString(toString());
}

BaseAction* PrintTrainerStatus::clone(){
    PrintTrainerStatus* printTrainerStatus= new PrintTrainerStatus(this->trainerId);
    printTrainerStatus->setErrorMs(this->getErrorMsg());
    printTrainerStatus->setStatus(this->getStatus());
    printTrainerStatus->setString(this->get_act_String());
    return printTrainerStatus;
}


std::string PrintTrainerStatus::toString() const{
    std:: string toString_PrintTrainerStatus = "status "+std::to_string(trainerId)+" Completed";
    return toString_PrintTrainerStatus;
}



//PrintActionsLog
PrintActionsLog::PrintActionsLog():BaseAction(){}
void PrintActionsLog::act(Studio &studio)
{
    std::vector<BaseAction*> actionsLog= studio.getActionsLog();
    for (size_t i=0; i<actionsLog.size();i++)
    {
        std::cout<<actionsLog[i]->get_act_String()<<std::endl;
    }
    complete();
    setString(toString());
}
BaseAction* PrintActionsLog::clone(){
    PrintActionsLog* printActionsLog= new PrintActionsLog();
    printActionsLog->setErrorMs(this->getErrorMsg());
    printActionsLog->setStatus(this->getStatus());
    printActionsLog->setString(this->get_act_String());
    return  printActionsLog;
}

std::string PrintActionsLog::toString() const
{
    return "log "+toStringStatus();
}

//BackupStudio
BackupStudio::BackupStudio():BaseAction(){}
void BackupStudio::act(Studio &studio){
    if(backup)
        delete backup;
    backup= new Studio(studio);
    complete();
    setString(toString());
}

BaseAction* BackupStudio::clone(){
    BackupStudio* backupStudio= new BackupStudio();
    backupStudio->setErrorMs(this->getErrorMsg());
    backupStudio->setStatus(this->getStatus());
    backupStudio->setString(this->get_act_String());
    return  backupStudio;
}

std::string BackupStudio::toString() const{
    return "backup " +toStringStatus();
}


//RestoreStudio
RestoreStudio::RestoreStudio():BaseAction(){}
void RestoreStudio::act(Studio &studio){
    if(backup== nullptr){
        error(("No backup available"));
        std::cout<<"Error:"<<getErrorMsg()<<std::endl;
    }
    else{
        studio=*backup;
    }
    complete();
    setString(toString());
}
BaseAction* RestoreStudio::clone(){
    RestoreStudio* restoreStudio=new RestoreStudio();
    restoreStudio->setErrorMs(this->getErrorMsg());
    restoreStudio->setStatus(this->getStatus());
    restoreStudio->setString(this->get_act_String());
    return  restoreStudio;
}

std::string RestoreStudio::toString() const{
    return "restore " +toStringStatus();
}