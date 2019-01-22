#pragma once

#include "../../libapi.h"

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "../string/String.h"

namespace tunage {
    class LIB_API Object {

    public:
		Object();
		explicit Object(const char* name);

        int getId();
        String getName();
        void setName(const char* newString);
    private:
		virtual void render() = 0;
		static int idGenCount;
        int m_id;
        String m_name{""};
    };
}