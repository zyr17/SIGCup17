#include "query.h"
namespace query{
	std::vector<Trajectory> traj;

	std::pair<double, double> getminvalidpos(Point startpoint, Point endpoint, Point refrencepoint, double k)
	{
		Point pj = proj(startpoint, endpoint, refrencepoint);
		double mindis2 = (pj - refrencepoint).len2();
		if (mindis2 > k * k) return std::make_pair(1e100, 0);
		double dlength = sqrt(k * k - mindis2);
		double midpos = (pj - startpoint).len();
		double totlength = (endpoint - startpoint).len();
		dlength /= totlength;
		midpos /= totlength;
		//printf("%f %f %f %f\n", mindis2, k, midpos - dlength, midpos + dlength);
		return std::make_pair(midpos - dlength, midpos + dlength);
	}

	bool frechetdistancevalid(Trajectory x, Trajectory y, double distance) {
		int n = x.data.size(), m = y.data.size();
		if ((x[0] - y[0]).len() > distance + EPS || (x[n - 1] - y[m - 1]).len() > distance + EPS)
			return 0;
		std::vector<std::vector<double>> left, down;
		left.resize(n);
		down.resize(n);
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++) {
				left[i].push_back(1e100);
				down[i].push_back(1e100);
			}
		left[0][0] = down[0][0] = 0;
		for (int i = 0; i < n - 1; i++)
			for (int j = 0; j < m - 1; j++) {
				if (left[i][j] > 1 + EPS && down[i][j] > 1 + EPS) continue;
				//printf("%d %d\n", i, j);
				std::pair<double, double> horizontal = getminvalidpos(x[i], x[i + 1], y[j + 1], distance);
				std::pair<double, double> vertical = getminvalidpos(y[j], y[j + 1], x[i + 1], distance);
				if (left[i][j] < 1 + EPS || down[i][j] <= horizontal.first)
					down[i][j + 1] = horizontal.first;
				else if (down[i][j] <= horizontal.second + EPS)
					down[i][j + 1] = down[i][j];
				if (down[i][j] < 1 + EPS || left[i][j] <= vertical.first)
					left[i + 1][j] = vertical.first;
				else if (left[i][j] <= vertical.second + EPS)
					left[i + 1][j] = left[i][j];
			}
		//printf("%f %f\n", left[n - 2][m - 2], down[n - 2][m - 2]);
		return left[n - 2][m - 2] <= 1 + EPS || down[n - 2][m - 2] <= 1 + EPS;
	}

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
				//printf("%f %f, %f %f, %f\n", x[i].x, x[i].y, y[j].x, y[j].y, nowdis);
				if (lastmin < nowdis)
					lastmin = nowdis;
				d[i][j] = lastmin;
			}
		//printf("%f\n", d[n - 1][m - 1]);
		//getchar();
		return d[n - 1][m - 1];
	}
	std::vector<int> onequery(Query q) {
		std::vector<int> res;
		for (int i = 0; i < traj.size(); i++) {
			//printf("%d: ", i);
			//if (discretefrechetdiatance(q.traj, traj[i]) <= q.k)
			if (frechetdistancevalid(q.traj, traj[i], q.k))
				res.push_back(i);
		}
		return res;
	}
}