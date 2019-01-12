#include "../stdafx.h"
#include <gtest/gtest.h>
#include <iomanip>

#include <FreeImage.h>

#include "../tuna-ge.h"
#include "../structure/utils/CurrentDir.h"


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

		char dir[FILENAME_MAX];
		GetCurrentDir(dir, FILENAME_MAX);

		std::cout << dir << std::endl;

#ifdef CI
		std::cerr << "Running in CI" << std::endl;
    	FIBITMAP* bmp = FreeImage_Load(FIF_BMP, "../test/expected_results/1.bmp");
#else
		FIBITMAP* bmp = FreeImage_Load(FIF_BMP, "../../tuna-ge/test/expected_results/1.bmp");
#endif
    	ASSERT_NE(bmp, nullptr);

    	TunaGE::setDisplayWindow(true);
		TunaGE::init();

		Camera camera1{"camera 1"};
		Camera camera2{"camera 2"};

		camera1.setPos(glm::vec3(0.1, -7, 0.1));
		ASSERT_EQ(glm::vec3(0.1, -7, 0.1), camera1.getPos());

		camera1.setUp(glm::vec3(0.0f, -1.0f, 0.0f));
		ASSERT_EQ(glm::vec3(0.0f, -1.0f, 0.0f), camera1.getUp());

		camera1.lookAt(glm::vec3(0.0, -10, 0));
		ASSERT_EQ(glm::vec3(0.0, -10, 0), camera1.getLookAtPoint());

		camera1.setMode(CameraMode::LOOK_AT_POINT);
		ASSERT_EQ(CameraMode::LOOK_AT_POINT, camera1.getMode());

		camera2.setPos(glm::vec3(1.0, -5, 0));
		ASSERT_EQ(glm::vec3(1.0, -5, 0), camera2.getPos());

		camera2.lookAt(glm::vec3(0, -10, 0));
		ASSERT_EQ(glm::vec3(0, -10, 0), camera2.getLookAtPoint());

		camera2.setUp(glm::vec3(0.0f, 1.0f, 0.0f));
		ASSERT_EQ(glm::vec3(0, 1.0, 0.0), camera2.getUp());

		Mesh mesh{ "plane" };
		ASSERT_EQ("plane", mesh.getName());


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
		ASSERT_EQ("a bad time", tex.getName());

		std::cerr << "Hello :)" << std::endl;

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
		root.link(&camera1);
		root.link(&camera2);

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

		ASSERT_EQ(2, TunaGE::renderList.getRenderCameras().size());
		ASSERT_EQ(1, TunaGE::renderList.getRenderElements().size());
		ASSERT_EQ(2, TunaGE::renderList.getRenderLights().size());

		std::cout << "Library Version: " << TunaGE::version() << std::endl;
		int w = 700;
		int h = 700;

		auto pixels = new uint8_t(w  * h * 3);

		auto* rendered_bmp = (FIBITMAP*) TunaGE::renderSingleFrame(pixels, w, h);

		int r_w = FreeImage_GetWidth(rendered_bmp);
		int r_h = FreeImage_GetHeight(rendered_bmp);
		int r_bpp = FreeImage_GetBPP(rendered_bmp);

		int e_w = FreeImage_GetWidth(bmp);
		int e_h = FreeImage_GetHeight(bmp);
		int e_bpp = FreeImage_GetBPP(bmp);

		ASSERT_EQ(e_w, r_w);
		ASSERT_EQ(e_h, r_h);
		ASSERT_EQ(e_bpp, r_bpp);

		for(int i=0; i<e_h; i++){
			BYTE* sl1 = FreeImage_GetScanLine(bmp, i);
			BYTE* sl2 = FreeImage_GetScanLine(rendered_bmp, i);

			for(int j=0; j<e_w; j++){
				ASSERT_EQ(sl1[j], sl2[j]);
			}
		}
		FreeImage_Unload(rendered_bmp);
		FreeImage_Unload(bmp);

		delete pixels;
    }

}