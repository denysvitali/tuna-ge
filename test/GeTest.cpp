#include <iomanip>

#include <FreeImage.h>
#include <gtest/gtest.h>

#include "../tuna-ge.h"
#include "../structure/utils/CurrentDir.h"
#include "utils/ImageComparator.h"


using namespace tunage;
using namespace testing_utils;

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

	TEST(GeTest, interface_test) {
		std::stringstream ss;
		ss << LIB_MAJOR << "." << LIB_MINOR << "." << LIB_PATCH;

		String version = TunaGE::version();
		std::string ver = std::string(version.data());
		//free(version);

		std::string ourver = ss.str();

		ASSERT_TRUE(strncmp(ver.data(), ourver.data(), ourver.size()) == 0);
	}

    TEST(GeTest, scene_setup){

		char dir[FILENAME_MAX];
		GetCurrentDir(dir, FILENAME_MAX);

#ifdef CI
		std::cerr << "Running in CI" << std::endl;
    	FIBITMAP* bmp = FreeImage_Load(FIF_BMP, "../test/expected_results/1.bmp");
#else
		FIBITMAP* bmp = FreeImage_Load(FIF_BMP, "../../tuna-ge/test/expected_results/1.bmp");
#endif
    	ASSERT_NE(bmp, nullptr);

    	TunaGE::init();

		auto* camera1 = new Camera{"camera 1"};
		auto* camera2 = new Camera{"camera 2"};

		camera1->setPos(glm::vec3(0, -7, 0));
		ASSERT_EQ(glm::vec3(0, -7, 0), camera1->getPos());

		camera1->setUp(glm::vec3(0.0f, -1.0f, 0.0f));
		ASSERT_EQ(glm::vec3(0.0f, -1.0f, 0.0f), camera1->getUp());

		camera1->lookAt(glm::vec3(-0.00002, -8, -0.00002));
		ASSERT_EQ(glm::vec3(-0.00002, -8, -0.00002), camera1->getLookAtPoint());

		camera1->setMode(CameraMode::LOOK_AT_POINT);
		ASSERT_EQ(CameraMode::LOOK_AT_POINT, camera1->getMode());

		camera2->setPos(glm::vec3(1.0, -5, 0));
		ASSERT_EQ(glm::vec3(1.0, -5, 0), camera2->getPos());

		camera2->lookAt(glm::vec3(0, -10, 0));
		ASSERT_EQ(glm::vec3(0, -10, 0), camera2->getLookAtPoint());

		camera2->setUp(glm::vec3(0.0f, 1.0f, 0.0f));
		ASSERT_EQ(glm::vec3(0, 1.0, 0.0), camera2->getUp());

		Mesh mesh{ "plane" };
		ASSERT_EQ(std::string("plane"), std::string(mesh.getName().data()));


		Material material{};
		material.setAmbient(glm::vec3(0.0f, 0.0f, 0.0f));
		ASSERT_EQ(glm::vec3(0.0f, 0.0f, 0.0f), material.getAmbient());

		material.setShininess(120);
		ASSERT_EQ(120, material.getShininess());

		material.setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
		ASSERT_EQ(glm::vec3(1.0f, 1.0f, 1.0f), material.getSpecular());

		material.setDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));
		ASSERT_EQ(glm::vec3(0.5f, 0.5f, 0.5f), material.getDiffuse());

		Texture tex{"a bad time"};
		ASSERT_EQ(std::string("a bad time"), std::string(tex.getName().data()));

#ifdef _WINDOWS
		tex.loadFromFile("../tuna-ge/assets/textures/sans.png");
#endif

#ifdef CI
		std::cerr << "Running in CI" << std::endl;
		tex.loadFromFile("../assets/textures/sans.png");
#else
		std::cerr << "Not running in CI" << std::endl;
		tex.loadFromFile("../../tuna-ge/assets/textures/sans.png");
#endif

		ASSERT_NE(0, tex.getId());

		material.setTexture(&tex);

		mesh.setMaterial(&material);
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
		root.link(camera1);
		root.link(camera2);

		Light light{ "Light 1" };
		light.setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.2f, 0.0f)));
		light.setLight(1);
		light.setIntensity(1);
		light.setRadius(0.15);
		light.setLightAmbient(glm::vec3(0.0f, 1.0f, 0.0f));
		light.setLightDiffuse(glm::vec3(0.0f, 1.0f, 0.0f));
		light.setLightSpecular(glm::vec3(0.0f, 1.0f, 0.0f));
		light.enable();

		mesh.link(&light);

		Light light2{ "Light 2" };
		light2.setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 1.2f, 0.0f)));
		light2.setLight(2);
		light2.setIntensity(1.0);
		light2.setRadius(0.15);
		light2.setLightAmbient(glm::vec3(1.0f, 0.0f, 0.0f));
		light2.setLightDiffuse(glm::vec3(1.0f, 0.0f, 0.0f));
		light2.setLightSpecular(glm::vec3(1.0f, 0.0f, 0.0f));
		light2.enable();
		mesh.link(&light2);

		TunaGE::renderList.pass(&root);

		//ASSERT_EQ(2, TunaGE::renderList.getRenderCameras().size());
		ASSERT_EQ(1, TunaGE::renderList.getRenderElements().size());
		ASSERT_EQ(2, TunaGE::renderList.getRenderLights().size());

		std::cout << "Library Version: " << TunaGE::version().data() << std::endl;
		int w = 900;
		int h = 900;

		TunaGE::setWindowSize(w, h);

		auto* rendered_bmp = (FIBITMAP*) TunaGE::renderSingleFrame(w, h);

		ImageComparator::compare(bmp, rendered_bmp);
		
		FreeImage_Unload(rendered_bmp);
		FreeImage_Unload(bmp);

		delete camera1;
		delete camera2;

		TunaGE::free();
    }

}