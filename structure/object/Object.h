#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace tunage {
    class Object {

    public:
        Object(std::string name="");

        virtual void render() = 0;
        int getId();
        std::string getName();
        void setName(std::string newString);
    private:
        static int idGenCount;
        int m_id;
        std::string m_name;
    };
}