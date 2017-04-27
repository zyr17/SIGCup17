#pragma once
#include <iostream>
#include <vector>
#include "geo.h"
using namespace geo;
namespace input {
	std::vector<Trajectory> readtraindata();
	std::vector<std::pair<std::string, double>> readquerydata();
}