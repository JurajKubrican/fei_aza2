#include <map>

#include <thread>
#include <mutex>
#include <queue>


#define ACCURACY 0.2
#define NUM_THREADS 8
using namespace std;



class MyThreadPool {


public:
	
	MyThreadPool(vector<int>A,map<string, vector<int>>B) {
		for (map<string, vector<int>>::iterator it = B.begin(); it != B.end(); it++) {
			inputForThreads.push(make_pair(it->first,make_pair(A,it->second)));
		}

		vector<thread> threads;
		for (int i = 0; i < NUM_THREADS; i++) {
			threads.push_back(thread(&MyThreadPool::thread_worker,this));
		}

		for (vector<std::thread>::iterator it = threads.begin(); it != threads.end(); it++) {
			(*it).join();
		} 
	}

	map<string, int>get_result() {
		return this->outputForThreads;
	}

private:
	mutex inputMutex;
	mutex outputMutex;
	queue<pair<string, pair<vector<int>, vector<int>>>> inputForThreads;
	map<string, int> outputForThreads;


	void thread_worker() {
		while (1) {
			inputMutex.lock();
			if (this->inputForThreads.empty()) {
				inputMutex.unlock();
				return;
			}
			pair<string, pair<vector<int>, vector<int>>> input = this->inputForThreads.front();
			inputForThreads.pop();
			inputMutex.unlock();

			int result = is_neighbor_LCS(input.second);

			outputMutex.lock();
			this->outputForThreads.insert(make_pair(input.first, result));
			outputMutex.unlock();
		}

	}

	bool is_neighbor_LCS(pair<vector<int>, vector<int>> in)
	{
		const vector<int> str1 = in.first;
		const vector<int> str2 = in.second;
		size_t str1size = str1.size();
		size_t str2size = str2.size();
		int minSizeTreshold = ceil(ACCURACY * min(str1size, str2size));

		if (str1.empty() || str2.empty())
			return 0;

		int *curr = new int[str2size];
		int *prev = new int[str2size];
		int *swap = nullptr;
		int maxSubstr = 0;

		for (uint16_t i = 0; i < str1size; ++i) {
			for (uint16_t j = 0; j<str2size; ++j) {
				if (str1[i] != str2[j]) {
					curr[j] = 0;
				}
				else {
					if (i == 0 || j == 0) {
						curr[j] = 1;
					}
					else {
						curr[j] = 1 + prev[j - 1];
					}
					//The next if can be replaced with:
					//maxSubstr = max(maxSubstr, curr[j]);
					//(You need algorithm.h library for using max())
					if (maxSubstr < curr[j]) {
						maxSubstr = curr[j];
						if (maxSubstr >= minSizeTreshold) {
							return true;
						}

					}

				}
			}
			swap = curr;
			curr = prev;
			prev = swap;
		}
		delete[] curr;
		delete[] prev;
		return false;
	}


};