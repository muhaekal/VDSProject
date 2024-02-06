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

}

const std::vector<BDD_ID> &Reachability::getStates() const {
    return states;
}

const std::vector<BDD_ID> &Reachability::getInputs() const {
    return inputs;
}

void Reachability::setInitState(const std::vector<bool> &stateVector)
{
    if (stateVector.size() != states.size()) {
        std::__throw_runtime_error("Incorrect size");
    }

    initialState = stateVector;

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

}

bool Reachability::isReachable(const std::vector<bool> &stateVector) {
    if (stateVector.size() != states.size()) {
        std::__throw_runtime_error("Incorrect size");
    }

    return stateDistance(stateVector) >= 0;
}

int Reachability::stateDistance(const std::vector<bool> &stateVector) {
    if (stateVector.size() != states.size()) {
        std::__throw_runtime_error("Incorrect size");
    }

    // Compute transition relation function (tau)
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

    // Iterator for symbolic traversal algorithm
    distance = 0;
    do {
        Cr = Cr_it;

        // Compute img(S')
        BDD_ID img_sp = and2(Cr, t); //temp1
        for (size_t i = states.size() - 1; i != SIZE_MAX; i--) {
            img_sp = or2(coFactorTrue(img_sp, states.at(i)),
                         coFactorFalse(img_sp, states.at(i)));
        }
        for (size_t i = inputs.size() - 1; i != SIZE_MAX; i--) {
            img_sp = or2(coFactorTrue(img_sp, inputs.at(i)),
                         coFactorFalse(img_sp, inputs.at(i)));

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

        for (size_t i = inputs.size() - 1; i != SIZE_MAX; i--) {
            img_s = or2(coFactorTrue(img_s, inputs.at(i)),
                        coFactorFalse(img_s, inputs.at(i)));

        }

        Cr_it = or2(Cr, img_s);
        distance++;
    } 
    while(!(shannon_cofactor(Cr,stateVector,states) == True()) && Cr != Cr_it);

    BDD_ID r = shannon_cofactor(Cr,stateVector,states);

    if(r == True()) {
        return distance - 1;
    }
    else {
        return -1;
    }
} 

BDD_ID Reachability::shannon_cofactor(const BDD_ID &f, const std::vector<bool> &stateVector, const std::vector<BDD_ID> &v){
    auto temp = f;
    for (int i = stateVector.size()-1;i >= 0; i--) {
        temp = stateVector.at(i) ? coFactorTrue(temp, v.at(i)) : coFactorFalse(temp, v.at(i)); 
    }
    return temp;
}


Reachability::~Reachability() { }