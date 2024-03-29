#include <set>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>

#include <thread>
#include <future>

#include "MyThreadPool.cpp"

#define ACCURACY 0.2
//TO multithread or not t
#define THREAD_IT 1

using namespace std;

class Vertex {

public:
	Vertex(string fname , vector<int> contents) {
		this->fname = fname;
		this->contents = contents;
	}
	
	//cahced get neighbors
	set<Vertex*> get_neighbours( set<Vertex*> possible) {
		set<Vertex*> result;

		//remove those who are cached not neighbors
		set<Vertex*> notNeighbors = this->get_not_neighbours();
		for (set<Vertex*>::iterator it = notNeighbors.begin(); it != notNeighbors.end(); ) {
			possible.erase(it);
		}


		//remove those who are cached neighbors + add them to result
		set<Vertex*> neighbors = this->get_neighbours();
		for (set<Vertex*>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {
			result.insert(*it);
			possible.erase(*it);
		}

		if (THREAD_IT) {
			//multithreaded prepairation
			map<string, vector<int>> input;
			map<string, Vertex*> ref;
			for (set<Vertex*>::iterator it = possible.begin(); it != possible.end(); it++) {
				if (*it == this)
					continue;
				ref.insert(make_pair((*it)->get_name(), *it));
				input.insert(make_pair((*it)->get_name(), (*it)->get_contents()));
			}


			MyThreadPool threadPool(this->get_contents(), input);
			map<string, int> output = threadPool.get_result();
			//calculate + cache
			for (map<string, int>::iterator it = output.begin(); it != output.end(); it++) {
				if (it->second == 1) {
					//cout <<	ref[it->first]->get_name() << endl;
					result.insert(ref[it->first]);
					this->add_neighbour(ref[it->first]);
				}
				else {
					this->add_not_neighbour(ref[it->first]);
				}
				
			}
		
		}else{
			//single threaded
			//calculate the rest + cache it	
			vector<int> V1C = this->get_contents();
			for (set<Vertex*>::iterator it = possible.begin(); it != possible.end(); it++) {
				if (*it == this)
					continue;

				vector<int>V2C = (*it)->get_contents();
				if (is_neighbor_LCS(make_pair(V1C, V2C))) {
					this->add_neighbour(*it);
					result.insert(*it);
				}
				else {
					this->add_not_neighbour(*it);
				}
			}
		}
		

		return result;
	}


	string get_name() {
		return this->fname;
	}
	int get_size() {
		return this->contents.size();
	}

private:
	string fname;
	vector<int> contents;
	set<Vertex*> neighbours;
	set<Vertex*> not_neighbors;

	set<Vertex*> get_neighbours() {
		return neighbours;
	}

	void add_neighbour(Vertex* pnewVert) {
		neighbours.insert(pnewVert);
		pnewVert->add_neighbour2(this);
	}
	void add_neighbour2(Vertex* pnewVert) {
		neighbours.insert(pnewVert);
	}
	void add_not_neighbour(Vertex* pnewVert) {
		not_neighbors.insert(pnewVert);
		pnewVert->add_not_neighbour2(this);
	}
	void add_not_neighbour2(Vertex* pnewVert) {
		not_neighbors.insert(pnewVert);
	}

	set<Vertex*> get_not_neighbours() {
		return not_neighbors;
	}

	vector<int> get_contents() {
		return this->contents;
	}

	//takes less memory than the originla implemewnation
	int is_neighbor_LCS( pair<vector<int>,vector<int>> in)
	{
		const vector<int> str1 = in.first;
		const vector<int> str2 = in.second;
		int str1size = str1.size();
		int str2size = str2.size();
		//optimalization:
		int minSizeTreshold = ceil(min(str1size, str2size));

		if (str1.empty() || str2.empty())
			return 0;

		int *curr = new int[str2size];
		int *prev = new int[str2size];
		int *swap = nullptr;
		int maxSubstr = 0;

		for (uint16_t i = 0; i < str1size; ++i){
			for (uint16_t j = 0; j<str2size; ++j){
				if (str1[i] != str2[j]){
					curr[j] = 0;
				}else{
					if (i == 0 || j == 0){
						curr[j] = 1;
					}else{
						curr[j] = 1 + prev[j - 1];
					}
					//return immediately after found > 20% 
					if (maxSubstr < curr[j]){
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
		return (maxSubstr >= (ACCURACY * min(str2.size(), str1.size())));
	}
};


class Graph {

public:

	Vertex* get_Vertex(string fname) {
		if (vertices.find(fname) != vertices.end())
			return vertices[fname];
		else
			return NULL;
	}

	Vertex* add_new_vertex(string fname, vector<int> contents) {
		return (vertices[fname] = new Vertex(fname, contents));
	}

	set<Vertex*> cliqueFrom(string fname) {
		
		Vertex * V1 = this->get_Vertex(fname);
		set<Vertex*> all;
		for (map<string, Vertex* >::iterator it = vertices.begin(); it != vertices.end(); it++) {
			all.insert(it->second);
		}
		
		set<Vertex*> iniR;
		iniR.insert(V1);
		set<Vertex*> iniP = V1->get_neighbours(all);
		set<Vertex*> iniX;
		set<Vertex*> max_clique = this->BronKerbosh(iniR, iniP, iniX);
		
		return max_clique;
	}


private:
	set<Vertex*> BronKerbosh(set<Vertex*>R, set<Vertex*>P, set<Vertex*>&X) {
		set<Vertex*>max_clique;
		if (P.size() == 0 && X.size() == 0) {
			cout << "REPORTING AS MAX CLIQUE";
			max_clique = R;
			return max_clique;
		}
		cout << "clique: " << R.size() << "cadidates:" << P.size() << endl;
		//Optimalization: vertex ordering
		map<int, Vertex*> ordered;
		for (set<Vertex*>::iterator it = P.begin(); it != P.end(); it++) {
			ordered.insert(make_pair((*it)->get_size(),(*it)));
		}

		for (map<int,Vertex*>::iterator it = ordered.begin(); it != ordered.end(); it++) {

			//cout << (*it)->get_name();
			set<Vertex*>tmpN = it->second->get_neighbours(P);
			
			set<Vertex*> tmpR(R); tmpR.insert(it->second );
			set<Vertex*> tmpP(my_intersect(P, tmpN));
			set<Vertex*> tmpX = my_intersect(X, tmpN);
			if (tmpR.size() + tmpP.size() > max_clique.size()) {
				set<Vertex*> tmp_clique = BronKerbosh(tmpR, tmpP, tmpX);
				if (max_clique.size() < tmp_clique.size()) {
					max_clique = tmp_clique;
				}
			}
			
		
			
			if(P.find(it->second) != P.end())
				P.erase(it->second);
			X.insert(it->second);
		}
		return max_clique;
	};

	map<string, Vertex* > vertices;

	set<Vertex*> my_intersect(set<Vertex*> a, set<Vertex*> b) {
		set<Vertex*> intersect;
		set_intersection(a.begin(), a.end(), b.begin(), b.end(),
			std::inserter(intersect, intersect.begin()));
		return intersect;
	}




};;
