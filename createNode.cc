#include <gtest/gtest.h>
#include "Manager.h"  // Include the header file for your Manager class
#include "ManagerInterface.h"

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

    // Act
    //ClassProject::BDD_ID l = 1;
    //ClassProject::BDD_ID h = 1;
    //ClassProject::BDD_ID x = 1;
    //std::string label = "True";

    ClassProject::BDD_ID FalseNode = manager.createNode(0, 0, 0, "False");
    ClassProject::BDD_ID TrueNode = manager.createNode(1, 1, 1, "True");

    // Assert
    // You need to define your own assertions based on the expected behavior of createNode
    // For example, you might want to check if the uniqueTable has been updated correctly
    //EXPECT_TRUE(manager.isVariable(result));
    EXPECT_EQ(manager.topVar(FalseNode), 0);
    EXPECT_EQ(manager.topVar(TrueNode), 1); 
    //ASSERT_EQ(manager.getUniqueTableMapSize(), 1);  // Assuming one entry is added to the map
    // Add more assertions as needed
}

// You can define additional test cases as needed

// Entry point for running the tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

