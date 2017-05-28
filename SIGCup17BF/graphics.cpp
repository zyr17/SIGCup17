#include "graphics.h"
namespace graphics {

	std::pair<double, double> getminvalidpos(geo::Point startpoint, geo::Point endpoint, geo::Point refrencepoint, double k)
	{
		geo::Point pj = proj(startpoint, endpoint, refrencepoint);
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

	bool drawdistancepicture(Trajectory x, Trajectory y, double distance, std::string filepath, int cellsize) {
		int n = x.data.size(), m = y.data.size();
		if ((x[0] - y[0]).len() > distance + EPS || (x[n - 1] - y[m - 1]).len() > distance + EPS)
			return 0;
		std::vector<std::vector<double>> left, down;
		std::vector<std::vector<std::vector<double>>> drawdis;
		left.resize(n);
		down.resize(n);
		drawdis.resize(n);
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++) {
				left[i].push_back(1e100);
				down[i].push_back(1e100);
			}
		for (int i = 0; i < n; i++) {
			drawdis[i].resize(m);
			for (int j = 0; j < m; j++)
				drawdis[i][j].resize(4);
		}
		left[0][0] = down[0][0] = 0;
		for (int i = 0; i < n - 1; i++)
			for (int j = 0; j < m - 1; j++) {
				if (left[i][j] > 1 + EPS && down[i][j] > 1 + EPS) continue;
				//printf("%d %d\n", i, j);
				std::pair<double, double> horizontal = getminvalidpos(x[i], x[i + 1], y[j + 1], distance);
				std::pair<double, double> vertical = getminvalidpos(y[j], y[j + 1], x[i + 1], distance);
				drawdis[i][j][0] = horizontal.first;
				drawdis[i + 1][j][2] = 1 - horizontal.second;
				drawdis[i][j][1] = vertical.first;
				drawdis[i][j + 1][3] = 1 - vertical.second;
				if (left[i][j] < 1 + EPS || down[i][j] <= horizontal.first)
					down[i][j + 1] = horizontal.first;
				else if (down[i][j] <= horizontal.second + EPS)
					down[i][j + 1] = down[i][j];
				if (down[i][j] < 1 + EPS || left[i][j] <= vertical.first)
					left[i + 1][j] = vertical.first;
				else if (left[i][j] <= vertical.second + EPS)
					left[i + 1][j] = left[i][j];
			}
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				for (int k = 0; k < 4; k++)
					if (drawdis[i][j][k] < 0)
						drawdis[i][j][k] = 0;
					else if (drawdis[i][j][k] > 1)
						drawdis[i][j][k] = 1;
		const int p[4][2] = { { 1, 0 },{ 0, 1 },{ -1, 0 },{ 0, -1 } };
		FILE *drawdata = fopen("drawdata.txt", "w");
		fprintf(drawdata, "%d %d\n", cellsize * (n + 1), cellsize * (m + 1));
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++) {
				int x = (i + 1) * cellsize;
				int y = (j + 1) * cellsize;
				for (int k = 0; k < 4; k++) {
					//fprintf(drawdata, "%f ", drawdis[i][j][k]);
					fprintf(drawdata, "%d %d ", int(x + p[k][0] * cellsize * drawdis[i][j][k]), int(y + p[k][1] * cellsize * drawdis[i][j][k]));
				}
				fprintf(drawdata, "\n");
			}
		/*for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++)
				fprintf(drawdata, "%d %d ", left[i][j] < 0 || left[i][j] > 1 ? 0 : 1, down[i][j] < 0 || down[i][j] > 1 ? 0 : 1);
			fprintf(drawdata, "\n");
		}*/
		fclose(drawdata);

		return left[n - 2][m - 2] <= 1 + EPS || down[n - 2][m - 2] <= 1 + EPS;
	}
}