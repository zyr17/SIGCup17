#pragma once
#include "geo.h"
using namespace geo;
namespace graphics {
	//std::pair<double, double> getminvalidpos(geo::Point startpoint, geo::Point endpoint, geo::Point refrencepoint, double k);
	bool drawdistancepicture(Trajectory x, Trajectory y, double distance, std::string filepath, int cellsize = 30);
}