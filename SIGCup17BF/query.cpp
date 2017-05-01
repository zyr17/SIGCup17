#include "query.h"
namespace query{
	std::vector<Trajectory> traj;
	double discretefrechetdiatance(Trajectory x, Trajectory y){
		std::vector<std::vector<double>> d;
		int n = x.data.size(), m = y.data.size();
		d.resize(n);
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				d[i].push_back(1e100);
		d[0][0] = (x.data[0] - y.data[0]).len();
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++){
				if (!(i || j)) continue;
				double lastmin = 1e100;
				if (i && j && lastmin > d[i - 1][j - 1])
					lastmin = d[i - 1][j - 1];
				if (i && lastmin > d[i - 1][j])
					lastmin = d[i - 1][j];
				if (j && lastmin > d[i][j - 1])
					lastmin = d[i][j - 1];
				double nowdis = (x.data[i] - y.data[j]).len();
				if (lastmin < nowdis)
					lastmin = nowdis;
				d[i][j] = lastmin;
			}
		//printf("%f\n", d[n - 1][m - 1]);
		return d[n - 1][m - 1];
	}
	std::vector<int> onequery(Query q){
		std::vector<int> res;
		for (int i = 0; i < traj.size(); i++)
			if (discretefrechetdiatance(q.traj, traj[i]) <= q.k)
				res.push_back(i);
		return res;
	}
}