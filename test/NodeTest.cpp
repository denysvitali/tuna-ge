#include <gtest/gtest.h>
#include <tuna-ge.h>

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

    TEST(NodeTest, node_creation) {
        Node node{"Node 1"};
        ASSERT_STREQ("Node 1", node.getName().data());
    }

    TEST(NodeTest, node_id) {
        Node node{"Node A"};
        int id1 = node.getId();
        Node node2{"Node B"};
        int id2 = node2.getId();

        ASSERT_NE(id1, id2);
        ASSERT_LT(id1, id2);
    }

    TEST(NodeTest, node_hierarchy) {
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

    TEST(NodeTest, node_matrix) {
        Node n{"Node 1"};
        glm::mat4 m{1.2f};
        n.setMatrix(m);

        ASSERT_EQ(m, n.getMatrix());
    }

    TEST(NodeTest, node_linking){
        Node n{"Node 1"};
        Node n2{"Node 2"};

        n.link(&n2);
        ASSERT_EQ(n.getChildren()[0], &n2);
    }

    TEST(NodeTest, node_unlinking){
        Node n{"Node 1"};
        Node n2{"Node 2"};

        n.link(&n2);
        ASSERT_EQ(1, n.getChildren().size());
        n2.unlink();
        ASSERT_EQ(0, n.getChildren().size());
    }

    TEST(NodeTest, node_render) {
        Node n{"Node 1"};
        ASSERT_THROW(n.render(), std::runtime_error);
    }

    TEST(NodeTest, node_render_matrix) {
        Node n0{"Node 0"};
        Node n1{"Node 1"};
        Node n2{"Node 2"};
        Node n3{"Node 3"};

        glm::mat4 m0 = glm::mat4{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
        glm::mat4 m1 = glm::mat4{15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
        glm::mat4 m2 = glm::mat4{0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30};
        glm::mat4 m3 = glm::mat4{1,2,3,1,2,3,1,2,3,1,2,3,1,2,3,0};
        //      n0
        //      n1
        //  n2      n3

        n0.setMatrix(m0);
        n1.setMatrix(m1);
        n2.setMatrix(m2);
        n3.setMatrix(m3);

        n0.link(&n1);
        n1.link(&n2);
        n1.link(&n3);

        ASSERT_EQ(n0.getRenderMatrix(), m0);
        ASSERT_EQ(n1.getRenderMatrix(), m0*m1);
        ASSERT_EQ(n2.getRenderMatrix(), m0*m1*m2);
        ASSERT_EQ(n3.getRenderMatrix(), m0*m1*m3);
    }
}