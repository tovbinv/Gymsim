#include "../include/Workout.h"
#include <iostream> 
#include <string>

using namespace std;


Workout::Workout(int w_id, string w_name, int w_price, WorkoutType w_type): id(w_id), name(w_name), price(w_price), type(w_type) {}



int Workout:: getId() const { return id; }

int Workout:: getPrice() const { return price; }

string Workout:: getName() const { return name; }

WorkoutType Workout:: getType() const { return type; }


