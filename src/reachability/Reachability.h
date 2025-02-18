#ifndef VDSPROJECT_REACHABILITY_H
#define VDSPROJECT_REACHABILITY_H

#include "ReachabilityInterface.h"

namespace ClassProject {

    class Reachability : public ReachabilityInterface {

    public:

        const std::vector<BDD_ID> &getStates() const override;

        const std::vector<BDD_ID> &getInputs() const override;

        bool isReachable(const std::vector<bool> &stateVector) override;

        int stateDistance(const std::vector<bool> &stateVector) override;

        void setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions) override;

        void setInitState(const std::vector<bool> &stateVector) override;

        explicit Reachability(unsigned int stateSize, unsigned int inputSize = 0);

        ~Reachability();



    private:

        std::vector<BDD_ID> states;

        std::vector<BDD_ID> next_states;

        std::vector<BDD_ID> inputs;

        std::vector<BDD_ID> transitions;

        std::vector<bool> initialState;

        BDD_ID characteristic_function;

        int distance;

        BDD_ID shannon_cofactor(const BDD_ID &f, const std::vector<bool> &stateVector, const std::vector<BDD_ID> &v);

    };

}
#endif
