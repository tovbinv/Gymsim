#ifndef STUDIO_H_
#define STUDIO_H_
#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"
#include <vector>

class Studio{		 //need dustructor,copy constructor,move constructor,copy assignment operator,move assignment operator
public:
	Studio();//constructor
    Studio(const std::string &configFilePath);
    ~Studio();//destructor
    void clear();
    Studio(const Studio &studio);// copy constructor
    Studio( Studio &&studio);//move constructor
    Studio& operator=(const Studio &studio);// Copy Assignment
    Studio& operator=(Studio && studio);// Move Assignment
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid); // null if not exist
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    std::vector<Trainer*>& getTrainers();
    bool isOpen();
    
    


private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
};

#endif