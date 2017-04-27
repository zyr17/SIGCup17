#include "input.h"

namespace input {
	std::string datapath = "C:\\Users\\zyr17\\Documents\\Lab\\SIGSPATIAL 2017\\Datas\\dataset-sample\\files\\";
	std::string inputfilename = "dataset.txt";
	std::vector<Trajectory> readdata()
	{
		FILE *inputlist = fopen((datapath + "\\" + inputfilename).c_str(), "r");
		std::vector<Trajectory> res;
		char buffer[100];
		for (; fgets(buffer, 100, inputlist) != NULL; ) {
			buffer[strlen(buffer) - 1] = 0;
			Trajectory traj(buffer);
			std::vector<Point> trajdata;
			FILE *inputdata = fopen((datapath + "\\" + buffer).c_str(), "r");
			fgets(buffer, 100, inputdata);
			double t1, t2, t3, t4;
			while (~fscanf(inputdata, "%lf%lf%lf%lf", &t1, &t2, &t3, &t4))
				trajdata.push_back(Point(t1, t2));
			traj.data = trajdata;
			res.push_back(traj);
		}
		return res;
	}
}
