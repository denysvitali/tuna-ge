#include <gtest/gtest.h>
#include "../structure/TunaNode.h"

using namespace tunage;
namespace {

class NodeTest : public ::testing::Test {
    protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        NodeTest() {
            // You can do set-up work for each test here.
        }

        ~NodeTest() override {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        void SetUp() override {
            // Code here will be called immediately after the constructor (right
            // before each test).
        }

        void TearDown() override {
            // Code here will be called immediately after each test (right
            // before the destructor).
        }

        // Objects declared here can be used by all tests in the test case for Foo.
    };

    TEST(NodeCreation, node_creation){
        TunaNode node{"Node 1"};
        ASSERT_EQ("Node 1", node.getName());
    }

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}