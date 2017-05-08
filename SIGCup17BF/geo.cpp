#include "geo.h"
namespace geo {
	/*inline int dcmp(double x) {
		if (x > EPS) return 1;
		if (x < -EPS) return -1;
		return 0;
	}
	template<typename T>
	inline T min(T x, T y) {
		return x < y ? x : y;
	}
	template<typename T>
	inline T max(T x, T y) {
		return x > y ? x : y;
	}
	double displine(Point p1, Point p2, Point q) {
		return fabs((p1 - q) * (p2 - q)) / (p1 - p2).len();
	}*/
	Point proj(Point p1, Point p2, Point q) {
		return p1 + ((p2 - p1) * ((p2 - p1) % (q - p1) / (p2 - p1).len2()));
	}
}