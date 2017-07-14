#include "kdtree.h"

namespace kdt {

	void twod_tree::ini(int now, int l, int r, int base) {
		int p = (l + r) / 2;
		if (base) std::nth_element(points.begin() + l, points.begin() + p, points.begin() + r, [](fpoint x, fpoint y) {return x.y < y.y || x.y == y.y && x.x < y.x; });
		else std::nth_element(points.begin() + l, points.begin() + p, points.begin() + r, [](fpoint x, fpoint y) {return x.x < y.x || x.x == y.x && x.y < y.y; });
		for (int i = l; i < r; i++) {
			if (points[i].x < minx[now]) minx[now] = points[i].x;
			if (points[i].y < miny[now]) miny[now] = points[i].y;
			if (points[i].x > maxx[now]) maxx[now] = points[i].x;
			if (points[i].y > maxy[now]) maxy[now] = points[i].y;
		}
		tree[now] = p;
		if (l < p) ini(now * 2, l, p, base ^ 1);
		if (p < r - 1) ini(now * 2 + 1, p + 1, r, base ^ 1);
	}
	inline void twod_tree::p_add(int pos, fpoint o, int totnum, std::vector<int> &res) {
		auto heap_opr = [=](int x, int y) {return (points[x] - o).len2() < (points[y] - o).len2(); };
		if (res.size() < totnum) {
			res.push_back(pos);
			if (res.size() == totnum) std::make_heap(res.begin(), res.end(), heap_opr);
			return;
		}
		if ((points[pos] - o).len2() < (points[res[0]] - o).len2()) {
			std::pop_heap(res.begin(), res.end(), heap_opr);
			res[res.size() - 1] = pos;
			std::push_heap(res.begin(), res.end(), heap_opr);
		}
	}
	inline bool twod_tree::is_in(fpoint point, std::vector<int> &res, int totnum, int checkpos) {
		if (totnum > res.size()) return 1;
		double nowmax = (points[res[0]] - point).len();
		if (minx[checkpos] > point.x + nowmax) return 0;
		if (maxx[checkpos] < point.x - nowmax) return 0;
		if (miny[checkpos] > point.y + nowmax) return 0;
		if (maxy[checkpos] < point.y - nowmax) return 0;
		return 1;
	}
	void twod_tree::pnum(int now, fpoint point, int totnum, std::vector<int> &res, int base) {
		if (now >= tot * 4 + 4 || tree[now] == -1) return;
		int pos = now;
		for (;;) {
			int next;
			fpoint &x = point, &y = points[tree[pos]];
			if (!base) {
				next = pos * 2 + !(x.x < y.x || x.x == y.x && x.y < y.y);
			}
			else {
				next = pos * 2 + !(x.y < y.y || x.y == y.y && x.x < y.x);
			}
			if (next >= tot * 4 + 4) break;
			if (tree[next] == -1) next ^= 1;
			if (tree[next] == -1) break;
			pos = next;
			base ^= 1;
		}
		for (; pos != now;) {
			p_add(tree[pos], point, totnum, res);
			if (is_in(point, res, totnum, pos ^ 1)) pnum(pos ^ 1, point, totnum, res, base);
			base ^= 1;
			pos >>= 1;
		}
		p_add(tree[pos], point, totnum, res);
	}
	void twod_tree::psize(int now, fpoint p1, fpoint p2, std::vector<int> &res, int base) {
		if (now >= tot * 4 + 4) return;
		if (tree[now] == -1) return;
		if ((minx[now] > p2.x || maxx[now] < p1.x) || (miny[now] > p2.y || maxy[now] < p1.y)) return;
		fpoint &p = points[tree[now]];
		int pd = p.x >= p1.x && p.x <= p2.x && p.y >= p1.y && p.y <= p2.y;
		//if (pd) p_add(tree[now], fpoint(0, 0), INT_MAX, res);
		if (pd) res.push_back(tree[now]);
		psize(now * 2, p1, p2, res, base ^ 1);
		psize(now * 2 + 1, p1, p2, res, base ^ 1);
	}
	twod_tree::twod_tree() {
		clear();
	}
	void twod_tree::clear() {
		tot = 0;
		tree.clear();
		points.clear();
		minx.clear();
		miny.clear();
		maxx.clear();
		maxy.clear();
	}
	int twod_tree::make_tree(fpoint *ins, int intot) {
		std::vector<fpoint> tmp;
		for (int i = 0; i < intot; i++)
			tmp.push_back(ins[i]);
		return make_tree(tmp, intot);
	}
	int twod_tree::make_tree(std::vector<fpoint> &ins, int intot) {
		if (~intot) tot = intot;
		else tot = ins.size();
		points = ins;
		for (int i = 0; i < tot; i++)
			points[i].id = i;
		if (!tot) return 0;
		tree.resize(tot * 4 + 4, -1);
		minx.resize(tot * 4 + 4, 1e100);
		miny.resize(tot * 4 + 4, 1e100);
		maxx.resize(tot * 4 + 4, -1e100);
		maxy.resize(tot * 4 + 4, -1e100);
		ini(1, 0, tot, 0);

#ifdef DEBUG_KDT
		for (int i = 0; i < tot; i++)
			printf("[%d %lf %lf %lf %d]\n", i, points[i].x, points[i].y, points[i].len2(), points[i].id);
		printf("\n");
		for (int i = 1; i <= tot * 4; i++)
			if (minx[i] < 1e90) printf("[%d %d %lf %lf %lf %lf]\n", i, tree[i], minx[i], miny[i], maxx[i], maxy[i]);
		printf("\n");
#endif

		return tot;
	}
	std::vector<int> twod_tree::find_by_dis(double x, double y, double maxdis)
	{
		std::vector<int> res;
		find_by_rect(fpoint(x - maxdis, y - maxdis), fpoint(x + maxdis, y + maxdis), res);
		return res;
	}
	int twod_tree::find_by_num(double x, double y, int totnum, std::vector<int> &res) {
		return find_by_num(fpoint(x, y), totnum, res);
	}
	int twod_tree::find_by_num(fpoint point, int totnum, std::vector<int> &res) {
		auto heap_opr = [=](int x, int y) {return (points[x] - point).len2() < (points[y] - point).len2(); };
		res.clear();
		if (!totnum) return 0;
		pnum(1, point, totnum, res, 0);
		std::make_heap(res.begin(), res.end(), heap_opr);
		for (int i = 0; i < res.size(); i++)
			std:pop_heap(res.begin(), res.end() - i, heap_opr);

		for (int i = 0; i < res.size(); i++)
			res[i] = points[res[i]].id;

		return res.size();
	}
	int twod_tree::find_by_rect(double x1, double y1, double x2, double y2, std::vector<int> &res) {
		return find_by_rect(fpoint(x1, y1), fpoint(x2, y2), res);
	}
	int twod_tree::find_by_rect(fpoint poi1, fpoint poi2, std::vector<int> &res) {
		res.clear();
		if (poi1.x > poi2.x) {
			double t = poi1.x;
			poi1.x = poi2.x;
			poi2.x = t;
		}
		if (poi1.y > poi2.y) {
			double t = poi1.y;
			poi1.y = poi2.y;
			poi2.y = t;
		}
		psize(1, poi1, poi2, res, 0);

		for (int i = 0; i < res.size(); i++)
			res[i] = points[res[i]].id;

		return res.size();
	}

}