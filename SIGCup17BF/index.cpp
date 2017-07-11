#include "index.h"
namespace index {
	void search(input::Query q, std::vector<int> &checklist, std::vector<int> &surelist){
		checklist.clear();
		surelist.clear();
		for (int i = 0; i < query::traj.size(); i++)
			checklist.push_back(i);
	}
}