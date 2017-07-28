#pragma once
#include <vector>
#include "geo.h"
#include "input.h"
#include "query.h"
#include "kdtree.h"
#include "def.h"
namespace Index{
	void makeindex(std::vector<geo::Trajectory> &input, kdt::twod_tree &start, kdt::twod_tree &end);
	void search(input::Query q, kdt::twod_tree &start, kdt::twod_tree &end, std::vector<int> &checklist, std::vector<int> &surelist);
}