#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "omp.h"
#include "geo.h"
#include "input.h"
#include "query.h"

double **dfsleft, **dfsdown;
std::pair<int, int> **dfsheap;
int **dfsstage;
char **dfsleftchar, **dfsdownchar, **dfspoint;
int *dfsclearsize;

#ifdef USE_PTHREAD
#include <pthread.h>

std::vector<input::Query> queryres;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t notfull = PTHREAD_COND_INITIALIZER;
pthread_cond_t notempty = PTHREAD_COND_INITIALIZER;
int nextid = -1;

void* consume(void* arg){
	long long arrayid = (long long)arg;
	for (;;){
		pthread_mutex_lock(&mutex);
		while (nextid == -1){
			pthread_cond_wait(&notempty, &mutex);
		}
		int i = nextid;
		nextid = -1;
		pthread_cond_signal(&notfull);
		pthread_mutex_unlock(&mutex);

		if (i < -1) return 0;

		char buffer[input::BUFFER_LENGTH];
		sprintf(buffer, "result-%04d.txt", i);
		printf("running %d, threadnum %lld\n", i, arrayid);
		FILE *f = fopen((input::datapath + "/results/" + buffer).c_str(), "w");
		auto res3 = query::onequery(queryres[i], dfsleft[arrayid], dfsdown[arrayid], dfsheap[arrayid], dfsstage[arrayid], dfsleftchar[arrayid], dfsdownchar[arrayid], dfsclearsize + arrayid, dfspoint[arrayid]);
		for (auto &i : res3)
			fprintf(f, "%s\n", query::traj[i].name.c_str());
		fclose(f);
	}
}

#endif

void calcmain(){
	input::getconfig("C:/Users/zyr17/Documents/Lab/SIGSPATIAL 2017/Datas/data.config");
	auto res = input::readtraindata();
	auto res2 = input::readquerydata();
	query::traj = res;
	Index::makeindex(res, query::starttree, query::endtree);

	const int THREADS = omp_get_max_threads();

	dfsleft = new double*[THREADS];
	dfsdown = new double*[THREADS];
	dfsheap = new std::pair<int, int>*[THREADS];
	dfsstage = new int*[THREADS];
	dfsleftchar = new char*[THREADS];
	dfsdownchar = new char*[THREADS];
	dfspoint = new char*[THREADS];
	dfsclearsize = new int[THREADS];
	memset(dfsclearsize, 0, sizeof(int) * THREADS);

	for (int i = 0; i < THREADS; i++){
		dfsleft[i] = new double[query::MAX_DFS_ARRAY];
		dfsdown[i] = new double[query::MAX_DFS_ARRAY];
		dfsheap[i] = new std::pair<int, int>[query::MAX_DFS_ARRAY];
		dfsstage[i] = new int[query::MAX_DFS_ARRAY];
		dfsleftchar[i] = new char[query::MAX_DFS_ARRAY];
		dfsdownchar[i] = new char[query::MAX_DFS_ARRAY];
		dfspoint[i] = new char[query::MAX_DFS_ARRAY];
		memset(dfsleftchar[i], 0, sizeof(char) * query::MAX_DFS_ARRAY);
		memset(dfsdownchar[i], 0, sizeof(char) * query::MAX_DFS_ARRAY);
		memset(dfspoint[i], 0, sizeof(char) * query::MAX_DFS_ARRAY);
	}

#ifdef USE_PTHREAD
	queryres = res2;
	pthread_t *thids = new pthread_t[THREADS];
	printf("thread %d\n", THREADS);
	for (int i = 0; i < THREADS; i++)
		pthread_create(&thids[i], NULL, consume, (void*)i);

	for (int i = 0; i < res2.size() + THREADS; i++){
		pthread_mutex_lock(&mutex);
		while (nextid != -1){
			pthread_cond_wait(&notfull, &mutex);
		}
		nextid = i < res2.size() ? i : -2;
		pthread_cond_signal(&notempty);
		pthread_mutex_unlock(&mutex);
	}
	long long ret;
	for (int i = 0; i < THREADS; i++)
		pthread_join(thids[i], (void**)&ret);

#else

#pragma omp parallel for
	for (int i = 0; i < res2.size(); i++){
		char buffer[input::BUFFER_LENGTH];
		sprintf(buffer, "result-%04d.txt", i);
		FILE *f = fopen((input::datapath + "/results/" + buffer).c_str(), "w");
		int threadnum = omp_get_thread_num();
		auto res3 = query::onequery(res2[i], dfsleft[threadnum], dfsdown[threadnum], dfsheap[threadnum], dfsstage[threadnum], dfsleftchar[threadnum], dfsdownchar[threadnum], dfsclearsize + threadnum, dfspoint[threadnum]);
		for (auto &i : res3)
			fprintf(f, "%s\n", query::traj[i].name.c_str());
		fclose(f);
#ifdef DEBUG
		printf("%d %s %d %f\n", i, res2[i].traj.name.c_str(), res3.size(), res2[i].k);
		if (res3.size()){
			//printf("draw %s\n", query::traj[res3[0]].name.c_str());
			//graphics::drawdistancepicture(res2[i].traj, query::traj[res3[0]], res2[i].k, "");
		}
#else
		printf("running %d\n", i);
#endif
	}
#ifdef DEBUG
	printf("dfsnum size: %d\n", query::dfsnum.size());
	std::sort(query::dfsnum.begin(), query::dfsnum.end(), [](fur x, fur y) {return x.c < y.c; });
	for (int i = 0; i < query::dfsnum.size(); i += query::dfsnum.size() / 1000)
		printf("%d %d %d %d\n", query::dfsnum[i].c / (query::dfsnum[i].a + query::dfsnum[i].b), query::dfsnum[i].a, query::dfsnum[i].b, query::dfsnum[i].c);
	long long tot = 0;
	for (auto i : query::dfsnum)
		tot += i.c;
	printf("total: %lld\n", tot);
	getchar();
#endif

#endif
}

int main(){
	calcmain();
}