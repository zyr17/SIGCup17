#include "index.h"
namespace Index {
	void makeindex(std::vector<geo::Trajectory> &input, kdt::twod_tree &start, kdt::twod_tree &end)
	{
		std::vector<kdt::fpoint> vec;
		for (auto i : input)
			vec.push_back(kdt::fpoint(i.data[0].x, i.data[0].y));
		start.make_tree(vec);
		vec.clear();
		for (auto i : input)
			vec.push_back(kdt::fpoint(i.data[i.data.size() - 1].x, i.data[i.data.size() - 1].y));
		end.make_tree(vec);
	}
	void search(input::Query q, kdt::twod_tree &start, kdt::twod_tree &end, std::vector<int> &checklist, std::vector<int> &surelist){
		checklist.clear();
		surelist.clear();
		auto res1 = start.find_by_dis(q.traj.data[0].x, q.traj.data[0].y, q.k);
		auto res2 = end.find_by_dis(q.traj.data[q.traj.data.size() - 1].x, q.traj.data[q.traj.data.size() - 1].y, q.k);
		std::sort(res1.begin(), res1.end());
		std::sort(res2.begin(), res2.end());
		int p1 = 0, p2 = 0;
		for (; p1 < res1.size() && p2 < res2.size(); ) {
			if (res1[p1] == res2[p2]) {
				checklist.push_back(res1[p1]);
				p1++;
				p2++;
			}
			else if (res1[p1] > res2[p2])
				p2++;
			else
				p1++;
		}
	}
}