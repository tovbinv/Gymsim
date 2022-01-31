#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"
using namespace std;

//no need for rule of 5(no need to copmare actions,copy a exist action,the action fields on the stack(automatically deleted),no need to move(rvalue) )


enum ActionStatus{
    COMPLETED, ERROR
};

//Forward declaration
class Studio;

class BaseAction{
public:
    BaseAction();
    virtual ~BaseAction()=default;//new
    ActionStatus getStatus() const;
    virtual void act(Studio& studio)=0;
    virtual std::string toString() const=0;
    virtual std::string getType() const = 0; //short way to get what class 
    virtual BaseAction* clone() =0;
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
    
};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, std::vector<Customer *> &customersList);
    OpenTrainer(int id, std::vector<Customer *> &customersList,string str);
    virtual ~OpenTrainer();
    // OpenTrainer(const OpenTrainer & other_trainer);
    void act(Studio &studio);
    std::string toString() const;
    std::string getType() const;
    OpenTrainer* clone();
private:
	const int trainerId;
	std::vector<Customer *> customers;
    std::string default_str;
};


class Order : public BaseAction {
public:
    Order(int id);
    Order(int id,string str);
    virtual ~Order();
    void act(Studio &studio);
    std::string toString() const;
    std::string getType() const;
    Order* clone();
private:
    const int trainerId;
    std::string default_str;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    MoveCustomer(int src, int dst, int customerId,string str);
    virtual ~MoveCustomer();
    void act(Studio &studio);
    std::string toString() const;
    std::string getType() const;
    MoveCustomer* clone();
private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
    std::string default_str;
};


class Close : public BaseAction {
public:
    Close(int id);
    Close(int id,string str);
    virtual ~Close();
    void act(Studio &studio);
    std::string toString() const;
    std::string getType() const;
    Close* clone();
private:
    const int trainerId;
    std::string default_str;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    CloseAll(string str);
    virtual ~CloseAll();
    void act(Studio &studio);
    std::string toString() const;
    std::string getType() const;
    CloseAll* clone();
private:
std::string default_str;
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    PrintWorkoutOptions(string str);
    virtual ~PrintWorkoutOptions();
    void act(Studio &studio);
    std::string toString() const;
    std::string getType() const;
    PrintWorkoutOptions* clone();
private:
std::string default_str;
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    PrintTrainerStatus(int id,string str);
    virtual ~PrintTrainerStatus();
    void act(Studio &studio);
    std::string toString() const;
    std::string getType() const;
    PrintTrainerStatus* clone();
private:
    const int trainerId;
    std::string default_str;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    PrintActionsLog(string str);
    virtual ~PrintActionsLog();
    void act(Studio &studio);
    std::string toString() const;
    std::string getType() const;
    PrintActionsLog* clone();
private:
std::string default_str;
};


class BackupStudio : public BaseAction {
public:
    BackupStudio();
    BackupStudio(string str);
    virtual ~BackupStudio();
    void act(Studio &studio);
    std::string toString() const;
    std::string getType() const;
    BackupStudio* clone();
private:
std::string default_str;
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();
    RestoreStudio(string str);
    virtual ~RestoreStudio();
    void act(Studio &studio);
    std::string toString() const;
    std::string getType() const;
    RestoreStudio* clone();
private:
std::string default_str;
};


#endif