#pragma once
#include <vector>
#include "input.h"
#include "query.h"
namespace index {
	void search(input::Query q, std::vector<int> &checklist, std::vector<int> &surelist);
}