Compile the program with instruction: "g++ -o main --std=c++11 -Ofast -fopenmp -pthread geo.cpp index.cpp input.cpp kdtree.cpp Main.cpp query.cpp"

Run the program: 
	In file "data.config", there are three lines. First line means the path of data, second line means the position of "dataset.txt", third line means the position of "queries.txt".
	In data folder, there should be a folder named "results" to save results which can be empty. 
	Then run "./main" in terminal.
	To test sample, put "data.config" "sample-data" "main" in a same directory and run "main".
	In "def.h", define DEBUG can output some debug datas; define USE_PTHREAD will use pthreads. If USE_PTHREAD not defined, the program will use OpenMP.
	
Central idea:
	Obviously, two trajectories' fretchet distance is less than k only if their first points' distance is less than k and their last points' distance is less than k. So we use two KD-tree to index all trajectories' first and last points. There's an DP algorithm to check whether two trajectories' fretchet distance is less than k. It costs O(nm) to check two trajectories with length n and m. Use DFS to make this DP more quickly. Use multithreading to speedup.