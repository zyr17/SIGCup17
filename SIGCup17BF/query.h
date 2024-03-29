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
	bool frechetdistancevalid_dfs(Trajectory x, Trajectory y, double distance, double *dfsleft, double *dfsdown, std::pair<int, int> *dfsheap, int *dfsstage, char *dfsleftchar, char *dfsdownchar, int *dfsclearsize, char *dfspoint);
	void dfsquick(int &totalnum, double *dfsleft, double *dfsdown, std::pair<int, int> *dfsheap, int *dfsstage, char *dfsleftchar, char *dfsdownchar, char *dfspoint, Trajectory &x, Trajectory &y, double distance);
	bool dfsdisconnect(int &totalnum, std::pair<int, int> *dfsheap, int *dfsstage, char *dfspoint, Trajectory &x, Trajectory &y, double distance);
	std::vector<int> onequery(Query query, double *dfsleft, double *dfsdown, std::pair<int, int> *dfsheap, int *dfsstage, char *dfsleftchar, char *dfsdownchar, int *dfsclearsize, char *dfspoint);
}