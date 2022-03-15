#include "../include/Studio.h"
#include "../include/Trainer.h"
#include "../include/Workout.h"
#include <fstream>
#include <sstream>

Studio::Studio():open(true), number_of_trainers(0), id_customer_counter(0){}


    Studio::Studio(const std::string &configFilePath):open(true), number_of_trainers(0), id_customer_counter(0) {
        std::ifstream file(configFilePath);
        std::string line;
        std::vector<std::string> string_file;
        while (file) {
            getline(file, line);
            if (line[0] == '#' || line[0] == '\0') {
                continue;
            } else {
                string_file.push_back(line);
            }
        }
        int num_of_trainers;
        std::stringstream make_int;
        make_int << string_file[0];
        make_int >> num_of_trainers;
        number_of_trainers=num_of_trainers;

        //creat trainers
        std::string list_capacities = string_file[1];
        for (size_t i = 0; i < list_capacities.size(); i++) {
            std::string capacity = "";
            while (i < list_capacities.size() && list_capacities[i] != ',') {
                capacity = capacity + list_capacities[i];
                i++;
            }
                int capacity_of_trainer;
                std::stringstream make_int2;
                make_int2 << capacity;
                make_int2 >> capacity_of_trainer;
                Trainer* creat_trainer = new Trainer(capacity_of_trainer);
                trainers.push_back(creat_trainer);
            }
        //creat workout option
        int id_counter = 0;
        for (size_t i = 2; i < string_file.size(); i++) {
            std::string workout_string = string_file[i];
            std::string workout_name="";
            std::string workout_type="";
            std::string workout_price="";
            size_t j=0;
            //workout name
            while (j<workout_string.size() && workout_string[j]!=','){
                    workout_name=workout_name+workout_string[j];
                j++;
            }
            j++;
            //workout type
            while (j<workout_string.size() && workout_string[j]!=','){
                if (workout_string[j]!=' ')
                    workout_type=workout_type+workout_string[j];
                j++;
            }
            j++;
            //workout price
            while (j<workout_string.size() && workout_string[j]!=','){
                if (workout_string[j]!=' ')
                    workout_price=workout_price+workout_string[j];
                j++;
            }
            WorkoutType curr_workout_type;
            if ((workout_type[0]=='a')||(workout_type[0]=='A'))
                curr_workout_type=ANAEROBIC;
            else if ((workout_type[0]=='m')||(workout_type[0]=='M'))
                curr_workout_type= MIXED;
            else
                curr_workout_type= CARDIO;

            std::stringstream make_int3;
            int curr_workout_price;
            make_int3 << workout_price;
            make_int3 >> curr_workout_price;

            Workout curr_workout = Workout(id_counter,workout_name,curr_workout_price, curr_workout_type);
            workout_options.push_back(curr_workout);

            id_counter++;
        }
    }

    void Studio::start() {
        std::cout<<"Studio is now open!"<<std::endl;
        std:: string input;
        std::getline(std::cin,input);
        while (input!="closeall") {
            if (input.substr(0, 4) == "open") {
                std:: string customer_name="";
                std:: string customer_type="";
                std::vector<Customer *> customersList;
                std::stringstream make_int;
                int trainer_id;
                std::string string_trainer_id;
                size_t i=5;
                while(input[i]!=' ') {
                    string_trainer_id = string_trainer_id + input[i];
                    i++;
                }
                make_int << string_trainer_id;
                make_int>> trainer_id;
                if (getTrainer(trainer_id)!= nullptr) {
                    int capacity_trainer = trainers[trainer_id]->getCapacity();
                    i++;
                    if (!trainers[trainer_id]->isOpen()) {
                        while ((i < input.size()) && (capacity_trainer > 0)) {
                            while (input[i] != ',') {
                                customer_name = customer_name + input[i];
                                i++;
                            }
                            i++;
                            customer_type = input.substr(i, 3);
                            i = i + 4;
                            Customer *new_customer;
                            if (customer_type == "swt")
                                new_customer = new SweatyCustomer(customer_name, id_customer_counter);

                            else if (customer_type == "chp")
                                new_customer = new CheapCustomer(customer_name, id_customer_counter);

                            else if (customer_type == "mcl")
                                new_customer = new HeavyMuscleCustomer(customer_name, id_customer_counter);

                            else
                                new_customer = new FullBodyCustomer(customer_name, id_customer_counter);

                            customersList.push_back(new_customer);
                            capacity_trainer--;
                            id_customer_counter = id_customer_counter + 1;
                            customer_name = "";
                        }
                    }
                }
                    BaseAction *last_action = new OpenTrainer(trainer_id, customersList);
                    last_action->act(*this);
                    actionsLog.push_back(last_action);
            }

            else if (input.substr(0, 5) == "order") {
                int trainer_id;
                std::stringstream make_int;
                std::string string_trainer_id="";
                size_t i=6;
                while(i<input.size()&&input[i]!=' ') {
                    string_trainer_id = string_trainer_id + input[i];
                    i++;
                }
                make_int << string_trainer_id;
                make_int>> trainer_id;
                BaseAction* last_action = new Order (trainer_id);
                last_action->act(*this);
                actionsLog.push_back(last_action);
            }
            else if (input.substr(0, 5) == "close") {
                int trainer_id;
                std::stringstream make_int;
                std::string string_trainer_id="";
                size_t i=6;
                while(i<input.size()&&input[i]!=' ') {
                    string_trainer_id = string_trainer_id + input[i];
                    i++;
                }
                make_int << string_trainer_id;
                make_int>> trainer_id;
                BaseAction* last_action = new Close (trainer_id);
                last_action->act(*this);
                actionsLog.push_back(last_action);
            }
            else if (input.substr(0, 6) == "status") {
                int trainer_id;
                std::stringstream make_int;
                std::string string_trainer_id="";
                size_t i=7;
                while(i<input.size()&&input[i]!=' ') {
                    string_trainer_id = string_trainer_id + input[i];
                    i++;
                }
                make_int << string_trainer_id;
                make_int>> trainer_id;
                BaseAction* last_action = new PrintTrainerStatus(trainer_id);
                last_action->act(*this);
                actionsLog.push_back(last_action);
            }
            else if (input.substr(0, 4) == "move") {
                int origin_trainer_id;
                int dest_trainer_id;
                int customer_id;
                size_t i = 5;
                std::string string_origin_trainer_id="";
                std::string string_dest_trainer_id="";
                std::string string_customer_id="";
                while(i<input.size() && input[i]!=' ')
                {
                    string_origin_trainer_id=string_origin_trainer_id+input[i];
                    i++;
                }
                i++;
                while(i<input.size() && input[i]!=' ') {
                    string_dest_trainer_id = string_dest_trainer_id + input[i];
                    i++;
                }
                i++;
                while(i<input.size() && input[i]!=' ')
                {
                    string_customer_id=string_customer_id+input[i];
                    i++;
                }
                std::stringstream make_int;
                std::stringstream make_int2;
                std::stringstream make_int3;
                make_int << string_origin_trainer_id;
                make_int>> origin_trainer_id;
                make_int2 << string_dest_trainer_id;
                make_int2>> dest_trainer_id;
                make_int3 << string_customer_id;
                make_int3>> customer_id;
                BaseAction* last_action = new MoveCustomer (origin_trainer_id,dest_trainer_id,customer_id);
                last_action->act(*this);
                actionsLog.push_back(last_action);

                if (trainers[origin_trainer_id]->getCurr_capacity()==trainers[origin_trainer_id]->getCapacity() && trainers[origin_trainer_id]->isOpen()){
                    BaseAction* closeTrainer = new Close (origin_trainer_id);
                    closeTrainer->act(*this);
                    actionsLog.push_back(closeTrainer);
                }
            }

            else if (input=="workout_options") {
                BaseAction* last_action = new PrintWorkoutOptions ();
                last_action->act(*this);
                actionsLog.push_back(last_action);
            }

            else if (input=="log") {
                BaseAction* last_action = new PrintActionsLog ();
                last_action->act(*this);
                actionsLog.push_back(last_action);
            }

            else if (input=="backup") {
                BaseAction* last_action = new BackupStudio ();
                last_action->act(*this);
                actionsLog.push_back(last_action);
            }

            else if (input=="restore") {
                BaseAction* last_action = new RestoreStudio ();
                last_action->act(*this);
                actionsLog.push_back(last_action);
            }

            std::getline(std::cin,input);
        }

        BaseAction* last_action = new CloseAll();
        actionsLog.push_back(last_action);
        last_action->act(*this);
    }

    int Studio::getNumOfTrainers() const{
    return number_of_trainers;

}
    Trainer* Studio::getTrainer(int tid){
    size_t idt = tid;
    if(trainers.size()<=idt){
        return nullptr;
    }
    else
        return trainers[tid]; //The trainers Organized in the vector by their id
    }
    const std::vector<BaseAction*>& Studio::getActionsLog() const{ // Return a reference to the history of actions
    return actionsLog;
}
    std::vector<Workout>& Studio::getWorkoutOptions() {
        return workout_options;
}

//destructor
Studio::~Studio(){
    if (!trainers.empty()){
        for (size_t i=0; i<trainers.size();i++)
            delete trainers[i];
    }
    trainers.clear();
    if (!actionsLog.empty()){
        for (size_t i=0; i<actionsLog.size();i++)
            delete actionsLog[i];
    }
    actionsLog.clear();
    workout_options.clear();
}

//copy constructor
Studio::Studio(const Studio&other) {
    open = other.open;
    id_customer_counter=other.id_customer_counter;
    number_of_trainers=other.number_of_trainers;
    for (size_t i=0; i<other.trainers.size();i++) { //deep copy
        Trainer* temp_trainer =new Trainer(*other.trainers[i]);
        trainers.push_back(temp_trainer);
    }
    for (size_t i=0; i<other.workout_options.size();i++) { //deep copy
        workout_options.push_back(other.workout_options[i]);
    }
    for (size_t i=0; i<other.actionsLog.size();i++) { //deep copy
        actionsLog.push_back(other.actionsLog[i]->clone());
    }
}

// copy assignment operator
    Studio& Studio:: operator=(Studio &other){
    if (this!=&other) {
        open = other.open;
        id_customer_counter=other.id_customer_counter;
        number_of_trainers = other.number_of_trainers;
        for (size_t i = 0; i < trainers.size(); i++) {
            if (trainers[i]) delete trainers[i];
        }

        workout_options.clear();
        for (size_t i = 0; i < actionsLog.size(); i++) {
            if (actionsLog[i]) delete actionsLog[i];
        }
        trainers.clear();
        actionsLog.clear();

        for (size_t i = 0; i < other.trainers.size(); i++) { //deep copy
            Trainer *temp_trainer = new Trainer(*other.trainers[i]);
            this->trainers.push_back(temp_trainer);
        }
        for (size_t i = 0; i < other.workout_options.size(); i++) { //deep copy
            this->workout_options.push_back(other.workout_options[i]);
        }
        for (size_t i = 0; i < other.actionsLog.size(); i++) { //deep copy
            BaseAction *temp_baseAction = other.actionsLog[i]->clone();
            this->actionsLog.push_back(temp_baseAction);
        }
    }
        return *this;
}

//move constructor
Studio::Studio(Studio&& other){
    open=other.open;
    number_of_trainers=other.number_of_trainers;
    id_customer_counter=other.id_customer_counter;
    for(size_t i=0; i<other.trainers.size(); i++){
        trainers.push_back(other.trainers[i]);
        other.trainers[i]= nullptr;
    }
    other.trainers.clear();

    for(size_t i=0; i<other.actionsLog.size(); i++){
        actionsLog.push_back(other.actionsLog[i]);
        other.actionsLog[i]= nullptr;
    }
    other.actionsLog.clear();

    for(size_t i=0; i<other.workout_options.size();i++){
        workout_options.push_back(other.workout_options[i]);
    }
    other.workout_options.clear();
}

//move assignment operator
Studio& Studio:: operator=(Studio&& other){
    if (this!=&other){
        open=other.open;
        number_of_trainers=other.number_of_trainers;
        id_customer_counter=other.id_customer_counter;
        for(size_t i=0; i<actionsLog.size();i++){
            if(actionsLog[i])delete actionsLog[i];
            actionsLog[i]= nullptr;
        }
        actionsLog.clear();

        for(size_t i=0; i<other.actionsLog.size(); i++){
            actionsLog.push_back(other.actionsLog[i]);
            other.actionsLog[i]= nullptr;
        }
        other.actionsLog.clear();

        for(size_t i=0; i<trainers.size(); i++){
            if(trainers[i]) delete trainers[i];
            trainers[i]= nullptr;
        }
        trainers.clear();
        for(size_t i=0; i<other.trainers.size(); i++){
            trainers.push_back(other.trainers[i]);
            other.trainers[i]= nullptr;
        }
        other.trainers.clear();

        workout_options.clear();
        for(size_t i=0; i<other.workout_options.size();i++){
            workout_options.push_back(other.workout_options[i]);
        }
        other.workout_options.clear();
    }
    return *this;
}




