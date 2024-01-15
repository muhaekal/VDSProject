// A minimalistic BDD library, following Wolfgang Kunz lecture slides
//
// Created by Markus Wedler 2014

#ifndef VDSPROJECT_MANAGER_H
#define VDSPROJECT_MANAGER_H

#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <fstream>
#include <memory>
#include <set>

#include "ManagerInterface.h"

namespace ClassProject {

    struct UniqueTableEntry {
        std::string label;
        BDD_ID id;
        BDD_ID high;
        BDD_ID low;
        BDD_ID TopVar;
    };

    class Manager : public ManagerInterface {
    public:
        BDD_ID createVar(const std::string &label) override;

        const BDD_ID &True() override;

        const BDD_ID &False() override;

        bool isConstant(BDD_ID f) override;

        bool isVariable(BDD_ID x) override;

        BDD_ID topVar(BDD_ID f) override;

        BDD_ID ite(BDD_ID i, BDD_ID t, BDD_ID e) override;

        BDD_ID coFactorTrue(BDD_ID f, BDD_ID x) override;

        BDD_ID coFactorFalse(BDD_ID f, BDD_ID x) override;

        BDD_ID coFactorTrue(BDD_ID f) override;

        BDD_ID coFactorFalse(BDD_ID f) override;

        BDD_ID neg(BDD_ID a) override;

        BDD_ID and2(BDD_ID a, BDD_ID b) override;

        BDD_ID or2(BDD_ID a, BDD_ID b) override;

        BDD_ID xor2(BDD_ID a, BDD_ID b) override;

        BDD_ID nand2(BDD_ID a, BDD_ID b) override;

        BDD_ID nor2(BDD_ID a, BDD_ID b) override;

        BDD_ID xnor2(BDD_ID a, BDD_ID b) override;

        std::string getTopVarName(const BDD_ID &root) override;

        void findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) override;

        void findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) override;

        size_t uniqueTableSize() override;
        
        std::vector<UniqueTableEntry> uniqueTable;
        
        std::unordered_map<size_t, BDD_ID> uniqueTableMap;
        
        BDD_ID createNode(BDD_ID l, BDD_ID h, BDD_ID x, std::string label);
        
        static size_t keyGen(BDD_ID a, BDD_ID b, BDD_ID c);

        BDD_ID findOrAdd(BDD_ID a, BDD_ID b, BDD_ID c);

        BDD_ID TrueID;

        BDD_ID FalseID;

        BDD_ID highSuccessor(BDD_ID a);

        BDD_ID lowSuccessor(BDD_ID a);

        std::string getLabel(BDD_ID f);

        void visualizeBDD(std::string filepath, BDD_ID &root) override;

        //Constructor & destructor
       Manager();

       ~Manager();

    private:

        std::unordered_map<size_t, BDD_ID> computedTable;

        //std::map<std::tuple<BDD_ID, BDD_ID, BDD_ID>, BDD_ID> computed_table;

    };


}

#endif
