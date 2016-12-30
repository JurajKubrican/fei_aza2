#include <iostream>
#include <fstream>
#include <string>
#include "Graph.cpp"
#include <vector>
#include <regex>
#include <iterator>

//#define RK_BASE 101
//vstup.txt kslova.txt out.txt
//vstup_lipsum.txt kslova_lipsum.txt out.txt

using namespace std;

Graph all;


//loads the keywords from file
map<string,int> loadKeyWords(char * input ) {
	string line;
	map<string, int> out;
	ifstream myfile((string)input);
	int i = 0;
	if (myfile.is_open())
	{
		int linenum = 0;
		while ( getline(myfile, line))
		{
			transform(line.begin(), line.end(), line.begin(), ::tolower);
			out.insert(make_pair(line,i++));
		}
		myfile.close();
	}
	return out;
}

//loads the file list from file
vector<string> loadFileList(char * input) {

	string line;
	ifstream myfile((string)input);
	vector<string> out;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			out.push_back(line);
		}
		myfile.close();
	}
	return out;
}

//loads the file
string loadFile(string input) {

	string line;
	ifstream myfile(input);
	string out;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			transform(line.begin(), line.end(), line.begin(), ::tolower);
			out.append(line);
		}
		myfile.close();
	}
	return out;
}

//prepairs the regex
regex make_regex(map<string, int>keywords) {
	

	string reg = "(";
	for (map<string, int>::iterator it = keywords.begin(); it != keywords.end(); it++) {
		reg = reg + it->first + "|";
	}
	reg = reg.substr(0, reg.size() - 1);
	reg += ")";

	regex words_regex(reg);
	return words_regex;
}

//uses regex to change the file from string into a set of keyword indexes
vector<int> cleanFile(const string &s, map<string, int> &keywords, regex &words_regex) {
	vector<int> output;
	auto words_begin = sregex_iterator(s.begin(), s.end(), words_regex);
	auto words_end = sregex_iterator();
	for (sregex_iterator it = words_begin; it != words_end; ++it) {
		output.push_back(keywords[it->str()]);
	}
	return output;
}

//rabin karp - slow
/*
uint32_t rabin_karp_hash(string str) {
	uint32_t hash = 0;

	for (uint32_t i = 0; i < str.length() - 1; i++) {
		hash += str[i] * (uint32_t)pow(RK_BASE, i);
	}

	return hash;
}

vector<int> rabin_karp_analyze(const string &s, vector<string> keywords ) {
	vector<int>results;
	vector<uint32_t>key_hashes;
	for (auto it = keywords.begin(); it != keywords.end(); it++) {
		key_hashes.push_back(rabin_karp_hash(*it));
	}

	for (uint32_t i = 0; i < s.length() - 1; i++) {
		for (uint32_t j = 0; j< keywords.size(); j++) {
			string piece = s.substr(i, keywords[j].size());
			uint32_t tmp = rabin_karp_hash(piece);
			if (tmp == key_hashes[j]) {
				if (keywords[j] == piece) {
					results.push_back(j);
				}
				i += keywords[j].size();
			}
		}
	}
	return results;
}
*/

//write to file
void writeout(set<Vertex*> vertices, string file) {
	ofstream ofs(file);
	for (set<Vertex* >::iterator it = vertices.begin(); it != vertices.end(); it++) {
		cout << (*it)->get_name() << endl;
		ofs << (*it)->get_name() << endl;
	}
	ofs.close();

}



int main(int argc, char *argv[]) {

	//arguments
	char* files = argv[1];
	char* keyword_file = argv[2];
	char* vystup = argv[3];


	//load files
	map<string,int> keywords = loadKeyWords(keyword_file);
	vector<string> filenames = loadFileList(files);

	string tmpfile;
	vector<int> tmp_vec;
	int i = 0;
	for (vector<string>::iterator it = filenames.begin(); it != filenames.end(); it++) {
		
		cout << *it << endl;
		tmpfile = loadFile(*it);
		regex reg = make_regex(keywords);
		tmp_vec = cleanFile(tmpfile,keywords, reg);
		all.add_new_vertex(*it, tmp_vec);
	}

	// clique
	set<Vertex*> clique = all.cliqueFrom(filenames[0]);

	writeout(clique, vystup);
	 	return 0;
}



