#pragma once
#include <iostream>
#include <vector>
#include "geo.h"
using namespace geo;
namespace input {
	struct Query{
		double k;
		Trajectory traj;
		Query(){}
		Query(Trajectory traj, double k) : traj(traj), k(k){}
	};
	const int BUFFER_LENGTH = 100;
	Trajectory readtrajectory(std::string filename);
	std::vector<Trajectory> readtraindata();
	std::vector<Query> readquerydata();
}