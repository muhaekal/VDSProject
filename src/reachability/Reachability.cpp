#include "Reachability.h"

#include <algorithm>

using namespace ClassProject;

Reachability::Reachability(unsigned int stateSize, unsigned int inputSize) :
ReachabilityInterface::ReachabilityInterface(stateSize, inputSize)
{
    if (stateSize == 0) {
        std::__throw_runtime_error("Size cannot be zero");
    }

    for (unsigned int i = 0; i < stateSize; i++ ) {
        states.push_back(createVar("s" + std::to_string(i)));
        next_states.push_back(createVar("s'" + std::to_string(i)));
    }

    for (unsigned int i = 0; i < inputSize; i++ ) {
        inputs.push_back(createVar("x" + std::to_string(i)));
    }


    //default transition function is the identity function
    transitions = states;

    changed = true;
}

void Reachability::setInitState(const std::vector<bool> &stateVector)
{
    if (stateVector.size() != states.size()) {
        std::__throw_runtime_error("Incorrect size");
    }

    initialState = stateVector;

    changed = true;
}

void Reachability::setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions)
{
    //exception if the number of given transition functions does not match the number of state bits
    if (transitionFunctions.size() != states.size()) {
        std::__throw_runtime_error("Incorrect size");
    }

    transitions = transitionFunctions;

    //check all transition BDD_IDs are known
    for(size_t i = 0; i < transitions.size(); i++) {
        if(transitions.at(i) >= uniqueTableSize()) {
            std::__throw_runtime_error("BDD_IDs is unknown for transition function");
        }
    }

    changed = true;
}

void Reachability::compute_reachable_states()
{
    // Compute transition relation (tau)
    BDD_ID t = or2(and2(next_states.at(0),transitions.at(0)),
                   nor2(next_states.at(0),transitions.at(0)));

    for (size_t i = 1; i < states.size(); i++) {
        t = and2(t, or2(and2(next_states.at(i),transitions.at(i)),
                        nor2(next_states.at(i),transitions.at(i))));
    }


    // Compute characteristic function of initial state
    BDD_ID Cr;
    if (initialState.at(0)) {
        Cr = xnor2(states.at(0), True());
    }
    else {
        Cr = xnor2(states.at(0), False());
    }

    for (size_t i = 1; i < states.size(); i++) {
        if (initialState.at(i)) {
            Cr = and2(Cr,xnor2(states.at(i), True()));
        }
        else {
            Cr = and2(Cr,xnor2(states.at(i), False()));
        }
    }

    BDD_ID Cr_it = Cr;

    // Iterator for characteristic function
    //int k = 0;
    do {
        Cr = Cr_it;
        // Compute img(S')
        BDD_ID img_sp = and2(Cr, t); //temp1
        for (size_t i = states.size() - 1; i != SIZE_MAX; i--) {
            img_sp = or2(coFactorTrue(img_sp, states.at(i)),
                         coFactorFalse(img_sp, states.at(i)));
        }

        // Compute img(S)
        BDD_ID img_s = and2(xnor2(states.at(0), next_states.at(0)),img_sp);//temp1 = img_sp * s0
        for (size_t i = 1; i < states.size(); i++) {
            img_s = and2(img_s, xnor2(states.at(i), next_states.at(i))); //temp1 * s1 *...
        }

        for (size_t i = states.size() - 1; i != SIZE_MAX; i--) {
            img_s = or2(coFactorTrue(img_s, next_states.at(i)),
                        coFactorFalse(img_s, next_states.at(i)));
        }

        Cr_it = or2(Cr, img_s);

    } while(Cr != Cr_it);

   /* for (auto node : uniqueTable) {
        std::cout <<"ID "<< node.id <<" High " << node.high <<" Low "<< node.low
        <<" Top "<< node.TopVar<<" Label "<<node.label<< std::endl;
    }; */

    // Existential quantification w.r.t inputs
    std::set<BDD_ID> vars;
    findVars(Cr, vars);
    for (auto v : vars) {
        if (std::find(inputs.begin(), inputs.end(), v) != inputs.end()) {
            Cr = or2(coFactorTrue(Cr,v),coFactorFalse(Cr,v));
            //std::cout <<"1 input removed"<<std::endl;
            //std::cout << "Cr & Input ID" <<" "<<uniqueTable[Cr].id <<" "<<  uniqueTable[v].id<<std::endl;
        }


    }

    characteristic_function = Cr;
    changed = false;
}

const std::vector<BDD_ID> &Reachability::getStates() const {
    return states;
}

const std::vector<BDD_ID> &Reachability::getInputs() const {
    return inputs;
}

bool Reachability::isReachable(const std::vector<bool> &stateVector) {
    if (stateVector.size() != states.size()) {
        std::__throw_runtime_error("Incorrect size");
    }

    if (changed) {
        compute_reachable_states();
    }

    BDD_ID r = characteristic_function;

    //Existential Quantification w.r.t states
    std::cout <<std::endl;
    for (size_t i = 0; i < states.size(); i++) {
        //size_t j;
        if (stateVector.at(i)) {
            r = coFactorTrue(r,states.at(i));
           // std::cout << i << " true" <<" "<<uniqueTable[r].id <<" "<< uniqueTable[r].label <<std::endl;
        }
        else {
            r = coFactorFalse(r,states.at(i));
           // std::cout << i << " false" <<" "<<uniqueTable[r].id <<" " <<uniqueTable[r].label <<std::endl;
        }
    }

    return (r == True());
}

int Reachability::stateDistance(const std::vector<bool> &stateVector) {

    // Compute transition relation (tau)
    BDD_ID t = or2(and2(next_states.at(0),transitions.at(0)),
                   nor2(next_states.at(0),transitions.at(0)));

    for (size_t i = 1; i < states.size(); i++) {
        t = and2(t, or2(and2(next_states.at(i),transitions.at(i)),
                        nor2(next_states.at(i),transitions.at(i))));
    }

    // Compute characteristic function of initial state
    BDD_ID Cr;
    if (initialState.at(0)) {
        Cr = xnor2(states.at(0), True());
    }
    else {
        Cr = xnor2(states.at(0), False());
    }

    for (size_t i = 1; i < states.size(); i++) {
        if (initialState.at(i)) {
            Cr = and2(Cr,xnor2(states.at(i), True()));
        }
        else {
            Cr = and2(Cr,xnor2(states.at(i), False()));
        }
    }

    BDD_ID Cr_it = Cr;

    //Compute characteristic function of the desired state
    BDD_ID D;

    if (stateVector.at(0)) {
        D = xnor2(states.at(0), True());
    }
    else {
        D = xnor2(states.at(0), False());
    }

    for (size_t i = 1; i < states.size(); i++) {
        if (stateVector.at(i)) {
            D = and2(D,xnor2(states.at(i), True()));
        }
        else {
            D = and2(D,xnor2(states.at(i), False()));
        }
    }

    // Iterator to get to the desired state
    int j = 0;
    while (Cr_it != D)
    {
        Cr = Cr_it;
        // Compute img(S')
        BDD_ID img_sp = and2(Cr, t); //temp1
        for (size_t i = states.size() - 1; i != SIZE_MAX; i--) {
            img_sp = or2(coFactorTrue(img_sp, states.at(i)),
                         coFactorFalse(img_sp, states.at(i)));
        }

        // Compute img(S)
        BDD_ID img_s = and2(xnor2(states.at(0), next_states.at(0)),img_sp);//temp1 = img_sp * s0
        for (size_t i = 1; i < states.size(); i++) {
            img_s = and2(img_s, xnor2(states.at(i), next_states.at(i))); //temp1 * s1 *...
        }

        for (size_t i = states.size() - 1; i != SIZE_MAX; i--) {
            img_s = or2(coFactorTrue(img_s, next_states.at(i)),
                        coFactorFalse(img_s, next_states.at(i)));
        }

        Cr_it = img_s;
        j++;
        if (j > std::pow(2,states.size())) {
            return -1;
        }
    }
    return j;
    /*std::set<BDD_ID> vars;
    findVars(Cr, vars);
    for (auto v : vars) {
        if (std::find(inputs.begin(), inputs.end(), v) != inputs.end()) {
            Cr = or2(coFactorTrue(Cr,v),coFactorFalse(Cr,v));
        }
    }

    for (size_t i = 0; i < states.size(); i++) {
        //size_t j;
        if (stateVector.at(i)) {
            Cr = coFactorTrue(Cr,states.at(i));
            //std::cout << i << " true" <<" "<<uniqueTable[r].id <<" "<< uniqueTable[r].label <<std::endl;
        }
        else {
            Cr = coFactorFalse(Cr,states.at(i));
            //std::cout << i << " false" <<" "<<uniqueTable[r].id <<" " <<uniqueTable[r].label <<std::endl;
        }
    }
    if (Cr = True()){*/
}


Reachability::~Reachability() { }