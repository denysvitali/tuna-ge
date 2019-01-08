#include "../stdafx.h"
#include <gtest/gtest.h>
#include <tuna-ge.h>
#include <iomanip>

using namespace tunage;
namespace {

class GeTest : public ::testing::Test {
    protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        GeTest() {
            // You can do set-up work for each test here.
        }

        ~GeTest() override {
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

    TEST(GeTest, scene_setup){

		TunaGE::init();
		TunaGE::enableOriginMarker();


		Camera camera1{"camera 1"};
		Camera camera2{"camera 2"};

		camera1.setCameraPos(glm::vec3(-0.0, 2.0f, -0.0f));  // Camera is at (0,2,10), in World Space
		camera1.setCameraFront(glm::vec3(0.01f, -1.0f, 0.01f)); // looks in the direction
		camera1.setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f)); // Head is up (set to 0,-1,0 to look upside-down)
		camera1.setCameraSpeed(0.1f);

		camera2.setCameraPos(glm::vec3(-0.0, 3.0f, -0.0f));  // Camera is at (0,2,10), in World Space
		camera2.setCameraFront(glm::vec3(0.01f, -1.0f, 0.01f)); // looks in the direction
		camera2.setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f)); // Head is up (set to 0,-1,0 to look upside-down)
		camera2.setCameraSpeed(0.1f);

		Mesh mesh{ "plane" };

		Material material{};
		material.setAmbient(glm::vec3(0.0f, 0.0f, 0.0f));
		material.setShininess(120);
		material.setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
		material.setDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));

		Texture tex{"a bad time"};

#if _WINDOWS
		tex.loadFromFile("../tuna-ge/assets/textures/sans.png");
#else
		tex.loadFromFile("../../tuna-ge/assets/textures/sans.png");

#endif

		material.setTexture(&tex);

		mesh.setMaterial(material);
		mesh.setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -10.0f, 0.0f)));

		float width = 1;
		Vertex v1{ -width, 0, 0, 0, 1, 0, 0, 0 };
		Vertex v2{ width, 0, 0, 0, 1, 0, 1, 1 };
		Vertex v3{ 0, 0, -width, 0, 1, 0 , 1, 0 };

		Vertex v4{ 0, 0, width, 0, 1, 0 , 0, 1 };
		Vertex v5{ width, 0, 0, 0, 1, 0 , 1, 1 };
		Vertex v6{ -width, 0, 0, 0, 1, 0 , 0 , 0 };

		mesh.addVertex(v1);
		mesh.addVertex(v2);
		mesh.addVertex(v3);
		mesh.addVertex(v4);
		mesh.addVertex(v5);
		mesh.addVertex(v6);

		Node root{"root"};
		root.link(&mesh);
		root.link(&camera1);

		Light light{ RGBColor{100, 255, 255}, "Light 1" };
		light.setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f)));
		light.setLight(1);
		light.setIntensity(1);
		light.setLightAmbient(glm::vec3(0.0f, 1.0f, 0.0f));
		light.setLightDiffuse(glm::vec3(0.0f, 1.0f, 0.0f));
		light.setLightSpecular(glm::vec3(0.0f, 1.0f, 0.0f));
		light.enable();

		mesh.link(&light);

		Light light2{ RGBColor{100, 255, 255}, "Light 2" };
		light2.setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f)));
		light2.setLight(2);
		light2.setIntensity(1.0);
		light2.setLightAmbient(glm::vec3(1.0f, 1.0f, 1.0f));
		light2.setLightDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
		light2.setLightSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
		light2.enable();

		light.link(&light2);
		light2.link(&camera2);

		Light light3{ RGBColor{100, 255, 255}, "Light 3" };
		light3.setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, -2.0f)));
		light3.setLight(3);
		light3.setIntensity(1.0);
		light3.setLightAmbient(glm::vec3(1.0f, 0.0f, 0.0f));
		light3.setLightDiffuse(glm::vec3(1.0f, 0.0f, 0.0f));
		light3.setLightSpecular(glm::vec3(1.0f, 0.0f, 0.0f));
		light3.enable();

		light.link(&light3);

		Light light4{ RGBColor{100, 255, 255}, "Light 4" };
		light4.setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-0.8f, -0.8f, 0.4f)));
		light4.setLight(4);
		light4.setIntensity(1.0);
		light4.setLightAmbient(glm::vec3(0.0f, 0.0f, 1.0f));
		light4.setLightDiffuse(glm::vec3(0.0f, 0.0f, 1.0f));
		light4.setLightSpecular(glm::vec3(0.0f, 0.0f, 1.0f));
		light4.enable();

		light.link(&light4);

		TunaGE::renderList.pass(&root);

		std::cout << "Library Version: " << TunaGE::version() << std::endl;
		int w = 800;
		int h = 400;

		auto pixels = new uint8_t(w  * h * 3);

		TunaGE::renderSingleFrame(pixels, w, h);
		delete pixels;
    }

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}