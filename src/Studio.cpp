#include "../include/Studio.h"
#include <vector>
#include <fstream>
#include <list>


using namespace std;


Studio::Studio():open(false), trainers(), workout_options(), actionsLog(){}

Studio::Studio(const std::string &configFilePath):
    open(true), trainers(), workout_options(), actionsLog()
{
    ifstream file(configFilePath);
    string check;
    //numbers that lets us know what type of line we are in
    int y = 0; 
    //how many trainers in studio
    int trainersCount; 
    int workoutID = 0;

    while(getline(file, check))
    {
        if(check.size() > 0 && check.at(0) != '#')
        {
            if (y == 0)
            {
                y++;
                trainersCount = stoi(check);
            }

            else if (y == 1)
            {
                for (int index = 0; index <trainersCount; index++)
                {
                int x = check.find_first_of(',');
                string copy = check.substr(0, x);
                int size = stoi(copy);
                trainers.push_back(new Trainer((size)));
                check.erase(0,x+1);
                }
            y++;
            }
            else
            {
                int x = check.find_first_of(',');
                string name = check.substr(0,x);
                check.erase(0 , x+2);
                x = check.find_first_of(',');
                string type = check.substr(0,x);
                check.erase(0, x+2);
                int price = stoi(check);
                if (!type.compare("Anaerobic"))
                    workout_options.push_back(Workout(workoutID,name, price, WorkoutType::ANAEROBIC));
            
                else if(!type.compare("Mixed"))
                    workout_options.push_back(Workout(workoutID,name, price, WorkoutType::MIXED));
            
                else
                    workout_options.push_back(Workout(workoutID,name, price, WorkoutType::CARDIO));

                workoutID++;
            }
        }
    }
    
    
}



void Studio::clear(){
    
    for(int i=0;i< (int)trainers.size();i++)
    {
        Trainer* T = trainers[i];
        delete(T);
    }
    trainers.clear();
    workout_options.clear();
    
    for(int i=0;i< (int)actionsLog.size();i++)
    {
        BaseAction* act = actionsLog[i];
        delete (act);
    }
    actionsLog.clear();      

}


Studio::~Studio()
{ //destructor
    clear();
}


Studio::Studio(const Studio& other_studio):
    open(other_studio.open), trainers(), workout_options(), actionsLog()
{
    for(int i =0;i< (int)other_studio.trainers.size();i++)
    {
        Trainer* T=other_studio.trainers.at(i);
        trainers.push_back(new Trainer(*T));
    }
    for(int i=0;i< (int)other_studio.workout_options.size();i++)
    {
        workout_options.push_back((other_studio.workout_options.at(i)));
    }
    for (int i = 0; i < (int)other_studio.actionsLog.size();i++ )
    {
        actionsLog.push_back(other_studio.actionsLog.at(i)->clone());
    }
}



Studio::Studio(Studio&& other_studio): 
open(other_studio.open), trainers(), workout_options(), actionsLog()
{ 
    trainers = move(other_studio.trainers);
    workout_options = move(other_studio.workout_options);
    actionsLog = move(other_studio.actionsLog);
    other_studio.clear();
}


Studio& Studio::operator=(const Studio& other_studio)
{

    if(this!=&other_studio)
    {

        clear();
        for(int i =0;i< (int)other_studio.trainers.size();i++)
        {
            Trainer* T=other_studio.trainers.at(i);
            trainers.push_back(new Trainer(*T));
        }
        for(int i=0;i< (int)other_studio.workout_options.size();i++)
        {
            workout_options.push_back((other_studio.workout_options.at(i)));
        }
        for (int i = 0; i < (int)other_studio.actionsLog.size();i++ )
        {
            actionsLog.push_back(other_studio.actionsLog.at(i)->clone());
        }
    }
    return *this;
}


Studio& Studio::operator=(Studio && other_studio)
{ //move assignment
    if(this!=&other_studio)
    {
        clear(); 
        open=other_studio.open;
        trainers=move(other_studio.trainers);
        workout_options=move(other_studio.workout_options);
        actionsLog=move(other_studio.actionsLog);
        other_studio.clear();
    }
    return *this;
}


void Studio::start()
{
    std::cout <<"Studio is now OPEN!!!"<< std::endl;

    int custID = 0;
    while (open) 
    {
        cout << "enter your next action:" << endl;
        char i[1000];
        cin.getline(i, 1000);
        string input(i);
        //string og = input;
      
        if (!input.compare("closeall")) 
        {
            open = false;
            CloseAll* x = new CloseAll();
            x->act(*this);
            actionsLog.push_back(x);
        }
        else if (!input.compare("workout_option")) 
        {
            PrintWorkoutOptions* x = new PrintWorkoutOptions(input);
            x->act(*this);
            actionsLog.push_back(x);



        }
        else if (!input.compare("log")) 
        {
            PrintActionsLog* x = new PrintActionsLog(input); 
            x->act(*this);
            actionsLog.push_back(x);

        }
        else if (!input.compare("backup")) 
        {
            BackupStudio* x = new BackupStudio(input);
            x->act(*this);
            actionsLog.push_back(x);
            
        }
        else if (!input.compare("restore")) 
        {
            RestoreStudio* x =new RestoreStudio(input);
            x->act(*this);
            actionsLog.push_back(x);
        }
        else 
        { //actions with more than 1 word
            int space = input.find_first_of(' ');
            string action = input.substr(0, space);
            input.erase(0, space + 1); //deleted the action, when getting into action, can already ignore action call.
            
            

            if (!action.compare("open")) 
            {
                input.append(" "); //to make life easier
                int space2 = input.find_first_of(' '); //getting trainer ID
                string id = input.substr(0, space2); //trainer id
                string temp = "open " + id  + " "; //make a string to send to action
                input.erase(0, space2 + 1);
                vector<Customer*> customerList;
                int count = 0;
                //check if there are customers and trainer id is valid
                if (input.size() > 0 && stoi(id) < getNumOfTrainers()){
                // while there are customers, and there is a free spot for the trainer and the trainer is open
                while (input.size() > 0 && count < (int)getTrainer(stoi(id))->getCapacity() && !(getTrainer(stoi(id))->isOpen()))  
                { //making customers and pushing to vector
                    int psik = input.find_first_of(',');
                    string name = input.substr(0, psik);
                    input.erase(0, psik + 1);
                    string type = input.substr(0, 3);
                    input.erase(0, 4);
                    temp = temp + name + "," + type + " ";  
                    
                    if (!type.compare("swt")) 
                    {
                        customerList.push_back(new SweatyCustomer(name, custID));
                    }
                    else if (!type.compare("chp")) 
                    {
                        customerList.push_back(new CheapCustomer(name, custID));
                    }
                    else if (!type.compare("mcl")) 
                    {
                        customerList.push_back(new HeavyMuscleCustomer(name, custID));
                    }
                    else 
                    {
                        customerList.push_back(new FullBodyCustomer(name, custID));
                    }
                    count ++;
                    custID ++;
                }
                temp.pop_back();

                OpenTrainer* x = new OpenTrainer(stoi(id), customerList, temp); 
                    x->act(*this);
                    actionsLog.push_back(x);
            }
                

            }
            else if (!action.compare("order")) 
            {
                string temp = "order " + input; 
                Order* op= new Order(stoi(input), temp); 
                op->act(*this);
                actionsLog.push_back(op);
            }
            else if (!action.compare("move")) 
            {
                string temp = "move " + input; 
                string og = input.substr(0, 1);
                string target = input.substr(2, 1);
                string  customer = input.substr(4, 1);
                MoveCustomer* op=new MoveCustomer(stoi(og), stoi(target), stoi(customer), temp); 
                op->act(*this);
                actionsLog.push_back(op);
                // if the action was successful, we are check if the origin trainer has no customers, if so closing him with action
                if (op->getStatus() == COMPLETED)
                {
                    if ((int)getTrainer(stoi(og))->getCustomers().size() == 0)
                    {
                        Close* closeT = new Close(stoi(og), "close " + og);
                        closeT->act(*this);
                        actionsLog.push_back(closeT);
                    } 
                }
            }
            else if (!action.compare("close")) 
            {
                // original cin string from future print in log print
                string temp = "close " + input; 
                Close* operation = new Close(stoi(input), temp);  
                operation->act(*this);
                actionsLog.push_back(operation);
            }
            else if (!action.compare("status")) 
            {
                string temp = "status " + input;
                PrintTrainerStatus* operation = new PrintTrainerStatus(stoi(input), temp);
                operation->act(*this);
                actionsLog.push_back(operation);
            }
        }
    }

}

int Studio::getNumOfTrainers() const
{
    return trainers.size();
}

Trainer* Studio::getTrainer(int tid)
{
    if (tid >= 0 && tid <(int)trainers.size())
    {
        return trainers[tid];
    } 
    else
        return nullptr;
}

const std::vector<BaseAction*>& Studio::getActionsLog() const
{
    return actionsLog;
}

std::vector<Workout>& Studio::getWorkoutOptions()
{
    return workout_options;
}



bool Studio::isOpen()
{
    return open;
}


