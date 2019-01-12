#include <gtest/gtest.h>
#include "../structure/node/Node.h"

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

    TEST(NodeTest, node_creation){
        Node node{"Node 1"};
        ASSERT_EQ("Node 1", node.getName());
    }

    TEST(NodeTest, node_id){
        Node node{"Node A"};
        int id1 = node.getId();
        Node node2{"Node B"};
        int id2 = node2.getId();

        ASSERT_NE(id1, id2);
        ASSERT_LT(id1, id2);
    }

    TEST(NodeTest, node_hierarchy){
        Node node_a{"Node A"};
        Node node_b{"Node B"};
        Node node_c{"Node C"};

        /*      A
         *  B       C
         */

        ASSERT_EQ(nullptr, node_a.getParent());
        ASSERT_EQ(nullptr, node_b.getParent());
        ASSERT_EQ(nullptr, node_c.getParent());

        // Link B to A (A -> B)
        node_a.link(&node_b);
        ASSERT_EQ(&node_a, node_b.getParent());

        // Link C to A (A -> C)
        node_a.link(&node_c);
        ASSERT_EQ(&node_a, node_c.getParent());

        ASSERT_EQ(2, node_a.getChildren().size());
        ASSERT_EQ(&node_b, node_a.getChildren()[0]);
        ASSERT_EQ(&node_c, node_a.getChildren()[1]);

        Node* p = node_a.unlinkById(node_b.getId());
        ASSERT_EQ(&node_b, p);

        ASSERT_EQ(1, node_a.getChildren().size());
        ASSERT_EQ(&node_c, node_a.getChildren()[0]);
    }

    TEST(NodeTest, node_matrix){
        Node n{"Node 1"};
        glm::mat4 m{1.2f};
        n.setMatrix(m);

        ASSERT_EQ(m, n.getMatrix());
    }

    TEST(NodeTest, node_render){
        Node n{"Node 1"};
        ASSERT_THROW(n.render(), std::runtime_error);
    }

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}