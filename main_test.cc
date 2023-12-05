#include <gtest/gtest.h>
#include "Manager.h"  // Include the header file for your Manager class

// Define a test fixture for the Manager class
class ManagerTest : public ::testing::Test {
protected:
    // You can initialize objects or resources shared by all tests in the fixture here

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
    // Arrange
    ClassProject::Manager manager;  // Create an instance of Manager class


    ClassProject::BDD_ID var_a = manager.createNode(0, 1, 2, "a");
    //ClassProject::BDD_ID TrueNode = manager.createNode(1, 1, 1, "True");

    // Assert
    // You need to define your own assertions based on the expected behavior of createNode
    // For example, you might want to check if the uniqueTable has been updated correctly
    //EXPECT_TRUE(manager.isVariable(result));
    //EXPECT_EQ(manager.topVar(var_a), 2); 
    EXPECT_EQ(manager.uniqueTableSize(), 3);
    EXPECT_EQ(manager.uniqueTableMap.size(), 3);
    //ASSERT_EQ(manager.getUniqueTableMapSize(), 1);  // Assuming one entry is added to the map
    // Add more assertions as needed
}

// You can define additional test cases as needed

// Entry point for running the tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

