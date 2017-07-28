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
		double left = midpos - dlength, right = midpos + dlength;
		if (left > 1 || right < 0)
			return std::make_pair(1e100, 0);
		return std::make_pair(left < 0 ? 0 : left, right > 1 ? 1 : right);
	}

#ifdef DEBUG
	std::vector<fur> dfsnum;
#endif
	bool frechetdistancevalid_dfs(Trajectory x, Trajectory y, double distance, double *dfsleft, double *dfsdown, std::pair<int ,int> *dfsheap, int *dfsstage, char *dfsleftchar, char *dfsdownchar, int *dfsclearsize, char *dfspoint)
	{
		int n = x.data.size(), m = y.data.size();
		if ((x[0] - y[0]).len() > distance + EPS || (x[n - 1] - y[m - 1]).len() > distance + EPS){
#ifdef DEBUG
			dfsnum.push_back(fur(n, m, 0, distance));
#endif
			return 0;
		}
		if (*dfsclearsize < n * m)
			*dfsclearsize = n * m;
		if (dfsleftchar[0] == 0x7f){
			memset(dfsleftchar, 0, sizeof(char) * *dfsclearsize * 2);
			memset(dfsdownchar, 0, sizeof(char) * *dfsclearsize * 2);
			memset(dfspoint, 0, sizeof(char) * *dfsclearsize * 2);
			*dfsclearsize = n * m;
		}
		int totalnum = 0, topzero = 0;
		dfsquick(totalnum, dfsleft, dfsdown, dfsheap, dfsstage, dfsleftchar, dfsdownchar, dfspoint, x, y, distance);
#ifdef DEBUG
		dfsnum.push_back(fur(n, m, totalnum, distance));
#endif
		return dfsleftchar[(n - 2) * m + m - 2] == dfsleftchar[0] && dfsleft[(n - 2) * m + m - 2] < 1 + EPS || dfsdownchar[(n - 2) * m + m - 2] == dfsleftchar[0] && dfsdown[(n - 2) * m + m - 2] < 1 + EPS;
	}

	void dfsquick(int &totalnum, double *dfsleft, double *dfsdown, std::pair<int, int> *dfsheap, int *dfsstage, char *dfsleftchar, char *dfsdownchar, char *dfspoint, Trajectory &x, Trajectory &y, double distance){
		int n = x.data.size(), m = y.data.size();
		int heaptop = 0, topzero = 0, nowchar = dfsleftchar[0] + 1;
		dfsheap[0].first = dfsheap[0].second = dfsstage[0] = 0;
		dfsleft[0] = dfsdown[0] = 0;
		dfsleftchar[0] = dfsdownchar[0] = nowchar;
		bool disconnecttest = 1;
		for (; heaptop >= 0; ){
			int i = dfsheap[heaptop].first, j = dfsheap[heaptop].second;

			if (dfsleftchar[i * m + j] != nowchar){
				dfsleftchar[i * m + j] = nowchar;
				dfsleft[i * m + j] = 1e100;
			}
			if (dfsdownchar[i * m + j] != nowchar){
				dfsdownchar[i * m + j] = nowchar;
				dfsdown[i * m + j] = 1e100;
			}
			if (dfsleftchar[(i + 1) * m + j] != nowchar){
				dfsleftchar[(i + 1) * m + j] = nowchar;
				dfsleft[(i + 1) * m + j] = 1e100;
			}
			if (dfsdownchar[i * m + j + 1] != nowchar){
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

			if (dfsstage[heaptop] == 0){
				dfsstage[heaptop] = 1;
				std::pair<double, double> horizontal = getminvalidpos(x[i], x[i + 1], y[j + 1], distance);

				if (i == 0 && disconnecttest && horizontal.first == 1e100){
					disconnecttest = 0;
					dfsheap[n * m].first = i;
					dfsheap[n * m].second = j + 1;
					dfsstage[n * m] = 0;
					dfspoint[i * m + j + 1] = nowchar;
					if (dfsdisconnect(totalnum, dfsheap + n * m, dfsstage + n * m, dfspoint, x, y, distance)){
						topzero = n - 1;
					}
				}

				double pastdown = dfsdown[i * m + j + 1] > 1 ? 1 + EPS : dfsdown[i * m + j + 1];
				if (dfsleft[i * m + j] < 1 + EPS || dfsdown[i * m + j] <= horizontal.first)
					dfsdown[i * m + j + 1] = horizontal.first;
				else if (dfsdown[i * m + j] <= horizontal.second + EPS)
					dfsdown[i * m + j + 1] = dfsdown[i * m + j];
				if (dfsdown[i * m + j + 1] >= pastdown)
					dfsdown[i * m + j + 1] = pastdown;
				else{
					dfsstage[++heaptop] = 0;
					dfsheap[heaptop].first = i;
					dfsheap[heaptop].second = j + 1;
					continue;
				}
			}

			if (dfsstage[heaptop] == 1){
				dfsstage[heaptop] = 2;
				std::pair<double, double> vertical = getminvalidpos(y[j], y[j + 1], x[i + 1], distance);

				if (j == m - 2 && disconnecttest && vertical.first == 1e100){
					disconnecttest = 0;
					dfsheap[n * m].first = i + 1;
					dfsheap[n * m].second = j;
					dfsstage[n * m] = 0;
					dfspoint[(i + 1) * m + j] = nowchar;
					if (dfsdisconnect(totalnum, dfsheap + n * m, dfsstage + n * m, dfspoint, x, y, distance)){
						topzero = n - 1;
					}
				}

				double pastleft = dfsleft[(i + 1) * m + j] > 1 ? 1 + EPS : dfsleft[(i + 1) * m + j];
				if (dfsdown[i * m + j] < 1 + EPS || dfsleft[i * m + j] <= vertical.first)
					dfsleft[(i + 1) * m + j] = vertical.first;
				else if (dfsleft[i * m + j] <= vertical.second + EPS)
					dfsleft[(i + 1) * m + j] = dfsleft[i * m + j];
				if (dfsleft[(i + 1) * m + j] >= pastleft)
					dfsleft[(i + 1) * m + j] = pastleft;
				else{
					dfsstage[++heaptop] = 0;
					dfsheap[heaptop].first = i + 1;
					dfsheap[heaptop].second = j;
					continue;
				}
			}

			heaptop--;

		}
	}

	bool dfsdisconnect(int &totalnum, std::pair<int, int> *dfsheap, int *dfsstage, char *dfspoint, Trajectory &x, Trajectory &y, double distance){
		return 0;
		int n = x.data.size(), m = y.data.size();
		char nowchar = dfspoint[dfsheap[0].first * m + dfsheap[0].second];
		int heaptop = 0, time = (n + m) * 2;
		for (; heaptop >= 0; ){
			int i = dfsheap[heaptop].first, j = dfsheap[heaptop].second;
			if (i == n - 1 || j == 0) return 1;
			if (!--time){
				return 0;
			}
			totalnum++;
			if (dfsstage[heaptop] == 0){
				dfsstage[heaptop] = 1;
				std::pair<double, double> p = getminvalidpos(y[j - 1], y[j], x[i], distance);
				if (p.first == 1e100 && dfspoint[i * m + j - 1] != nowchar){
					dfspoint[i * m + j - 1] = nowchar;
					dfsstage[++heaptop] = 0;
					dfsheap[heaptop].first = i;
					dfsheap[heaptop].second = j - 1;
					continue;
				}
			}
			if (dfsstage[heaptop] == 1){
				dfsstage[heaptop] = 2;
				std::pair<double, double> p = getminvalidpos(x[i], x[i + 1], y[j], distance);
				if (p.first == 1e100 && dfspoint[(i + 1) * m + j] != nowchar){
					dfspoint[(i + 1) * m + j] = nowchar;
					dfsstage[++heaptop] = 0;
					dfsheap[heaptop].first = i + 1;
					dfsheap[heaptop].second = j;
					continue;
				}
			}
			if (dfsstage[heaptop] == 2){
				dfsstage[heaptop] = 3;
				if (i){
					std::pair<double, double> p = getminvalidpos(x[i - 1], x[i], y[j], distance);
					if (p.first == 1e100 && dfspoint[(i - 1) * m + j] != nowchar){
						dfspoint[(i - 1) * m + j] = nowchar;
						dfsstage[++heaptop] = 0;
						dfsheap[heaptop].first = i - 1;
						dfsheap[heaptop].second = j;
						continue;
					}
				}
			}
			if (dfsstage[heaptop] == 3){
				dfsstage[heaptop] = 4;
				if (j < m - 1){
					std::pair<double, double> p = getminvalidpos(y[j], y[j + 1], x[i], distance);
					if (p.first == 1e100 && dfspoint[i * m + j + 1] != nowchar){
						dfspoint[i * m + j + 1] = nowchar;
						dfsstage[++heaptop] = 0;
						dfsheap[heaptop].first = i;
						dfsheap[heaptop].second = j + 1;
						continue;
					}
				}
			}

			heaptop--;

		}
		return 0;
	}
	std::vector<int> onequery(Query q, double *dfsleft, double *dfsdown, std::pair<int, int> *dfsheap, int *dfsstage, char *dfsleftchar, char *dfsdownchar, int *dfsclearsize, char *dfspoint){
		std::vector<int> res, checklist;
		Index::search(q, starttree, endtree, checklist, res);
		for (auto i : checklist){
			//printf("%d: ", i);
			//if (discretefrechetdiatance(q.traj, traj[i]) <= q.k)
			//bool a = frechetdistancevalid(q.traj, traj[i], q.k);
			bool another = frechetdistancevalid_dfs(q.traj, traj[i], q.k, dfsleft, dfsdown, dfsheap, dfsstage, dfsleftchar, dfsdownchar, dfsclearsize, dfspoint);
			//assert(a == another);
			if (another){
				res.push_back(i);
			}
		}
		return res;
	}
}