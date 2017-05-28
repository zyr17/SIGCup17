#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "geo.h"
#include "input.h"
#include "query.h"
#include "graphics.h"

int main() {
	auto res = input::readtraindata();
	srand(unsigned(time(NULL) + clock()));
	//printf("%d\n%s %d\n%s %d\n", res.size(), res[0].name.c_str(), res[0].data.size(), res[1].name.c_str(), res[1].data.size());
	auto res2 = input::readquerydata();
	//printf("%d\n%s %f %d\n%s %f %d\n", res2.size(), res2[0].traj.name.c_str(), res2[0].k, res2[0].traj.data.size(), res2[1].traj.name.c_str(), res2[1].k, res2[1].traj.data.size());
	//getchar();
	query::traj = res;
	/*
	for (int i = 0; i < res2.size(); i++){
		char buffer[input::BUFFER_LENGTH];
		sprintf(buffer, "%04d.txt", i);
		FILE *f = fopen((input::datapath + "\\results\\" + buffer).c_str(), "w");
		auto res3 = query::onequery(res2[i]);
		for (auto &i : res3)
			fprintf(f, "%s\n", query::traj[i].name.c_str());
		fclose(f);
	}
	*/
	int maxtime = 0;
	for (int i = 0; i < 10000; i++) {
		if (i % 100 == 0) printf("doing %d\n", i);
		int r = rand();
		double k = 600000 / 65536 * r;
		int x = rand() % query::traj.size(), y = rand() % query::traj.size();
		query::frechetdistancevalid_dfs(query::traj[x], query::traj[y], k);
		if (query::dfsnum[i].c > maxtime) {
			maxtime = query::dfsnum[i].c;
			graphics::drawdistancepicture(query::traj[x], query::traj[y], k, "");
		}
	}
	std::sort(query::dfsnum.begin(), query::dfsnum.end(), [](fur x, fur y) { return x.c * 1. / (x.a + x.b) < y.c * 1. / (y.a + y.b); });
	for (int ii = 0; ii < query::dfsnum.size(); ) {
		auto i = query::dfsnum[ii];
		printf("%d\t%d\t%d\t%d\t%f\t%f\n", ii, i.a, i.b, i.c, i.d, i.c * 1. / (i.a + i.b));
		if (ii < 9900) ii += 100;
		else ii++;
	}
	auto i = query::dfsnum.size() - 1;
	//auto drawres = graphics::drawdistancepicture(query::traj[querynum[i].first], query::traj[querynum[i].second], query::dfsnum[i].d, "C:\\Users\\zyr17\\Documents\\Lab\\SIGSPATIAL 2017\\a.png");
	
	//printf("%d %d\n", query::traj[querynum[i].first].data.size(), query::traj[querynum[i].second].data.size());
	//for (auto i : query::dfsnum)
	//	printf("%d %d %d %f\n", i.a, i.b, i.c, i.d);
	getchar();
}