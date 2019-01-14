#pragma once

#include "../../libapi.h"

namespace tunage {
	class LIB_API String {
	public:
		explicit String(const char* content) : m_content{content} {};

		const char* data();

		unsigned long size();

		bool operator==(const String &o) const;

	private:
		std::string m_content;
	};
}