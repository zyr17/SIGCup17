#include "geo.h"
#include "input.h"

int main() {
	auto res = input::readtraindata();
	printf("%d\n%s %d\n%s %d\n", res.size(), res[0].name.c_str(), res[0].data.size(), res[1].name.c_str(), res[1].data.size());
	auto res2 = input::readquerydata();
	printf("%d\n%s %f %d\n%s %f %d\n", res2.size(), res2[0].traj.name.c_str(), res2[0].k, res2[0].traj.data.size(), res2[1].traj.name.c_str(), res2[1].k, res2[1].traj.data.size());
	getchar();
}