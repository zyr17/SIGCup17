#include "input.h"


namespace fastIO{
#define BUF_SIZE 100000
#define OUT_SIZE 100000
#define ll long long
	bool IOerror = 0;

	inline char nc(FILE* filename){
		static char buf[BUF_SIZE], *p1 = buf + BUF_SIZE, *pend = buf + BUF_SIZE;
		if (p1 == pend){
			p1 = buf; pend = buf + fread(buf, 1, BUF_SIZE, filename);
			if (pend == p1) { IOerror = 1; return -1; }
		}
		return *p1++;
	}

	inline bool blank(char ch) { return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t'; }

	inline bool read(int &x, FILE* filename){
		bool sign = 0; char ch = nc(filename); x = 0;
		for (; blank(ch); ch = nc(filename));
		if (IOerror)return false;
		if (ch == '-')sign = 1, ch = nc(filename);
		for (; ch >= '0'&&ch <= '9'; ch = nc(filename))x = x * 10 + ch - '0';
		if (sign)x = -x;
		return true;
	}

	inline bool read(double &x, FILE* filename){
		bool sign = 0; char ch = nc(filename); x = 0;
		for (; blank(ch); ch = nc(filename));
		if (IOerror)return false;
		if (ch == '-')sign = 1, ch = nc(filename);
		for (; ch >= '0'&&ch <= '9'; ch = nc(filename))x = x * 10 + ch - '0';
		if (ch == '.'){
			double tmp = 1; ch = nc(filename);
			for (; ch >= '0'&&ch <= '9'; ch = nc(filename))tmp /= 10.0, x += tmp*(ch - '0');
		}
		if (sign)x = -x;
		return true;
	}

	inline bool read(char *s, FILE* filename){
		char ch = nc(filename);
		for (; blank(ch); ch = nc(filename));
		if (IOerror)return false;
		for (; !blank(ch) && !IOerror; ch = nc(filename))*s++ = ch;
		*s = 0;
		return true;
	}

	struct Ostream_fwrite{
		char *buf, *p1, *pend;

		Ostream_fwrite() { buf = new char[BUF_SIZE]; p1 = buf; pend = buf + BUF_SIZE; }

		void out(char ch, FILE* filename){
			if (p1 == pend){
				fwrite(buf, 1, BUF_SIZE, filename); p1 = buf;
			}
			*p1++ = ch;
		}

		void print(char *s, FILE* filename) { while (*s)out(*s++, filename); }
		void println(char *s, FILE* filename) { while (*s)out(*s++, filename); out('\n', filename); }
		void flush(FILE* filename) { if (p1 != buf) { fwrite(buf, 1, p1 - buf, filename); p1 = buf; } }

	}Ostream;

	inline void print(char *s, FILE* filename) { Ostream.print(s, filename); }
	inline void println(char *s, FILE* filename) { Ostream.println(s, filename); }
	inline void println(FILE* filename) { Ostream.out('\n', filename); }

#undef ll
#undef OUT_SIZE
#undef BUF_SIZE
};

using namespace fastIO;

namespace input{

	std::string datapath;
	std::string inputfilename;
	std::string inputqueryname;

	void getconfig(std::string path)
	{
		FILE *f = fopen(path.c_str(), "r");
		char buffer[BUFFER_LENGTH] = { 0 };
		fgets(buffer, BUFFER_LENGTH, f);
		for (int i = strlen(buffer) - 1; i >= 0 && buffer[i] < 33; i--)
			buffer[i] = 0;
		datapath = buffer;
		fgets(buffer, BUFFER_LENGTH, f);
		for (int i = strlen(buffer) - 1; i >= 0 && buffer[i] < 33; i--)
			buffer[i] = 0;
		inputfilename = buffer;
		fgets(buffer, BUFFER_LENGTH, f);
		for (int i = strlen(buffer) - 1; i >= 0 && buffer[i] < 33; i--)
			buffer[i] = 0;
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
		while (read(t1, inputdata) && read(t2, inputdata) && read(t3, inputdata) && read(t4, inputdata))
			trajdata.push_back(Point(t1, t2));

		traj.data = trajdata;
		IOerror = 0;
		if (trajdata.size() == 0) while (true);
		fclose(inputdata);
		return traj;
	}

	std::vector<Trajectory> readtraindata(){
		FILE *inputlist = fopen((datapath + "/" + inputfilename).c_str(), "r");
#ifdef DEBUG
		FILE *out = fopen((datapath + "/results/dataset.txt").c_str(), "w");
#endif
		std::vector<Trajectory> res;
		char buffer[BUFFER_LENGTH];
		int count = 0;
		for (; fgets(buffer, BUFFER_LENGTH, inputlist) != NULL;){
#ifdef DEBUG
			fprintf(out, "%s", buffer);
#endif
			if (buffer[strlen(buffer) - 1] < 33)
				buffer[strlen(buffer) - 1] = 0;
			if (buffer[0] == 0) continue;
			res.push_back(readtrajectory(buffer));
			count++;
			if (count % 1000 == 0) printf("%d read done\n", count);
		}
		fclose(inputlist);
#ifdef DEBUG
		fclose(out);
#endif
		return res;
	}

	std::vector<Query> readquerydata(){
		std::vector<Query> res;
		FILE *inputlist = fopen((datapath + "/" + inputqueryname).c_str(), "r");
#ifdef DEBUG
		FILE *out = fopen((datapath + "/results/queries.txt").c_str(), "w");
#endif
		char buffer[BUFFER_LENGTH];
		double maxdistance;
		while (~fscanf(inputlist, "%s%lf", buffer, &maxdistance)){
			if (buffer[0] == 0) continue;
#ifdef DEBUG
			fprintf(out, "%s %lf\n", buffer, maxdistance);
#endif
			res.push_back(Query(readtrajectory(buffer), maxdistance));
		}
		fclose(inputlist);
#ifdef DEBUG
		fclose(out);
#endif
		return res;
	}
}
