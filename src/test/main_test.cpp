//
// Created by ludwig on 22.11.16.
//
#include <gtest/gtest.h>
#include "Manager.h"  // Include the header file for your Manager class
#include "Reachability.h"


struct managerTest : public ::testing::Test
{
protected:
    ClassProject::Manager manager;
    ClassProject::BDD_ID trueNode, falseNode,a,b,c;
    // This function will be called before each test
    void SetUp() override {
        // Initialize any test-specific resources
        //SetUp initializes the BDD manager and creates three variables a, b, and c.
        trueNode = manager.True();
        falseNode = manager.False();
        a = manager.createVar("a");
        b = manager.createVar("b");
        c = manager.createVar("c");
        //d = manager.createVar("d");
    }

    ClassProject::BDD_ID andNode() {
        return manager.and2(a, b);
    };
};

struct reachabilityTest : public ::testing::Test
{
    ClassProject::Reachability fsm1 = ClassProject::Reachability(2,1);

    ClassProject::Reachability fsm2 = ClassProject::Reachability(3);

    std::vector<ClassProject::BDD_ID> stateVars = fsm1.getStates();
    std::vector<ClassProject::BDD_ID> inputVars = fsm1.getInputs();

    std::vector<ClassProject::BDD_ID> stateVars2 = fsm2.getStates();
    std::vector<ClassProject::BDD_ID> inputVars2 = fsm2.getInputs();


    std::vector<ClassProject::BDD_ID> transitionFunctions;
};

TEST_F(reachabilityTest, isReachableTest) {
  auto s0 = stateVars.at(0);
  auto s1 = stateVars.at(1);
  auto x0 = inputVars.at(0);

  // Counter up to 3: 00, 01, 10 and reset to 00
  transitionFunctions.push_back(fsm1.and2(x0, fsm1.ite(s1, fsm1.False(), fsm1.neg(s0))));
  transitionFunctions.push_back(fsm1.and2(x0, fsm1.and2(s0, fsm1.neg(s1))));

  fsm1.setTransitionFunctions(transitionFunctions);
  fsm1.setInitState({false, false});

  ASSERT_TRUE(fsm1.isReachable({false, false}));
  ASSERT_TRUE(fsm1.isReachable({false, true}));
  ASSERT_TRUE(fsm1.isReachable({true, false}));
  ASSERT_FALSE(fsm1.isReachable({true, true}));
}
TEST_F(reachabilityTest, stateDistanceTest) {
  auto s0 = stateVars.at(0);
  auto s1 = stateVars.at(1);

  transitionFunctions.push_back(fsm1.neg(s0)); // s0' = not(s0)
  transitionFunctions.push_back(fsm1.neg(s1)); // s1' = not(s1)

  fsm1.setTransitionFunctions(transitionFunctions);
  fsm1.setInitState({false, false});

  ASSERT_EQ(fsm1.stateDistance({false, false}), 0);
  ASSERT_EQ(fsm1.stateDistance({true, false}), -1);
  ASSERT_EQ(fsm1.stateDistance({false, true}), -1);
  ASSERT_EQ(fsm1.stateDistance({true, true}), 1);
}

TEST_F(reachabilityTest, StateDistance3StatesTest) {
  auto s0 = stateVars2.at(0);
  auto s1 = stateVars2.at(1);
  auto s2 = stateVars2.at(2);

  transitionFunctions.push_back(fsm2.neg(s0));  // s0'
  transitionFunctions.push_back(fsm2.ite(s0, fsm2.neg(s1), s1));  // s1'
  transitionFunctions.push_back(fsm2.ite(fsm2.and2(s1, s0), fsm2.neg(s2), s2));  // s2'
  
  fsm2.setTransitionFunctions(transitionFunctions);

  fsm2.setInitState({false, false, false});

  ASSERT_EQ(fsm2.stateDistance({false, false, false}), 0);
  ASSERT_EQ(fsm2.stateDistance({true, false, false}), 1);
  ASSERT_EQ(fsm2.stateDistance({false, true, false}), 2);
  ASSERT_EQ(fsm2.stateDistance({true, true, false}), 3);
  ASSERT_EQ(fsm2.stateDistance({false, false, true}), 4);
  ASSERT_EQ(fsm2.stateDistance({true, false, true}), 5);
  ASSERT_EQ(fsm2.stateDistance({false, true, true}), 6);
  ASSERT_EQ(fsm2.stateDistance({true, true, true}), 7);
}

TEST_F(reachabilityTest, ExceptionsTest) {
  auto s0 = stateVars.at(0);
  auto s1 = stateVars.at(1);

  transitionFunctions.push_back(fsm1.neg(s0));
  transitionFunctions.push_back(fsm1.neg(s0));
  transitionFunctions.push_back(fsm1.ite(s0, fsm1.neg(s1), s1));

  EXPECT_THROW(fsm1.setTransitionFunctions(transitionFunctions), std::runtime_error);
  EXPECT_THROW(fsm1.setInitState({false, false, true}), std::runtime_error);
  EXPECT_THROW(fsm1.isReachable({true, true, true}), std::runtime_error);
  EXPECT_THROW(fsm1.stateDistance({true, true, true}), std::runtime_error);
}


// Test that the True and False nodes exist and have correct properties
TEST_F(managerTest, TrueAndFalseNodes) {
    // Obtain the IDs of the True and False nodes

    // Test that the True node has the expected properties
    ASSERT_EQ(manager.topVar(trueNode), trueNode) << "Top variable of True node should be itself";
    ASSERT_EQ(manager.getLabel( manager.True()), "True") << "Label of True node should be 'True'";

    // Test that the False node has the expected properties
    ASSERT_EQ(manager.topVar(falseNode), falseNode) << "Top variable of False node should be itself";
    ASSERT_EQ(manager.getLabel(manager.False()), "False") << "Label of False node should be 'False'";

    // Test that the True and False nodes are different
    ASSERT_NE(trueNode, falseNode) << "True and False node IDs should be different";
}

TEST_F(managerTest, CreateVarTest) {

/*
    ClassProject::BDD_ID ID_a = manager.createVar("a");
    ClassProject::BDD_ID ID_b = manager.createVar("b");
    ClassProject::BDD_ID ID_c = manager.createVar("c");*/

    EXPECT_EQ(a, 2);
    EXPECT_EQ(b, 3);
    EXPECT_EQ(c, 4);

}
//We make sure that the size of the unique table in the BDD manager increases when a new variable is created.
TEST_F(managerTest, UniqueTableSizeIncreasesAfterCreatingVar) {
    /*ClassProject::BDD_ID testVar1 = manager.createVar("testVar1");
    size_t sizeAfterFirstVar = manager.uniqueTableSize();
    manager.createVar("testVar2");
    //the ASSERT_GT macro is used for comparison testing to assert that the first argument is greater than the second argument
    ASSERT_GT(manager.uniqueTableSize(), sizeAfterFirstVar) << "Unique table size should increase after creating a new variable";*/
    size_t initialSize = manager.uniqueTableSize();
    manager.createVar("test_var");
    ASSERT_EQ(manager.uniqueTableSize(), initialSize + 1);
}

//Returns true if the given ID represents a leaf node
TEST_F(managerTest, isconstantTest)
{
    // Create a set of known constant IDs
    //size_t tableSize = manager.uniqueTableSize();
    for (ClassProject::BDD_ID i = 0; i < manager.uniqueTableSize(); ++i)
    {
        // a boolean variable out is declared and set to true if the current i is equal to the ID of the False node or the True node in the manager's unique table.
        bool out = (i == manager.False() || i == manager.True());
        //If the assertion fails, it outputs the message "Node with ID [i] should be constant".
        ASSERT_EQ(manager.isConstant(i), out) << "Node with ID " << i << " should be constant";
    }
}

TEST_F(managerTest, isvariableTest)
{
    for (ClassProject::BDD_ID i = 0; i < manager.uniqueTableSize(); ++i)
    {
        // a boolean variable out is declared and set to true if the current i is equal to the ID of the variables we declared up there.
        bool out = (i == a || i == b || i == c);
        //If the assertion fails, it outputs the message "Node with ID [i] should be a variable".
        ASSERT_EQ(manager.isVariable(i), out) << "Node with ID " << i << " should be a variable";
    }

}

//hash table key(top, low, high)
//(1,1,1) is the true node and the node that is gonna be vreated after that, will take the key =2.
TEST_F(managerTest, keyGenTest) {
    /*
    ClassProject::BDD_ID d = manager.keyGen(1,1,1);

    EXPECT_EQ(d, 2);*/
// Example values for testing
    ClassProject::BDD_ID a = 2, b = 3, c = 4;
    ClassProject::BDD_ID d = 5, e = 6, f = 7;

    // Generate two different keys
    size_t key1 = manager.keyGen(a, b, c);
    size_t key2 = manager.keyGen(d, e, f);

    // Assert that the keys are unique, not equal
    ASSERT_NE(key1, key2) << "Keys should be unique for different inputs.";

    // Assert that the same inputs produce the same key
    ASSERT_EQ(key1, manager.keyGen(a, b, c)) << "Same inputs should produce the same key.";
}
TEST_F(managerTest, iteTest)
{
    // Test ite when the first argument is a constant
    ASSERT_EQ(manager.ite(trueNode, a, b), a); // i is constant True, should return t
    ASSERT_EQ(manager.ite(falseNode, a, b), b); // i is constant False, should return e

    // Test ite when then and else branches are the same
    ASSERT_EQ(manager.ite(a, b, b), b); // t and e are the same, should return t (or e)

    // Test ite when it retrieves a result from the computed table
    // Assuming the operation was previously computed and stored
    ClassProject::BDD_ID result = manager.ite(a, trueNode, falseNode);
    ASSERT_EQ(manager.ite(a, trueNode, falseNode), result);

    // Test ite with a non-constant condition and different then/else branches
    result = manager.ite(a, b, trueNode);
    // Check that the result is a valid BDD ID and not the same as any of the inputs
    ASSERT_NE(result, a);
    ASSERT_NE(result, b);
    ASSERT_NE(result, trueNode);
    ASSERT_NE(result, falseNode);
}

TEST_F(managerTest, coFactorTrueTest)
{
//Test coFactorTrue with a constant function
    ASSERT_EQ(manager.coFactorTrue(manager.True(), a), manager.True());
    ASSERT_EQ(manager.coFactorTrue(manager.False(), a), manager.False());

// Assuming a is the top variable of some expression
    ClassProject::BDD_ID f = manager.and2(a, b); // f has a as top variable
    ASSERT_EQ(manager.coFactorTrue(f, a), manager.highSuccessor(f));
}

TEST_F(managerTest, coFactorFalseTest)
{
// Test coFactorFalse with a constant function
    ASSERT_EQ(manager.coFactorFalse(manager.True(), a), manager.True());
    ASSERT_EQ(manager.coFactorFalse(manager.False(), a), manager.False());

    ClassProject::BDD_ID f = manager.and2(a, b); // f has a as top variable
    ASSERT_EQ(manager.coFactorFalse(f, a), manager.uniqueTable[f].low);
}

TEST_F(managerTest, negTest)
{
    //negation for constant variables
    ASSERT_EQ(manager.neg(manager.True()), manager.False());
    ASSERT_EQ(manager.neg(manager.False()), manager.True());

    //negation for nonconstant variables. In ite we change the low and high successors
    ASSERT_EQ(manager.neg(a), manager.ite(a, manager.False(), manager.True()));
}

TEST_F(managerTest, and2Test)
{
    ClassProject::BDD_ID f = manager.and2(a,b);
    ASSERT_EQ(manager.highSuccessor(f), b);
    ASSERT_EQ(manager.lowSuccessor(f), manager.False());
}
TEST_F(managerTest, or2Test)
{
    ClassProject::BDD_ID f = manager.or2(a,b);
    ASSERT_EQ(manager.highSuccessor(f), manager.True());
    ASSERT_EQ(manager.lowSuccessor(f),b);
}
TEST_F(managerTest, xor2Test)
{
    ClassProject::BDD_ID f = manager.xor2(a,b);
    ASSERT_EQ(manager.highSuccessor(f), manager.neg(b));
    ASSERT_EQ(manager.lowSuccessor(f), b);
}
TEST_F(managerTest, nand2Test)
{
    ClassProject::BDD_ID f = manager.nand2(a,b);
    ASSERT_EQ(manager.highSuccessor(f), manager.neg(b));
    ASSERT_EQ(manager.lowSuccessor(f), manager.True());
}
TEST_F(managerTest, nor2Test)
{
    ClassProject::BDD_ID f = manager.nor2(a,b);
    ASSERT_EQ(manager.highSuccessor(f), manager.False());
    ASSERT_EQ(manager.lowSuccessor(f),manager.neg(b));
}
TEST_F(managerTest, xnor2Test)
{
    ClassProject::BDD_ID f = manager.xnor2(a,b);
    ASSERT_EQ(manager.lowSuccessor(f), manager.neg(b));
    ASSERT_EQ(manager.highSuccessor(f), b);
}

TEST_F(managerTest, topVarTest)
{
    andNode();
    //ClassProject::BDD_ID f = manager.and2(a,b);
    //checks whether the top variable ID for the BDD node with ID 5 is indeed 3.
    ASSERT_EQ(manager.topVar(5), 2);
    // checks whether the top variable name for the BDD node with ID 5 is "a".
    ASSERT_EQ(manager.getTopVarName(5), "a");
}
TEST_F(managerTest, GetTopVarNameTest) {
    ClassProject::BDD_ID f = manager.and2(a,b);
    //ASSERT_EQ(manager.topVar(b), b) << "Top variable of 'b' should be 'b' itself.";
    ASSERT_EQ(manager.getTopVarName(f), "a");
}

/*TEST_F(managerTest, findNodesTest)
{
    ClassProject::BDD_ID f = manager.and2(a,b);
    std::set<ClassProject::BDD_ID> nodes;
    manager.findNodes(f, nodes);

    // Check if the set contains exactly 5 nodes.
    // This includes a, b, f, TRUE, and FALSE nodes.
    ASSERT_EQ(nodes.size(), 5);

    // Additionally, you can check if the known nodes are indeed in the set.
    ASSERT_TRUE(nodes.find(a) != nodes.end());
    ASSERT_TRUE(nodes.find(b) != nodes.end());
    ASSERT_TRUE(nodes.find(f) != nodes.end());
}

TEST_F(managerTest, findVarsTest)
{

}*/

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}