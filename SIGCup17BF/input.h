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
	#if 0
	const std::string datapath = "C:\\Users\\zyr17\\Documents\\Lab\\SIGSPATIAL 2017\\Datas\\dataset-sample\\files\\";
#else
	const std::string datapath = "D:\\Documents\\Lab\\SIGSPATIAL 2017\\dataset-sample\\files";
#endif
	const std::string inputfilename = "dataset.txt";
	const std::string inputqueryname = "queries.txt";

	Trajectory readtrajectory(std::string filename);
	std::vector<Trajectory> readtraindata();
	std::vector<Query> readquerydata();
}