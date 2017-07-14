#pragma once
#include <vector>
#include <algorithm>

//#define DEBUG_KDT
//#define KDT_N

namespace kdt {

	struct fpoint {
		double x, y;
		int id;
		fpoint() {}
		fpoint(double x, double y) : x(x), y(y) {}
		fpoint(double x, double y, int z) : x(x), y(y), id(z) {}
		fpoint operator- (const fpoint &b) {
			return fpoint(x - b.x, y - b.y);
		}
		inline double len2() {
			return x * x + y * y;
		}
		inline double len() {
			return sqrt(len2());
		}
	};
	class twod_tree {

	private:
		std::vector<fpoint> points;
		std::vector<int> tree;
		int tot;
		std::vector<double> minx, miny, maxx, maxy;
		void ini(int now, int l, int r, int base);
		inline void p_add(int pos, fpoint o, int totnum, std::vector<int> &res);
		inline bool is_in(fpoint point, std::vector<int> &res, int totnum, int checkpos);
		void pnum(int now, fpoint point, int totnum, std::vector<int> &res, int base);
		void psize(int now, fpoint p1, fpoint p2, std::vector<int> &res, int base);

	public:
		twod_tree();
		void clear();
		int make_tree(fpoint*, int tot);
		int make_tree(std::vector<fpoint>&, int tot = -1);
		std::vector<int> find_by_dis(double x, double y, double maxdis);
		int find_by_num(double x, double y, int totnum, std::vector<int> &result);
		int find_by_num(fpoint point, int totnum, std::vector<int> &result);
		int find_by_rect(double x1, double y1, double x2, double y2, std::vector<int> &result);
		int find_by_rect(fpoint point1, fpoint point2, std::vector<int> &result);
	};

}