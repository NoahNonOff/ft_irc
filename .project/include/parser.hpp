#pragma once

#include <iostream> /* debuggng */
#include <map>
#include <algorithm>

#include "Atype.hpp"
#include "Object.hpp"

/* --------------------------------------------------- */

namespace JSON {

	Object	*parseFile(const std::string &filename);
}