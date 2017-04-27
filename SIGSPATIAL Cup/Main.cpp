#include "geo.h"
#include "input.h"

int main() {
	auto res = input::readtraindata();
	auto res2 = input::readquerydata();
	printf("%d\n%s %f\n%s %f\n", res2.size(), res2[0].first.c_str(), res2[0].second, res2[1].first.c_str(), res2[1].second);
	getchar();
}