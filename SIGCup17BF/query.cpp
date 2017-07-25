#include "query.h"
namespace query{
	std::vector<Trajectory> traj;
	kdt::twod_tree starttree, endtree;

	std::pair<double, double> getminvalidpos(geo::Point startpoint, geo::Point endpoint, geo::Point refrencepoint, double k)
	{
		geo::Point pj = proj(startpoint, endpoint, refrencepoint);
		double mindis2 = (pj - refrencepoint).len2();
		if (mindis2 > k * k) return std::make_pair(1e100, 0);
		double dlength = sqrt(k * k - mindis2);
		double midpos = (pj - startpoint).len();
		double revpos = (pj - endpoint).len();
		double totlength = (endpoint - startpoint).len();
		dlength /= totlength;
		midpos /= totlength;
		revpos /= totlength;
		if (midpos + revpos > 1 - EPS && revpos > midpos)
			midpos *= -1;
		//printf("%f %f %f %f [%f %f] %f %f\n", startpoint.x, startpoint.y, endpoint.x, endpoint.y, refrencepoint.x, refrencepoint.y, pj.x, pj.y);
		//printf("%f %f\n", midpos, dlength);
		double left = midpos - dlength, right = midpos + dlength;
		if (left > 1 || right < 0)
			return std::make_pair(1e100, 0);
		return std::make_pair(left < 0 ? 0 : left, right > 1 ? 1 : right);
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
#ifdef DEBUG
	std::vector<fur> dfsnum;
#endif
	bool frechetdistancevalid_dfs(Trajectory x, Trajectory y, double distance, double *dfsleft, double *dfsdown, std::pair<int ,int> *dfsheap, int *dfsstage, char *dfsleftchar, char * dfsdownchar, int *dfsclearsize)
	{
		int n = x.data.size(), m = y.data.size();
		if ((x[0] - y[0]).len() > distance + EPS || (x[n - 1] - y[m - 1]).len() > distance + EPS) {
#ifdef DEBUG
			dfsnum.push_back(fur(n, m, 0, distance));
#endif
			return 0;
		}
		/*
		std::vector<std::vector<double>> left, down;
		left.resize(n);
		down.resize(n);
		for (int i = 0; i < n; i++) {
			left[i].resize(m);
			down[i].resize(m);
			for (int j = 0; j < m; j++) {
				left[i][j] = down[i][j] = 1e100;
			}
		}
		*/
		if (*dfsclearsize < n * m)
			*dfsclearsize = n * m;
		if (dfsleftchar[0] == 0x7f) {
			memset(dfsleftchar, 0, sizeof(char) * *dfsclearsize);
			memset(dfsdownchar, 0, sizeof(char) * *dfsclearsize);
			*dfsclearsize = n * m;
		}
		//memset(dfsleft, 0x58, sizeof(double) * n * m);
		//memset(dfsdown, 0x58, sizeof(double) * n * m);
		int totalnum = 0, topzero = 0;
		//dfsmain(0, 0, totalnum, n, m, x, y, distance, topzero);
		dfsquick(totalnum, n, m, dfsleft, dfsdown, dfsheap, dfsstage, dfsleftchar, dfsdownchar, x, y, distance);
		//printf("%s %s\n", x.name.c_str(), y.name.c_str());
		//getchar();
#ifdef DEBUG
		dfsnum.push_back(fur(n, m, totalnum, distance));
#endif
		return dfsleftchar[(n - 2) * m + m - 2] == dfsleftchar[0] && dfsleft[(n - 2) * m + m - 2] < 1 + EPS || dfsdownchar[(n - 2) * m + m - 2] == dfsleftchar[0] && dfsdown[(n - 2) * m + m - 2] < 1 + EPS;
	}

	void dfsquick(int &totalnum, int n, int m, double *dfsleft, double *dfsdown, std::pair<int, int> *dfsheap, int *dfsstage, char *dfsleftchar, char *dfsdownchar, Trajectory &x, Trajectory &y, double distance) {
		int heaptop = 0, topzero = 0, nowchar = dfsleftchar[0] + 1;
		dfsheap[0].first = dfsheap[0].second = dfsstage[0] = 0;
		dfsleft[0] = dfsdown[0] = 0;
		dfsleftchar[0] = dfsdownchar[0] = nowchar;
		bool disconnecttest = 1;
		for (; heaptop >= 0; ) {
			int i = dfsheap[heaptop].first, j = dfsheap[heaptop].second;

			if (dfsleftchar[i * m + j] != nowchar) {
				dfsleftchar[i * m + j] = nowchar;
				dfsleft[i * m + j] = 1e100;
			}
			if (dfsdownchar[i * m + j] != nowchar) {
				dfsdownchar[i * m + j] = nowchar;
				dfsdown[i * m + j] = 1e100;
			}
			if (dfsleftchar[(i + 1) * m + j] != nowchar) {
				dfsleftchar[(i + 1) * m + j] = nowchar;
				dfsleft[(i + 1) * m + j] = 1e100;
			}
			if (dfsdownchar[i * m + j + 1] != nowchar) {
				dfsdownchar[i * m + j + 1] = nowchar;
				dfsdown[i * m + j + 1] = 1e100;
			}
			if ((dfsleft[i * m + j] == 0 || dfsdown[i * m + j] == 0) && i == topzero)
				topzero++;
			if (topzero >= n - 1 && i < n - 2)
				dfsstage[heaptop] = 2;
			if (dfsleftchar[(n - 2) * m + m - 2] == nowchar && dfsleft[(n - 2) * m + m - 2] < 1 + EPS || dfsdownchar[(n - 2) * m + m - 2] == nowchar && dfsdown[(n - 2) * m + m - 2] < 1 + EPS)
				dfsstage[heaptop] = 2;
			if (i == n - 2 && j == m - 2)
				dfsstage[heaptop] = 2;
			if (i > n - 2 || j > m - 2)
				dfsstage[heaptop] = 2;
			totalnum++;
			//printf("%d %d %d %f %f %d\n", heaptop, i, j, dfsdown[i*m+j], dfsleft[i*m+j], dfsstage[heaptop]);

			if (dfsstage[heaptop] == 0) {
				dfsstage[heaptop] = 1;
				std::pair<double, double> horizontal = getminvalidpos(x[i], x[i + 1], y[j + 1], distance);
				double pastdown = dfsdown[i * m + j + 1] > 1 ? 1 + EPS : dfsdown[i * m + j + 1];
				if (dfsleft[i * m + j] < 1 + EPS || dfsdown[i * m + j] <= horizontal.first)
					dfsdown[i * m + j + 1] = horizontal.first;
				else if (dfsdown[i * m + j] <= horizontal.second + EPS)
					dfsdown[i * m + j + 1] = dfsdown[i * m + j];
				if (dfsdown[i * m + j + 1] >= pastdown)
					dfsdown[i * m + j + 1] = pastdown;
				else {
					dfsstage[++heaptop] = 0;
					dfsheap[heaptop].first = i;
					dfsheap[heaptop].second = j + 1;
					continue;
				}
			}

			if (dfsstage[heaptop] == 1) {
				dfsstage[heaptop] = 2;
				std::pair<double, double> vertical = getminvalidpos(y[j], y[j + 1], x[i + 1], distance);
				double pastleft = dfsleft[(i + 1) * m + j] > 1 ? 1 + EPS : dfsleft[(i + 1) * m + j];
				if (dfsdown[i * m + j] < 1 + EPS || dfsleft[i * m + j] <= vertical.first)
					dfsleft[(i + 1) * m + j] = vertical.first;
				else if (dfsleft[i * m + j] <= vertical.second + EPS)
					dfsleft[(i + 1) * m + j] = dfsleft[i * m + j];
				if (dfsleft[(i + 1) * m + j] >= pastleft)
					dfsleft[(i + 1) * m + j] = pastleft;
				else {
					dfsstage[++heaptop] = 0;
					dfsheap[heaptop].first = i + 1;
					dfsheap[heaptop].second = j;
					continue;
				}
			}

			heaptop--;

		}
	}

	//deprecated
	void dfsmain(int i, int j, int &totalnum, int n, int m, std::vector<std::vector<double>> &left, std::vector<std::vector<double>> &down, Trajectory &x, Trajectory &y, double distance, int &topzero)
	{
		if ((left[i][j] == 0 || down[i][j] == 0) && i == topzero)
			topzero++;
		if (topzero >= n - 1 && i < n - 2) {
			//printf("early break\n");
			return;
		}
		if (left[n - 2][m - 2] < 1 + EPS || down[n - 2][m - 2] < 1 + EPS) return;
		if (i == n - 2 && j == m - 2) return;
		if (i > n - 2 || j > m - 2) return;
		totalnum++;
		std::pair<double, double> horizontal = getminvalidpos(x[i], x[i + 1], y[j + 1], distance);
		std::pair<double, double> vertical = getminvalidpos(y[j], y[j + 1], x[i + 1], distance);
		double pastdown = down[i][j + 1] > 1 ? 1 + EPS : down[i][j + 1];
		if (left[i][j] < 1 + EPS || down[i][j] <= horizontal.first)
			down[i][j + 1] = horizontal.first;
		else if (down[i][j] <= horizontal.second + EPS)
			down[i][j + 1] = down[i][j];
		if (down[i][j + 1] < pastdown)
			dfsmain(i, j + 1, totalnum, n, m, left, down, x, y, distance, topzero);
		else down[i][j + 1] = pastdown;
		double pastleft = left[i + 1][j] > 1 ? 1 + EPS : left[i + 1][j];
		if (down[i][j] < 1 + EPS || left[i][j] <= vertical.first)
			left[i + 1][j] = vertical.first;
		else if (left[i][j] <= vertical.second + EPS)
			left[i + 1][j] = left[i][j];
		if (left[i + 1][j] < pastleft)
			dfsmain(i + 1, j, totalnum, n, m, left, down, x, y, distance, topzero);
		else left[i + 1][j] = pastleft;
	}

	//deprecated
	void dfsmain(int i, int j, int &totalnum, int n, int m, double *dfsleft, double *dfsdown, Trajectory &x, Trajectory &y, double distance, int &topzero)
	{
		if ((dfsleft[i * m + j] == 0 || dfsdown[i * m + j] == 0) && i == topzero)
			topzero++;
		if (topzero >= n - 1 && i < n - 2) {
			//printf("early break\n");
			return;
		}
		if (dfsleft[(n - 2) * m + m - 2] < 1 + EPS || dfsdown[(n - 2) * m + m - 2] < 1 + EPS) return;
		if (i == n - 2 && j == m - 2) return;
		if (i > n - 2 || j > m - 2) return;
		totalnum++;
		std::pair<double, double> horizontal = getminvalidpos(x[i], x[i + 1], y[j + 1], distance);
		std::pair<double, double> vertical = getminvalidpos(y[j], y[j + 1], x[i + 1], distance);
		double pastdown = dfsdown[i * m + j + 1] > 1 ? 1 + EPS : dfsdown[i * m + j + 1];
		if (dfsleft[i * m + j] < 1 + EPS || dfsdown[i * m + j] <= horizontal.first)
			dfsdown[i * m + j + 1] = horizontal.first;
		else if (dfsdown[i * m + j] <= horizontal.second + EPS)
			dfsdown[i * m + j + 1] = dfsdown[i * m + j];
		if (dfsdown[i * m + j + 1] < pastdown)
			dfsmain(i, j + 1, totalnum, n, m, dfsleft, dfsdown, x, y, distance, topzero);
		else dfsdown[i * m + j + 1] = pastdown;
		double pastleft = dfsleft[(i + 1) * m + j] > 1 ? 1 + EPS : dfsleft[(i + 1) * m + j];
		if (dfsdown[i * m + j] < 1 + EPS || dfsleft[i * m + j] <= vertical.first)
			dfsleft[(i + 1) * m + j] = vertical.first;
		else if (dfsleft[i * m + j] <= vertical.second + EPS)
			dfsleft[(i + 1) * m + j] = dfsleft[i * m + j];
		if (dfsleft[(i + 1) * m + j] < pastleft)
			dfsmain(i + 1, j, totalnum, n, m, dfsleft, dfsdown, x, y, distance, topzero);
		else dfsleft[(i + 1) * m + j] = pastleft;
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
	std::vector<int> onequery(Query q, double *dfsleft, double *dfsdown, std::pair<int, int> *dfsheap, int *dfsstage, char *dfsleftchar, char *dfsdownchar, int *dfsclearsize) {
		std::vector<int> res, checklist;
		Index::search(q, starttree, endtree, checklist, res);
		for (auto i : checklist) {
			//printf("%d: ", i);
			//if (discretefrechetdiatance(q.traj, traj[i]) <= q.k)
			//bool a = frechetdistancevalid(q.traj, traj[i], q.k);
			bool another = frechetdistancevalid_dfs(q.traj, traj[i], q.k, dfsleft, dfsdown, dfsheap, dfsstage, dfsleftchar, dfsdownchar, dfsclearsize);
			//assert(a == another);
			if (another) {
				res.push_back(i);
			}
		}
		return res;
	}
}