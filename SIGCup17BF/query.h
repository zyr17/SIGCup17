#pragma once
#include <assert.h>
#include <vector>
#include "geo.h"
#include "input.h"
#include "index.h"
#include "kdtree.h"
using namespace geo;
using namespace input;
namespace query{
	extern std::vector<Trajectory> traj;
	extern std::vector<fur> dfsnum;
	extern kdt::twod_tree starttree, endtree;
	std::pair<double, double> getminvalidpos(Point startpoint, Point endpoint, Point refrencepoint, double k);
	bool frechetdistancevalid(Trajectory x, Trajectory y, double distance);
	bool frechetdistancevalid_dfs(Trajectory x, Trajectory y, double distance);
	void dfsmain(int i, int j, int &totalnum, int n, int m, std::vector<std::vector<double>> &left, std::vector<std::vector<double>> &down, Trajectory &x, Trajectory &y, double distance);
	double discretefrechetdiatance(Trajectory x, Trajectory y);
	std::vector<int> onequery(Query query);
}