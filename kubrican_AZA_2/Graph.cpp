#include <set>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
//#include "LCS.cpp"
#include <algorithm>
#include <set>


using namespace std;

class Vertex {
private:
	string fname;
	string contents;
	set<Vertex*> neighbours;
	bool visited = false;

public:
	Vertex(string fname , string contents) {
		this->fname = fname;
		this->contents = contents;
	}
	void add_neighbour(Vertex* pnewVert) {
		neighbours.insert(pnewVert);
	}
	set<Vertex*> get_neighbours(set<Vertex*>possible) {
		if (this->visited) {
			set<Vertex*> intersect;
			set_intersection(possible.begin(), possible.end(), this->neighbours.begin(), this->neighbours.end(),
			std::inserter(intersect, intersect.begin()));
			//return intersect;
		}
		else {
		}
		return neighbours;
	}
};


class Graph {

private:
	map<string, Vertex* > vertices;

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

	Vertex* add_new_vertex(string fname, string contents) {
		return (vertices[fname] = new Vertex(fname, contents));
	}

	
	/*
	void writeout(string file) {
		ofstream ofs(file);
		vector<pair<pair<int, int>, pair<int, int>>>::iterator it;
		for (it = result.begin(); it != result.end(); it++) {
			ofs << "[" << it->first.first << "," << it->first.second << "] ";
			ofs << "[" << it->second.first << "," << it->second.second << "]" << endl;
		}
		ofs.close();

	}

	void MATwriteout() {
		ofstream ofs("matout.txt");
		vector<pair<pair<int, int>, pair<int, int>>>::iterator it;
		for (it = result.begin(); it != result.end(); it++) {
			ofs << it->first.first << " " << it->first.second << " ";
			ofs << it->second.first << " " << it->second.second << endl;
		}
		ofs.close();
	}*/
};;
