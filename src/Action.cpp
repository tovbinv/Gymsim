#include "..//include/Action.h"
#include "..//include/Trainer.h"
#include "..//include/Studio.h"
#include <string>
#include <vector>
#include <iostream>
extern Studio *backup;

using namespace std;

BaseAction::BaseAction() : errorMsg(), status() {}

void BaseAction::complete()
{
    status = (COMPLETED);
    errorMsg = "";
}

string BaseAction::getErrorMsg() const
{
    return errorMsg;
}

void BaseAction::error(string errorMsg_new)
{
    errorMsg = errorMsg_new;
    status = ERROR;
}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

//------------------------------------------------------------------------------------------//

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList): trainerId(id), customers(customersList), default_str("") {}

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList, string str): trainerId(id), customers(customersList), default_str(str) {}

OpenTrainer::~OpenTrainer()
{
    customers.clear();
};

OpenTrainer *OpenTrainer::clone()
{
    vector<Customer *> customer;
    return new OpenTrainer(trainerId, customers, default_str);
}

void OpenTrainer::act(Studio &studio)
{
    Trainer *T = studio.getTrainer(trainerId);
    if (T == nullptr || (*T).isOpen() == true)
    { //if trainer is open or the customers to many we print error massage
        error("Error: Workout session does not exist or is already open.");
        cout << getErrorMsg() << endl;
    }
    else if (customers.size() > 0)
    {
        (*T).openTrainer();

        for (int i = 0; i < (int)customers.size() && i < (*T).getCapacity(); i++)
        {
            (*T).addCustomer(customers.at(i));
        }

        complete();
    }
}

string OpenTrainer::toString() const
{
    if (getStatus() == 0)
    {
        return (default_str + " Complete");
    }
    else
    {
        return (default_str + " " + getErrorMsg());
    }
}

string OpenTrainer::getType() const
{
    return "openT";
}

//------------------------------------------------------------------------------------------//

Order::Order(int id) : trainerId(id), default_str("") {}

Order::Order(int id, string str) : trainerId(id), default_str(str) {}

Order::~Order() = default;

Order *Order::clone() { return new Order(*this); }

void Order::act(Studio &studio)
{ //what if wrong id
    Trainer *T = studio.getTrainer(trainerId);
    if (T == nullptr || (*T).isOpen() == false)
    {
        error("Error: Trainer does not exist or is not open.");
        cout << getErrorMsg() << endl;
    }
    else
    { 
        vector<Customer *> customersList = (*T).getCustomers();
        vector<Workout> workout_options = studio.getWorkoutOptions();
        for (int i = 0; i < (int)customersList.size(); i++)
        {
            (*T).order((*customersList.at(i)).getId(), (*customersList.at(i)).order(workout_options), workout_options);
        }
        for (OrderPair pair : T->getOrders())
        {
            cout << T->getCustomer(pair.first)->getName() << " Is Doing " << pair.second.getName() << endl;
        }
        complete();
    }
}

string Order::toString() const
{
    if (getStatus() == COMPLETED)
    {
        return (default_str + " complete");
    }
    else
    {
        return (default_str + " " + getErrorMsg());
    }
}

string Order::getType() const
{
    return "order";
}

//------------------------------------------------------------------------------------------//

MoveCustomer::MoveCustomer(int src, int dst, int customerId):
    srcTrainer(src), dstTrainer(dst), id(customerId), default_str("") {}

MoveCustomer::MoveCustomer(int src, int dst, int customerId, string str):
    srcTrainer(src), dstTrainer(dst), id(customerId), default_str(str) {}

MoveCustomer::~MoveCustomer() = default;

MoveCustomer *MoveCustomer::clone() { return new MoveCustomer(*this); }

void MoveCustomer::act(Studio &studio)
{
    Trainer *src = studio.getTrainer(srcTrainer);
    Trainer *dst = studio.getTrainer(dstTrainer);
    if (src != nullptr && dst != nullptr && (*src).isOpen() == true && (*dst).isOpen() == true)
    {
        if ((int)(*dst).getCustomers().size() < (*dst).getCapacity())
        {
            Customer *toMove = (*src).getCustomer(id);
            if (toMove != nullptr)
            {
                string name = toMove->getName();
                if (!toMove->toString().compare("sweaty"))
                    dst->addCustomer(new SweatyCustomer(name, id));

                else if (!toMove->toString().compare("cheap"))
                    dst->addCustomer(new CheapCustomer(name, id));

                else if (!toMove->toString().compare("heavy"))
                    dst->addCustomer(new HeavyMuscleCustomer(name, id));

                else
                    dst->addCustomer(new FullBodyCustomer(name, id));

                vector<int> list = toMove->order(studio.getWorkoutOptions());
                dst->order(id, list, studio.getWorkoutOptions());
                src->removeCustomer(id);
                list.clear();
                complete();
            }
            else
            {
                error("Error: Cannot move customer.");
                cout << getErrorMsg() << endl;
            }
            // if ((*src).getCustomers().size() == 0)
            // {
            //     (*src).closeTrainer();
            // }
        }
        else
        {
            error("Error: Cannot move customer.");
            cout << getErrorMsg() << endl;
        }
    }
    else
    {
        error("Error: Cannot move customer.");
        cout << getErrorMsg() << endl;
    }
}

string MoveCustomer::toString() const
{
    if (getStatus() == COMPLETED)
    {
        return (default_str + " complete");
    }
    else
    {
        return (default_str + " " + getErrorMsg());
    }
}

string MoveCustomer::getType() const
{
    return "move";
}

//------------------------------------------------------------------------------------------//

Close::Close(int id) : trainerId(id), default_str("") {}

Close::Close(int id, string str) : trainerId(id), default_str(str) {}

Close::~Close() = default;

Close *Close::clone() { return new Close(*this); }

void Close::act(Studio &studio)
{
    Trainer *T = studio.getTrainer(trainerId);
    if (T != nullptr && (*T).isOpen() == true)
    {
        cout << "Trainer" << default_str.substr(5) << " closed. salary " << (*T).getSalary() << "NIS." << endl;
        (*T).closeTrainer(); 
        complete();
    }
    else
    {
        error("Error: Trainer does not exist or is not open.");
        cout << getErrorMsg() << endl;
    }
}

string Close::toString() const
{
    if (getStatus() == COMPLETED)
    {
        return (default_str + " complete");
    }
    else
    {
        return (default_str + " " + getErrorMsg());
    }
}

string Close::getType() const
{
    return "close";
}

//------------------------------------------------------------------------------------------//

CloseAll::CloseAll() : default_str("") {}

CloseAll::CloseAll(string str) : default_str(str) {}

CloseAll::~CloseAll() = default;

CloseAll *CloseAll::clone() { return new CloseAll(*this); }

void CloseAll::act(Studio &studio)
{
    //never return error
    const int num_of_trainers = studio.getNumOfTrainers();
    for (int i = 0; i < num_of_trainers; i++)
    {
        Trainer *T = studio.getTrainer(i);
        if (T->isOpen() == true)
        {
            cout << "Trainer" << i << " closed. salary " << (*T).getSalary() << "NIS." << endl;
            (*T).closeTrainer();
        }
    }
    complete();
}

string CloseAll::toString() const
{
    if (getStatus() == COMPLETED)
    {
        return (default_str + " complete");
    }
    else
    {
        return (default_str + " " + getErrorMsg());
    }
}
string CloseAll::getType() const
{
    return "closeAll"; 
}

//------------------------------------------------------------------------------------------//

PrintWorkoutOptions::PrintWorkoutOptions() : default_str("") {}

PrintWorkoutOptions::PrintWorkoutOptions(string str) : default_str(str) {}

PrintWorkoutOptions::~PrintWorkoutOptions() = default;

PrintWorkoutOptions *PrintWorkoutOptions::clone() { return new PrintWorkoutOptions(*this); }

void PrintWorkoutOptions::act(Studio &studio)
{
    //never return error
    vector<Workout> workout_options = studio.getWorkoutOptions();
    string type="";
    for (int i = 0; i < (int)workout_options.size(); i++)
    {
        if(workout_options.at(i).getType()==0)
        {
            type="Anaerobic";
        }else if(workout_options.at(i).getType()==1)
        {
            type="Mixed";
        }else
        {
            type="Cardio";
        }
        cout << workout_options.at(i).getName() << ", " << type << ", " << workout_options.at(i).getPrice() << endl;
    }
    complete();
}

string PrintWorkoutOptions::toString() const
{
    if (getStatus() == COMPLETED)
    {
        return (default_str + " complete");
    }
    else
    {
        return (default_str + " " + getErrorMsg());
    }
}

string PrintWorkoutOptions::getType() const
{
    return "workout";
}

//------------------------------------------------------------------------------------------//

PrintTrainerStatus::PrintTrainerStatus(int trainer_id) : trainerId(trainer_id), default_str("") {}

PrintTrainerStatus::PrintTrainerStatus(int trainer_id, string str) : trainerId(trainer_id), default_str(str) {}

PrintTrainerStatus::~PrintTrainerStatus() = default;

PrintTrainerStatus *PrintTrainerStatus::clone() { return new PrintTrainerStatus(*this); }

void PrintTrainerStatus::act(Studio &studio)
{
    //never return error
    Trainer *T = studio.getTrainer(trainerId);
    if ((*T).isOpen() == false)
    {
        cout << "Trainer " << trainerId << " status: closed" << endl;
    }
    else
    {
        cout << "Trainer " << trainerId << " status: open" << endl;
        vector<OrderPair> orderList = (*T).getOrders();
        if (orderList.size() > 0)
        {
            cout << "Customers:" << endl;
            vector<Customer *> customersList = (*T).getCustomers();
            for (int i = 0; i < (int)customersList.size(); i++)
            {
                cout << (*customersList.at(i)).getId() << " " << (*customersList.at(i)).getName() << endl;
            }
            for (int i = 0; i < (int)orderList.size(); i++)
            {
                if (orderList.at(i).first != -1)
                {
                    int temp_id = orderList.at(i).first;
                    Workout temp_workout = orderList.at(i).second;
                    cout << temp_workout.getName() << " " << temp_workout.getPrice() << " " << temp_id << endl;
                }
            }
            cout << "Current Trainer’s Salary: " << (*T).getSalary() << endl;
            complete();
        }
    }
}

string PrintTrainerStatus::toString() const
{
    if (getStatus() == COMPLETED)
    {
        return (default_str + " complete");
    }
    else
    {
        return (default_str + " " + getErrorMsg());
    }
}

string PrintTrainerStatus::getType() const
{
    return "status";
}

//------------------------------------------------------------------------------------------//

PrintActionsLog::PrintActionsLog() : default_str("") {}

PrintActionsLog::PrintActionsLog(string str) : default_str(str) {}

PrintActionsLog::~PrintActionsLog() = default;

PrintActionsLog *PrintActionsLog::clone() { return new PrintActionsLog(*this); }

void PrintActionsLog::act(Studio &studio)
{
    vector<BaseAction *> Log = studio.getActionsLog();
    for (int i = 0; i < (int)Log.size(); i++)
    {
        cout << Log.at(i)->toString() << endl;
    }
    complete();
}

string PrintActionsLog::toString() const
{
    if (getStatus() == COMPLETED)
    {
        return (default_str + " complete");
    }
    else
    {
        return (default_str + " " + getErrorMsg());
    }
}

string PrintActionsLog::getType() const
{
    return "log";
}

//------------------------------------------------------------------------------------------//

BackupStudio::BackupStudio() : default_str("") {}

BackupStudio::BackupStudio(string str) : default_str(str) {}

BackupStudio::~BackupStudio() = default;

BackupStudio *BackupStudio::clone() { return new BackupStudio(*this); }

void BackupStudio::act(Studio &studio)
{
    if (backup == nullptr)
    {
        backup = new Studio();
    }
    backup->operator=(studio);
    complete();
}

string BackupStudio::toString() const
{
    if (getStatus() == COMPLETED)
    {
        return (default_str + " complete");
    }
    else
    {
        return (default_str + " " + getErrorMsg());
    }
}

string BackupStudio::getType() const
{
    return "backup";
}

//------------------------------------------------------------------------------------------//

RestoreStudio::RestoreStudio() : default_str("") {}

RestoreStudio::RestoreStudio(string str) : default_str(str) {}

RestoreStudio::~RestoreStudio() = default;

RestoreStudio *RestoreStudio::clone() { return new RestoreStudio(*this); }

void RestoreStudio::act(Studio &studio)
{
    if (backup != nullptr)
    {
        studio = *backup;
        complete();
    }
    else
    {
        error("Error: No backup available");
        cout << getErrorMsg() << endl;
    }
}

string RestoreStudio::toString() const
{
    if (getStatus() == COMPLETED)
    {
        return (default_str + " complete");
    }
    else
    {
        return (default_str + " " + getErrorMsg());
    }
}

string RestoreStudio::getType() const
{
    return "restore";
}
