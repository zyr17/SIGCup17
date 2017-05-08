#pragma once
#include <vector>
#include "geo.h"
#include "input.h"
using namespace geo;
using namespace input;
namespace query{
	extern std::vector<Trajectory> traj;
	std::pair<double, double> getminvalidpos(Point startpoint, Point endpoint, Point refrencepoint, double k);
	bool frechetdistancevalid(Trajectory x, Trajectory y, double distance);
	double discretefrechetdiatance(Trajectory x, Trajectory y);
	std::vector<int> onequery(Query query);
}