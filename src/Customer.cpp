
#include "../include/Customer.h"
#include <string>
#include<vector>
#include<algorithm>
using namespace std;

    Customer::Customer(std::string name, int id): name(name), id(id) {}

    string Customer::getName() const
    {
        return name;
    }
    int Customer:: getId() const
    {
        return id;
    }





    SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(name, id){}

    SweatyCustomer::~SweatyCustomer()=default;

    vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options)
    {
        vector<int> toReturn;
        for (Workout i :workout_options)
        {
            if (i.getType()==2)
            toReturn.push_back(i.getId());
        }
        return toReturn;
    }

    string SweatyCustomer:: toString() const{
        return "sweaty";
    }





    CheapCustomer::CheapCustomer(std::string name, int id):Customer(name,id) {}

    CheapCustomer::~CheapCustomer()=default;

    vector<int>CheapCustomer:: order(const std::vector<Workout> &workout_options) 
    {
        vector <int> workout_order_list;
        if(workout_options.empty()==false){
            int min_price=workout_options.at(0).getPrice();
            int workout_id(workout_options.at(0).getId());
            for(int i=1;i<(int)workout_options.size();i++)
            {
                if(workout_options.at(i).getPrice()<min_price)
                {
                    min_price=(workout_options.at(i).getPrice());
                    workout_id=(workout_options.at(i).getId());
                }
            }
            workout_order_list.push_back(workout_id);
        }
        return workout_order_list;
    }

    string CheapCustomer::toString() const
    {
        return "cheap";
    }




    HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id):Customer(name,id) {}

    HeavyMuscleCustomer::~HeavyMuscleCustomer()=default;

    vector<int>HeavyMuscleCustomer:: order(const std::vector<Workout> &workout_options)
    { 
        vector<int> workout_order_list;
        vector<int> prices;
        for(int i=0;i<(int)workout_options.size();i++)
        {
            if(workout_options.at(i).getType()==0)
            {
                int price=workout_options.at(i).getPrice();
                prices.push_back(price);
            }
        }
            sort(prices.begin(),prices.end());
            while(prices.size() > 0)
            {
                bool stop=false;
                int i=0;
                int j=(int)prices.size()-1;
                while(i<(int)workout_options.size() && stop==false)
                {
                    if(workout_options.at(i).getPrice()==prices.at(j) && workout_options.at(i).getType() == 0)
                    {
                        workout_order_list.push_back(workout_options.at(i).getId());
                        stop=true;
                        prices.pop_back();
                    }
                    i=i+1;
                }
            }

        
        return workout_order_list;
    }

    string HeavyMuscleCustomer:: toString() const
    {
        return "heavy";
    }




    FullBodyCustomer::FullBodyCustomer(std::string name, int id): Customer(name,id) {}

    FullBodyCustomer::~FullBodyCustomer()=default;
    
    vector<int>FullBodyCustomer:: order(const std::vector<Workout> &workout_options) 
    {

       vector <int> workout_order_list;
    
        int min_cardio(-1);
        int min_cardio_id(-1);

        int max_mix(-1);
        int max_mix_id(-1);

        int min_anaerobic(-1);
        int min_anaerobic_id(-1);

        for(int i=0;i<(int)workout_options.size();i++)
        {
            if(workout_options.at(i).getType()==2)
            {
                if(min_cardio == -1)
                {
                    min_cardio = workout_options.at(i).getPrice();
                    min_cardio_id = workout_options.at(i).getId();
                }
                else if(workout_options.at(i).getPrice() < min_cardio) 
                {
                    min_cardio = workout_options.at(i).getPrice();
                    min_cardio_id = workout_options.at(i).getId();
                    //min_cardio_id = i;
                }
            }
            if(workout_options.at(i).getType() == 1)
            {
                if(max_mix == -1)
                {
                    max_mix = workout_options.at(i).getPrice();
                    max_mix_id = workout_options.at(i).getId();
                }
                if(workout_options.at(i).getPrice() > max_mix) 
                {
                    max_mix = workout_options.at(i).getPrice();
                    max_mix_id = workout_options.at(i).getId();
                }
            }
            if(workout_options.at(i).getType() == 0)
            {
                if(min_anaerobic == -1)
                {
                    min_anaerobic = workout_options.at(i).getPrice();
                    min_anaerobic_id = workout_options.at(i).getId() ;
                }
                if(workout_options.at(i).getPrice() < min_anaerobic) 
                {
                    min_anaerobic=workout_options.at(i).getPrice();
                    min_anaerobic=workout_options.at(i).getId();
                }
            }
        }
        if(min_cardio_id!=-1) workout_order_list.push_back(min_cardio_id); 
        if(max_mix_id!=-1) workout_order_list.push_back(max_mix_id); 
        if(min_anaerobic_id!=-1)  workout_order_list.push_back(min_anaerobic_id); 
           
        return workout_order_list;
          

       
    }
    string FullBodyCustomer::toString() const{
        return "fullbody";
    }
