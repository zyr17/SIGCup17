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
#ifdef VS2017
	const std::string dataprev = "C:\\Users\\zyr17\\Documents\\Lab\\SIGSPATIAL 2017\\Datas\\";
#else
	const std::string dataprev = "C:\\Now Using\\Lab\\data\\";
#endif
#if 1
	const std::string dataname = "dataset-sample\\files";
#else
	const std::string dataname = "naivedata";
#endif
	const std::string datapath = dataprev + dataname;
	const std::string inputfilename = "dataset.txt";
	const std::string inputqueryname = "queries.txt";

	Trajectory readtrajectory(std::string filename);
	std::vector<Trajectory> readtraindata();
	std::vector<Query> readquerydata();
}