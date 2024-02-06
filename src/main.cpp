//
// Created by Carolina P. Nogueira 2016
//

//VDS Class Project Main.cpp

#include <iostream>
#include <string>
#include <stdio.h>
#include "Manager.h"
#include "reachability/Reachability.h"

//using ClassProject::Manager;


int main(int argc, char* argv[])
{
    ClassProject::Manager manager;

    ClassProject::BDD_ID a = manager.createVar("a");
    ClassProject::BDD_ID b = manager.createVar("b");
    ClassProject::BDD_ID c = manager.createVar("c");
    ClassProject::BDD_ID d = manager.createVar("d");
    ClassProject::BDD_ID f = manager.and2(manager.or2(a, b), manager.and2(c, d));
    //ClassProject::BDD_ID f = manager.or2(a, manager.and2(b, c));

    /*for (auto node : manager.uniqueTable) {
        std::cout <<"ID "<< node.id <<" High " << node.high <<" Low "<< node.low
        <<" Top "<< node.TopVar<<" Label "<<node.label<< std::endl;
    }; */


   manager.visualizeBDD("robdd.dot", f);

    ClassProject::Reachability fsm1 = ClassProject::Reachability(2,1);

    std::vector<ClassProject::BDD_ID> stateVars2 = fsm1.getStates();
    std::vector<ClassProject::BDD_ID> inputVars2 = fsm1.getInputs();
    std::vector<ClassProject::BDD_ID> transitionFunctions;

    ClassProject::BDD_ID s0 = stateVars2.at(0);
    ClassProject::BDD_ID s1 = stateVars2.at(1);

    ClassProject::BDD_ID x0 = inputVars2.at(0);
    //ClassProject::BDD_ID x1 = inputVars2.at(1);


    //Transitions without inputs
    //transitionFunctions.push_back(fsm1.neg(s1)); // s0' = not(s0)
    //transitionFunctions.push_back(s0); // s1' = not(s1)

    //Transitions with inputs
    transitionFunctions.push_back(fsm1.and2(x0,fsm1.ite(s1,fsm1.False(),fsm1.neg(s0))));
    transitionFunctions.push_back(fsm1.and2(x0,fsm1.and2(s0,fsm1.neg(s1))));
    

    fsm1.setTransitionFunctions(transitionFunctions);

    fsm1.setInitState({false,false});

    bool a1 = fsm1.isReachable({false, false});
    bool b1 = fsm1.isReachable({true, false});
    bool c1 = fsm1.isReachable({false, true});
    bool d1 = fsm1.isReachable({true, true});

    std::cout <<std::endl;
    std::cout <<" isReachable({false, false}) = "<< a1 << std::endl;
    std::cout <<" isReachable({true, false}) = " << b1 << std::endl;
    std::cout <<" isReachable({false, true}) = "<< c1 << std::endl;
    std::cout <<" isReachable({true, true}) = "<< d1 << std::endl;
    std::cout <<std::endl;
    int a2 = fsm1.stateDistance({false, false});
    std::cout <<" stateDistance({false, false}) = "<< a2 << std::endl;
    int b2 = fsm1.stateDistance({true, false});
    std::cout <<" stateDistance({true, false}) = "<< b2 << std::endl;
    int c2 = fsm1.stateDistance({false, true});
    std::cout <<" stateDistance({false, true}) = "<< c2 << std::endl;
    int d2 = fsm1.stateDistance({true, true});
    std::cout <<" stateDistance({true, true}) = "<< d2 << std::endl;

    return 0;
}

