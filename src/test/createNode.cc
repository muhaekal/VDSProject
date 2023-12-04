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
    Manager manager;  // Create an instance of Manager class

    // Act
    BDD_ID l = 0;
    BDD_ID h = 0;
    BDD_ID x = 0;
    std::string label = "False";

    BDD_ID result = manager.createNode(l, h, x, label);

    // Assert
    // You need to define your own assertions based on the expected behavior of createNode
    // For example, you might want to check if the uniqueTable has been updated correctly
    ASSERT_EQ(manager.UniqueTable.size(), 1);  // Assuming the initial size was 0 and one entry is added
    ASSERT_EQ(manager.getUniqueTableMapSize(), 1);  // Assuming one entry is added to the map
    // Add more assertions as needed
}

// You can define additional test cases as needed

// Entry point for running the tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

