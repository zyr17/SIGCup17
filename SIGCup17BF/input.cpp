#include "input.h"

namespace input {

	std::string dataprev;
	std::string dataname;
	std::string datapath;
	std::string inputfilename;
	std::string inputqueryname;

	void getconfig()
	{
		dataprev = "C:/Users/zyr17/Documents/Lab/SIGSPATIAL 2017/Datas/";
		dataname = "naive-sample";
		datapath = dataprev + dataname;
		inputfilename = "files/dataset.txt";
		inputqueryname = "files/queries.txt";
	}

	Trajectory readtrajectory(std::string filename){
		Trajectory traj(filename);
		std::vector<Point> trajdata;
		FILE *inputdata = fopen((datapath + "/" + filename).c_str(), "r");
		char buffer[BUFFER_LENGTH];
		fgets(buffer, BUFFER_LENGTH, inputdata);
		double t1, t2, t3, t4;
		while (~fscanf(inputdata, "%lf%lf%lf%lf", &t1, &t2, &t3, &t4))
			trajdata.push_back(Point(t1, t2));
		traj.data = trajdata;
		fclose(inputdata);
		return traj;
	}

	std::vector<Trajectory> readtraindata(){
		FILE *inputlist = fopen((datapath + "/" + inputfilename).c_str(), "r");
		std::vector<Trajectory> res;
		char buffer[BUFFER_LENGTH];
		int count = 0;
		for (; fgets(buffer, BUFFER_LENGTH, inputlist) != NULL;) {
			if (buffer[strlen(buffer) - 1] < 33)
				buffer[strlen(buffer) - 1] = 0;
			if (buffer[0] == 0) continue;
			res.push_back(readtrajectory(buffer));
			count++;
			if (count % 1000 == 0) printf("%d read done\n", count);
		}
		fclose(inputlist);
		return res;
	}

	std::vector<Query> readquerydata(){
		std::vector<Query> res;
		FILE *inputlist = fopen((datapath + "/" + inputqueryname).c_str(), "r");
		char buffer[BUFFER_LENGTH];
		double maxdistance;
		while (~fscanf(inputlist, "%s%lf", buffer, &maxdistance)) {
			if (buffer[0] == 0) continue;
			res.push_back(Query(readtrajectory(buffer), maxdistance));
		}
		fclose(inputlist);
		return res;
	}
}
