#include <iomanip>

#include <FreeImage.h>
#include <gtest/gtest.h>

#include "../tuna-ge.h"
#include "../structure/utils/CurrentDir.h"
#include "utils/ImageComparator.h"


using namespace tunage;
using namespace testing_utils;

namespace {

	class OvoScene : public ::testing::Test {
	protected:
		// You can remove any or all of the following functions if its body
		// is empty.

		OvoScene() {
			// You can do set-up work for each test here.
		}

		~OvoScene() override {
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

	TEST(OvoScene, scene_setup) {

		char dir[FILENAME_MAX];
		GetCurrentDir(dir, FILENAME_MAX);

		std::cerr << dir << std::endl;

		std::string path;
		FIBITMAP* bmp = nullptr;
#ifdef _WINDOWS
		bmp = FreeImage_Load(FIF_BMP, "./expected_results/2.bmp");
		path = "../assets/scenes/gauntletTex.ovo";
#else
	#ifdef CI
			std::cerr << "Running in CI" << std::endl;
			bmp = FreeImage_Load(FIF_BMP, "../test/expected_results/2.bmp");
			path = "../../assets/scenes/gauntletTex.ovo";
	#else
			bmp = FreeImage_Load(FIF_BMP, "../../tuna-ge/test/expected_results/2.bmp");
			path = "../../tuna-ge/assets/scenes/gauntletTex.ovo";
	#endif
#endif

		ASSERT_NE(nullptr, bmp);

		TunaGE::init();
		Node* root = TunaGE::loadOVO(path.data());
		ASSERT_NE(nullptr, root);
		ASSERT_NE(0, root->getChildren().size());

		auto c = new Camera("Camera 1");
		c->setPos(glm::vec3(200, 250, -250));
		//c->setPos(glm::vec3(-10, 40, -20));
		//c->setFront(glm::vec3(-0.5, -0.5, -0.5));
		c->lookAt(glm::vec3(0.01, 0.01, 0.01));
		c->setMode(CameraMode::LOOK_AT_POINT);
		c->setFarPlane(900);

		root->link(c);

		//((Light*) root->getSceneElementByName("Omni001"))->setRadius(0.2);
		//((Mesh*) root->getSceneElementByName("Torus"))->setMatrix(glm::mat4(1.0f));

		root->getSceneElementByName("Cylinder001")->setFlipScene(true);

		TunaGE::renderList.pass(root);

		int w = 900;
		int h = 900;

		auto pixels = new uint8_t(w  * h * 3);

		TunaGE::setWindowSize(w, h);

		auto* rendered_bmp = (FIBITMAP*) TunaGE::renderSingleFrame(pixels, w, h);

		ImageComparator::compare(bmp, rendered_bmp);

		FreeImage_Unload(bmp);
		FreeImage_Unload(rendered_bmp);

		TunaGE::free();

		delete c;
		delete pixels;
	}

}