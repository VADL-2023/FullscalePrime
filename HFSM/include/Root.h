#ifndef ROOT_H
#define ROOT_H

#include "State_Enums.h"

#include <iostream>
#include <map>
#include <ctime>
#include <string>
#include <unistd.h>
#include <thread>

class State;

class Root {
    public:
        double start_time_;
        std::map<StateName,State*> states_;

        Root();
        void addState(State* the_state);
        void execute();
        double getCurrentTime();

    private:

};
#endif