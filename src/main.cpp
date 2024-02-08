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

   manager.visualizeBDD("robdd.dot", f);

    ClassProject::Reachability fsm1 = ClassProject::Reachability(2);

    return 0;
}

