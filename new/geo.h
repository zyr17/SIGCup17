#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include "def.h"
namespace geo {
	const double EPS = 1e-10;
	struct fur {
		int a, b, c;
		double d;
		fur() {}
		fur(int a, int b, int c, double d) : a(a), b(b), c(c), d(d) {}
	};
	struct Point {
		double x, y;
		Point() {}
		Point(double x, double y) : x(x), y(y) {}
		Point operator + (const Point &b) {
			return Point(x + b.x, y + b.y);
		}
		inline Point operator- (Point b){
			return Point(x - b.x, y - b.y);
		}
		Point operator * (const double &b) {
			return Point(x * b, y * b);
		}
		double operator * (const Point &b) {
			return x * b.y - y * b.x;
		}
		double operator % (const Point &b) {
			return x * b.x + y * b.y;
		}
		inline double len2(){
			return x * x + y * y;
		}
		inline double len(){
			return sqrt(len2());
		}
	};
	struct Trajectory {
		std::vector<Point> data;
		std::string name;
		Trajectory() {}
		Trajectory(std::string name) : name(name) {}
		Trajectory(std::string name, std::vector<Point> data) : name(name) {}
		inline Point& operator[] (int k) {
			return data[k];
		}
	};
	/*inline int dcmp(double x);
	template<typename T>
	inline T min(T x, T y);
	template<typename T>
	inline T max(T x, T y);
	double displine(Point p1, Point p2, Point q);*/
	Point proj(Point p1, Point p2, Point q);
}