#pragma once
#include <vector>
#include "geo.h"
#include "input.h"
using namespace geo;
using namespace input;
namespace query{
	extern std::vector<Trajectory> traj;
	double discretefrechetdiatance(Trajectory x, Trajectory y);
	std::vector<int> onequery(Query query);
}