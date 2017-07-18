#include "input.h"

namespace input {

	std::string dataprev;
	std::string dataname;
	std::string datapath;
	std::string inputfilename;
	std::string inputqueryname;

	void getconfig(std::string path)
	{
		FILE *f = fopen(path.c_str(), "r");
		char buffer[BUFFER_LENGTH] = { 0 };
		fgets(buffer, BUFFER_LENGTH, f);
		if (buffer[strlen(buffer) - 1] < 33)
			buffer[strlen(buffer) - 1] = 0;
		dataprev = buffer;
		fgets(buffer, BUFFER_LENGTH, f);
		if (buffer[strlen(buffer) - 1] < 33)
			buffer[strlen(buffer) - 1] = 0;
		dataname = buffer;
		datapath = dataprev + "/" + dataname;
		fgets(buffer, BUFFER_LENGTH, f);
		if (buffer[strlen(buffer) - 1] < 33)
			buffer[strlen(buffer) - 1] = 0;
		inputfilename = buffer;
		fgets(buffer, BUFFER_LENGTH, f);
		if (buffer[strlen(buffer) - 1] < 33)
			buffer[strlen(buffer) - 1] = 0;
		inputqueryname = buffer;
		fclose(f);
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
		FILE *out = fopen((datapath + "/results/dataset.txt").c_str(), "w");
		std::vector<Trajectory> res;
		char buffer[BUFFER_LENGTH];
		int count = 0;
		for (; fgets(buffer, BUFFER_LENGTH, inputlist) != NULL;) {
			fprintf(out, "%s", buffer);
			if (buffer[strlen(buffer) - 1] < 33)
				buffer[strlen(buffer) - 1] = 0;
			if (buffer[0] == 0) continue;
			res.push_back(readtrajectory(buffer));
			count++;
			if (count % 1000 == 0) printf("%d read done\n", count);
		}
		fclose(inputlist);
		fclose(out);
		return res;
	}

	std::vector<Query> readquerydata(){
		std::vector<Query> res;
		FILE *inputlist = fopen((datapath + "/" + inputqueryname).c_str(), "r");
		FILE *out = fopen((datapath + "/results/queries.txt").c_str(), "w");
		char buffer[BUFFER_LENGTH];
		double maxdistance;
		while (~fscanf(inputlist, "%s%lf", buffer, &maxdistance)) {
			if (buffer[0] == 0) continue;
			fprintf(out, "%s %lf\n", buffer, maxdistance);
			res.push_back(Query(readtrajectory(buffer), maxdistance));
		}
		fclose(inputlist);
		fclose(out);
		return res;
	}
}
