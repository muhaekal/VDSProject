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

    //Printing list of nodes in uniqueTable
    for (int i = 0; i < manager.uniqueTableSize();i++)
    {
        printf("ID %d, High %d, Low %d, Top %d, Label %s\n",
               manager.uniqueTable[i].id,
               manager.uniqueTable[i].high,
               manager.uniqueTable[i].low,
               manager.uniqueTable[i].TopVar,
               manager.uniqueTable[i].label.c_str()
               );
    }

    manager.visualizeBDD("bdd_1.dot", f);

    return 0;
}

