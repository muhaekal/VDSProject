#include <gtest/gtest.h>
#include "Manager.h"  // Include the header file for your Manager class

// Define a test fixture for the Manager class
class ManagerTest : public ::testing::Test {
protected:
    ClassProject::Manager manager;  // Create an instance of Manager class

    // This function will be called before each test
    void SetUp() override {
        // Initialize any test-specific resources
    }

    // This function will be called after each test
    void TearDown() override {
        // Clean up any test-specific resources
    }

    // You can declare additional member functions or variables here
};

// Define a test case for the createNode function
TEST_F(ManagerTest, CreateNodeTest) {


    ClassProject::BDD_ID ID_a = manager.createNode(0, 1, 2, "a");
 
    EXPECT_EQ(manager.uniqueTableSize(), 3);
    EXPECT_EQ(manager.uniqueTableMap.size(), 3);
    EXPECT_EQ(ID_a, 2);
    
}

TEST_F(ManagerTest, CreateVarTest) {
    

    ClassProject::BDD_ID ID_a = manager.createVar("a");
    ClassProject::BDD_ID ID_b = manager.createVar("b");
 
    EXPECT_EQ(ID_a, 2);
    EXPECT_EQ(ID_b, 3);
    
}

TEST_F(ManagerTest, keyGenTest) {
   

    size_t d = manager.keyGen(1,1,1);
 
    EXPECT_EQ(d, 4398048608257);
    //EXPECT_EQ(ID_b, 3);
    
}

// define additional test cases as needed

// Entry point for running the tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

