#include <iostream>
#include <fstream>
#include <string>
#include "Graph.cpp"
#include <vector>
#include <regex>
#include <iterator>

#define RK_BASE 101
//vstup.txt kslova.txt out.txt
//vstup_lipsum.txt kslova_lipsum.txt out.txt

using namespace std;

Graph all;



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

//TODO: get rid of REGEX
vector<int> cleanFile(const string &s, map<string, int> &keywords, regex &words_regex) {
	vector<int> output;
	auto words_begin = sregex_iterator(s.begin(), s.end(), words_regex);
	auto words_end = sregex_iterator();
	for (sregex_iterator it = words_begin; it != words_end; ++it) {
		output.push_back(keywords[it->str()]);
	}
	return output;
}
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

int main(int argc, char *argv[]) {

	char* files = argv[1];
	char* keyword_file = argv[2];
	char* vystup = argv[3];

	map<string,int> keywords = loadKeyWords(keyword_file);
	vector<string> filenames = loadFileList(files);

	string tmpfile;
	vector<int> tmp_vec;
	int i = 0;
	for (auto it = filenames.begin(); it != filenames.end(); it++) {
		//if (++i > 21 /*|| i < 20 && i > 10*/)
			//continue;
		
		cout << *it << endl;
		tmpfile = loadFile(*it);
		//tmp_vec = rabin_karp_analyze(tmpfile, keywords);
		regex reg = make_regex(keywords);
		tmp_vec = cleanFile(tmpfile,keywords, reg);
		all.add_new_vertex(*it, tmp_vec);
	}

	all.cliqueFrom(filenames[0]);
	//all.writeout();
 	return 0;
}


int is_neighbor_LCS(pair<vector<int>, vector<int>> in)
{
	const vector<int> str1 = in.first;
	const vector<int> str2 = in.second;

	if (str1.empty() || str2.empty())
		return 0;

	int *curr = new int[str2.size()];
	int *prev = new int[str2.size()];
	int *swap = nullptr;
	int maxSubstr = 0;

	for (uint16_t i = 0; i < str1.size(); ++i) {
		for (uint16_t j = 0; j<str2.size(); ++j) {
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
				}
			}
		}
		swap = curr;
		curr = prev;
		prev = swap;
	}
	delete[] curr;
	delete[] prev;
	double comp = (ACCURACY * min(str2.size(), str1.size()));
	cout << comp << " =< ";
	cout << maxSubstr << endl;
	return (maxSubstr >= comp);
}
