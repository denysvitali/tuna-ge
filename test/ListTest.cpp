#include <gtest/gtest.h>
#include <tuna-ge.h>

using namespace tunage;
namespace {

    class ListTest : public ::testing::Test {
    protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        ListTest() {
            // You can do set-up work for each test here.
        }

        ~ListTest() override {
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

    TEST(ListTest, list_pass) {
        Mesh n0{"Mesh0"};
        Mesh n1{"Mesh1"};
        Mesh n2{"Mesh2"};
        Mesh n3{"Mesh3"};
        Mesh n4{"Mesh4"};
        Camera c0{"Camera0"};
        Camera c1{"Camera1"};
        Light l0{"Light0"};
        Light l1{"Light1"};

        n0.link(&n1);
        n0.link(&n2);
        n0.link(&c0);
        n0.link(&l0);
        n1.link(&n3);
        n1.link(&l1);
        n2.link(&n4);
        n4.link(&c1);

        n1.setFlipScene(true);

        List list;
        list.pass(&n0);
        std::vector<Element> render_elements = list.getRenderElements();

        ASSERT_EQ(list.getRenderElements().size(), 5);
        ASSERT_EQ(list.getRenderElementsMirrored().size(), 2);
        ASSERT_EQ(list.getRenderLights().size(), 2);
        ASSERT_EQ(list.getRenderLightsMirrored().size(), 1);
        ASSERT_EQ(list.getRenderCameras().size(), 2);
    }
}