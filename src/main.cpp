//
// Created by Carolina P. Nogueira 2016
//

//VDS Class Project Main.cpp

#include <iostream>
#include <string>
#include <stdio.h>
#include "Manager.h"

//using ClassProject::Manager;


int main(int argc, char* argv[])
{
    ClassProject::Manager manager;

    ClassProject::BDD_ID a = manager.createVar("a");
    ClassProject::BDD_ID b = manager.createVar("b");
    ClassProject::BDD_ID c = manager.createVar("c");
    ClassProject::BDD_ID d = manager.createVar("d");
    ClassProject::BDD_ID f = manager.and2(manager.or2(a, b), manager.and2(c, d));

    for (auto node : manager.uniqueTable) {
        std::cout <<"ID "<< node.id <<" High " << node.high <<" Low "<< node.low
        <<" Top "<< node.TopVar<<" Label "<<node.label<< std::endl;
    };

    manager.visualizeBDD("robdd.dot", f);

    return 0;
}

