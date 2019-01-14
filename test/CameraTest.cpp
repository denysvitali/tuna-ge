#include <gtest/gtest.h>
#include <tuna-ge.h>
#include "structure/camera/CameraMode.h"

using namespace tunage;
namespace {

    class CameraTest : public ::testing::Test {
    protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        CameraTest() {
            // You can do set-up work for each test here.
        }

        ~CameraTest() override {
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

    TEST(CameraTest, camera_update_look_at_point) {
        Camera c0{"Camera 0"};
        glm::vec3 position{1, 1, 1};
        glm::vec3 point{1, 2, 3};
        glm::vec3 front{1, 0, 0};
        glm::vec3 up{0,1,0};

        c0.setMode(CameraMode::LOOK_AT_POINT);
        c0.setProjMode(ProjectionMode::PERSPECTIVE);

        c0.setPos(position);
        c0.setUp(up);
        c0.lookAt(point);

        c0.update();
        ASSERT_EQ(c0.getMatrix(), glm::lookAt(position, point, up));
    }

    TEST(CameraTest, camera_update_look_front) {
        Camera c0{"Camera 0"};
        glm::vec3 position{1, 1, 1};
        glm::vec3 point{1, 2, 3};
        glm::vec3 front{1, 0, 0};
        glm::vec3 up{0,1,0};

        c0.setMode(CameraMode::LOOK_TOWARDS_VECTOR);
        c0.setProjMode(ProjectionMode::PERSPECTIVE);

        c0.setPos(position);
        c0.setUp(up);
        c0.setFront(front);

        c0.update();
        ASSERT_EQ(c0.getMatrix(), glm::lookAt(position, position + front, up));
    }
}