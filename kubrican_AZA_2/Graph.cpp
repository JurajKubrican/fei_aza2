#include <set>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>

#include <set>


#define ACCURACY 0.2

using namespace std;

class Vertex {
private:
	string fname;
	vector<int> contents;
	set<Vertex*> neighbours;
	bool visited = false;

public:
	Vertex(string fname , vector<int> contents) {
		this->fname = fname;
		this->contents = contents;
	}
	void add_neighbour(Vertex* pnewVert) {
		neighbours.insert(pnewVert);
	}

	set<Vertex*> get_neighbours() {
		return neighbours;
	}

	vector<int> getContents() {
		return this->contents;
	}

	string get_name() {
		return this->fname;
	}
};


class Graph {

public:
	void add_edge(string v1_c, string v2_c) {
		// Najst vrcholy
		Vertex* v1 = get_Vertex(v1_c);
		Vertex* v2 = get_Vertex(v2_c);

		// Ak neexistuju, vytvorit ich
		if (v1 == NULL) {
			cout << "ERROR v1 does not exist";
		}
		if (v2 == NULL) {
			cout << "ERROR v2 does not exist";
		}

		// Vytvorit hranu
		v1->add_neighbour(v2);
		v2->add_neighbour(v1);
	}

	Vertex* get_Vertex(string fname) {
		if (vertices.find(fname) != vertices.end())
			return vertices[fname];
		else
			return NULL;
	}

	Vertex* add_new_vertex(string fname, vector<int> contents) {
		return (vertices[fname] = new Vertex(fname, contents));
	}

	void cliqueFrom(string fname) {
		calcNeighbors( fname, 20);
		

		//return NULL;
	}

	void calcNeighbors(string fname,int perc) {

		Vertex * v1 = this->get_Vertex(fname);
		
		for (auto it = this->vertices.begin(); it != vertices.end(); it++) {
			if ( it->second == v1)
				continue;

			if (is_neighbor_LCS(v1->getContents(),v1->getContents())) {
				v1->add_neighbour(it->second);
			}
			else {
				;
			}
		}
	
	}

	void writeout() {

		for (map<string, Vertex* >::iterator it = vertices.begin(); it != vertices.end(); it++) {
			set<Vertex*> tmp = it->second->get_neighbours();
			for (set<Vertex*>::iterator tit = tmp.begin(); tit != tmp.end(); tit++) {
				cout << it->first << endl;
			}
		}


	}

private:
	map<string, Vertex* > vertices;

	set<Vertex*> my_intersect(set<Vertex*> a, set<Vertex*> b) {
		set<Vertex*> intersect;
		set_intersection(a.begin(), a.end(), b.begin(), b.end(),
			std::inserter(intersect, intersect.begin()));
		return intersect;
	}

	int is_neighbor_LCS(const vector<int> & X, const vector<int> & Y)
	{
		int m = X.size();
		int n = Y.size();
		int result = 0;  
		int** LCSuff = new int*[m + 1];
		for (int i = 0; i <= m; i++)
			LCSuff[i] = new int[n + 1];

		for (int i = 0; i <= m; i++)
		{
			for (int j = 0; j <= n; j++)
			{
				if (i == 0 || j == 0)
					LCSuff[i][j] = 0;
				else if (X[i - 1] == Y[j - 1])
				{
					LCSuff[i][j] = LCSuff[i - 1][j - 1] + 1;
					result = max(result, LCSuff[i][j]);
				}
				else LCSuff[i][j] = 0;
			}
		}
		for (int i = 0; i <= m; i++)
			delete LCSuff[i]; 
		delete[] LCSuff;

		return (result >= (ACCURACY * min(n, m)));

	}






};;
