#include <vector>
#include "..//include/Trainer.h"
#include<algorithm>
#include <iostream>
using namespace std;

typedef std::pair<int, Workout> OrderPair;

    Trainer::Trainer(int t_capacity): 
    capacity(t_capacity),open(false),salary(0),customersList(), orderList()
    {
        

    } 

    int Trainer::getCapacity() const{
        return capacity;
    }

    void Trainer:: addCustomer(Customer* customer)
    {
        if((int)customersList.size() < capacity){ //if size> capacity?
            customersList.push_back(customer);
        }
    }

    void Trainer:: removeCustomer(int id){
        for (int i = 0; i<(int)customersList.size(); i++)
        {
            if (customersList[i]->getId() == id) 
            {
                // need to delete customer
                delete customersList[i];
                customersList.erase(customersList.begin()+i);
                for (int i = 0; i<(int)orderList.size();i++)
                {
                    if (orderList[i].first == id)
                    {
                        orderList[i].first = -1;
                        salary = salary - orderList[i].second.getPrice();
                    }
                }
               
            }   
        }
    }
     

    Customer* Trainer:: getCustomer(int id)
    {
        for (int i = 0; i<(int)customersList.size(); i++){
            if (customersList[i]->getId() == id) {
                return customersList[i];
            }
        }
        return nullptr;
    }

    std::vector<Customer*>& Trainer:: getCustomers()
    {
        return customersList;
    }

    std::vector<OrderPair>& Trainer:: getOrders()
    {
        return orderList;
    }

    void Trainer:: order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options)
    {
      
        for (int m : workout_ids) 
        {
            orderList.push_back(make_pair(customer_id, workout_options[m]));
            salary = salary + workout_options[m].getPrice();
        }
        
    }




    void Trainer:: openTrainer()
    {
        open = true;
    }
    void Trainer:: closeTrainer()
    { // delete open workouts of the trainer? go to customer and change something?
        open = false;//need to print salary,remove all his customers,andd all the orders pair
        for (int i = 0; i <(int)customersList.size(); i++)
        {
            delete(customersList[i]);
            customersList[i] = nullptr;
        }
        customersList.clear();

       
        orderList.clear();
    }
    int Trainer::getSalary()
    {
        return salary;
    }
    bool Trainer::isOpen()
    {
        return open;
    }
    bool Trainer::workoutComp(const OrderPair& left, const OrderPair& right)
    {
        return (left.first < right.first);
    }

    bool Trainer::customerComp(const Customer* left,const Customer* right)
    {
        return(left->getId() < right->getId());
    }

    void Trainer::clear()
    {
        for (Customer* i :customersList)
        {
            delete (i);
        }
        customersList.clear();
        orderList.clear();
    };



    //rule of 5

    Trainer::~Trainer()
    {
        clear();
    }

    Trainer::Trainer(const Trainer& T): capacity(T.capacity), open(T.open), salary(T.salary),
        customersList(T.customersList.size()),orderList()
    {
            for (int i = 0; i <(int)T.customersList.size(); i++)
            { 
                int id = T.customersList[i]->getId();
                string name = T.customersList[i]->getName();
                //use toString func of customer to determine what customer in index i
                if (!T.customersList[i]->toString().compare("sweaty"))
                customersList[i] = new SweatyCustomer(name,id);

                else if (!T.customersList[i]->toString().compare("cheap"))
                customersList[i] = new CheapCustomer(name,id);

                else if (!T.customersList[i]->toString().compare("heavy"))
                customersList[i] = new HeavyMuscleCustomer(name,id);

                else 
                customersList[i] = new FullBodyCustomer(name,id);
            }
            for (int i=0; i<(int)T.orderList.size(); i++)
            {
                int id = T.orderList[i].first;
                Workout workout = T.orderList[i].second;
                orderList.push_back(make_pair(id,workout));
            }        
    }

    Trainer& Trainer::operator=(const Trainer& T)
    {
        if (this == &T)
            return *this;
        
        capacity = T.capacity;
        open = T.open;
        salary = T.salary;
        clear();

        for (int i = 0; i <(int)T.customersList.size(); i++)
        { //copying customers list
            int id = T.customersList[i]->getId();
            string name = T.customersList[i]->getName();
            //use toString func of customer to determine what customer in index i
            if (!T.customersList[i]->toString().compare("sweaty"))
            customersList[i] = new SweatyCustomer(name,id);

            else if (!T.customersList[i]->toString().compare("cheap"))
            customersList[i] = new CheapCustomer(name,id);

            else if (!T.customersList[i]->toString().compare("heavy"))
            customersList[i] = new HeavyMuscleCustomer(name,id);

            else 
            customersList[i] = new FullBodyCustomer(name,id);
        }
            


        for (int i=0; i<(int)T.orderList.size(); i++)
        {
            int id = T.orderList[i].first;
            Workout workout = T.orderList[i].second;
            orderList.push_back(make_pair(id,workout));
        }        
        return *this;
    }

    Trainer::Trainer(Trainer&& T):
    capacity(T.capacity), open(T.open), salary(T.salary),customersList(),orderList() 
    {
        customersList = move(T.customersList);
        orderList = move(T.orderList);
    }

    Trainer& Trainer::operator=(Trainer&& T)
    {
        if (this == &T)
            return *this;

        clear();
        capacity = T.capacity;
        open = T.open;
        salary = T.salary;
        customersList = move(T.customersList);
        orderList = move(T.orderList);

        return *this;
    }
