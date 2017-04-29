#include "input.h"

namespace input {
#if 0
	std::string datapath = "C:\\Users\\zyr17\\Documents\\Lab\\SIGSPATIAL 2017\\Datas\\dataset-sample\\files\\";
#else
	std::string datapath = "D:\\Documents\\Lab\\SIGSPATIAL 2017\\dataset-sample\\files";
#endif
	std::string inputfilename = "dataset.txt";
	std::string inputqueryname = "queries.txt";

	Trajectory readtrajectory(std::string filename){
		Trajectory traj(filename);
		std::vector<Point> trajdata;
		FILE *inputdata = fopen((datapath + "\\" + filename).c_str(), "r");
		char buffer[BUFFER_LENGTH];
		fgets(buffer, BUFFER_LENGTH, inputdata);
		double t1, t2, t3, t4;
		while (~fscanf(inputdata, "%lf%lf%lf%lf", &t1, &t2, &t3, &t4))
			trajdata.push_back(Point(t1, t2));
		traj.data = trajdata;
		return traj;
	}

	std::vector<Trajectory> readtraindata()
	{
		FILE *inputlist = fopen((datapath + "\\" + inputfilename).c_str(), "r");
		std::vector<Trajectory> res;
		char buffer[BUFFER_LENGTH];
		for (; fgets(buffer, BUFFER_LENGTH, inputlist) != NULL;) {
			if (buffer[strlen(buffer) - 1] < 33)
				buffer[strlen(buffer) - 1] = 0;
			res.push_back(readtrajectory(buffer));
		}
		return res;
	}

	std::vector<Query> readquerydata()
	{
		std::vector<Query> res;
		FILE *inputlist = fopen((datapath + "\\" + inputqueryname).c_str(), "r");
		char buffer[BUFFER_LENGTH];
		double maxdistance;
		while (~fscanf(inputlist, "%s%lf", buffer, &maxdistance)) {
			res.push_back(Query(readtrajectory(buffer), maxdistance));
		}
		return res;
	}
}
