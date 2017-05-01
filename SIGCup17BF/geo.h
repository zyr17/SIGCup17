#pragma once
#include <iostream>
#include <vector>
#include <cmath>
namespace geo {
	struct Point {
		double x, y;
		Point() {}
		Point(double x, double y) : x(x), y(y) {}
		inline Point operator- (Point b){
			return Point(x - b.x, y - b.y);
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
	};
}