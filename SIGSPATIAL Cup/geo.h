#pragma once
#include <iostream>
#include<vector>
namespace geo {
	struct Point {
		double x, y;
		Point() {}
		Point(double x, double y) : x(x), y(y) {}
	};
	struct Trajectory {
		std::vector<Point> data;
		std::string name;
		Trajectory() {}
		Trajectory(std::string name) : name(name) {}
		Trajectory(std::string name, std::vector<Point> data) : name(name) {}
	};
}