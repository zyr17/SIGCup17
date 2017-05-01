#include "geo.h"
#include "input.h"
#include "query.h"

int main() {
	auto res = input::readtraindata();
	//printf("%d\n%s %d\n%s %d\n", res.size(), res[0].name.c_str(), res[0].data.size(), res[1].name.c_str(), res[1].data.size());
	auto res2 = input::readquerydata();
	//printf("%d\n%s %f %d\n%s %f %d\n", res2.size(), res2[0].traj.name.c_str(), res2[0].k, res2[0].traj.data.size(), res2[1].traj.name.c_str(), res2[1].k, res2[1].traj.data.size());
	//getchar();
	query::traj = res;
	for (int i = 0; i < res2.size(); i++){
		char buffer[input::BUFFER_LENGTH];
		sprintf(buffer, "%04d.txt", i);
		FILE *f = fopen((input::datapath + "\\results\\" + buffer).c_str(), "w");
		auto res3 = query::onequery(res2[i]);
		for (auto &i : res3)
			fprintf(f, "%s\n", query::traj[i].name);
		fclose(f);
	}
}