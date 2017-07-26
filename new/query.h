#pragma once
#include <assert.h>
#include <vector>
#include "geo.h"
#include "input.h"
#include "index.h"
#include "kdtree.h"
#include "def.h"
using namespace geo;
using namespace input;
namespace query{
	const int MAX_DFS_ARRAY = 10000000;
	extern std::vector<Trajectory> traj;
#ifdef DEBUG
	extern std::vector<fur> dfsnum;
#endif
	extern kdt::twod_tree starttree, endtree;
	std::pair<double, double> getminvalidpos(Point startpoint, Point endpoint, Point refrencepoint, double k);
	bool frechetdistancevalid(Trajectory x, Trajectory y, double distance);
	bool frechetdistancevalid_dfs(Trajectory x, Trajectory y, double distance, double *dfsleft, double *dfsdown, std::pair<int, int> *dfsheap, int *dfsstage);
	void dfsquick(int &totalnum, int n, int m, double *dfsleft, double *dfsdown, std::pair<int, int> *dfsheap, int *dfsstage, Trajectory &x, Trajectory &y, double distance);

	//deprecated
	void dfsmain(int i, int j, int &totalnum, int n, int m, std::vector<std::vector<double>> &left, std::vector<std::vector<double>> &down, Trajectory &x, Trajectory &y, double distance, int &topzero);

	//deprecated
	void dfsmain(int i, int j, int &totalnum, int n, int m, double *dfsleft, double *dfsdown, Trajectory &x, Trajectory &y, double distance, int &topzero);

	double discretefrechetdiatance(Trajectory x, Trajectory y);
	std::vector<int> onequery(Query query, double *dfsleft, double *dfsdown, std::pair<int, int> *dfsheap, int *dfsstage);
}