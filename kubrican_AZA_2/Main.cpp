#include <iostream>
#include <fstream>
#include <string>
#include "Graph.cpp"
#include <vector>
#include <regex>
#include <iterator>


using namespace std;

Graph all;



vector<string> loadKeyWords(char * input ) {
	string line;
	vector<string> out;
	ifstream myfile((string)input);
	if (myfile.is_open())
	{
		int linenum = 0;
		while ( getline(myfile, line))
		{
			out.push_back(line);
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
		int linenum = 0;
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
		int linenum = 0;
		while (getline(myfile, line))
		{
			out.append(line);
		}
		myfile.close();
	}
	return out;
}

//TODO: get rid of REGEX
string cleanFile(const string s,vector<string> keywords) {
	string output;
	
	std::regex words_regex("(se|et|ut|ac|eu|eget|non|quis|vitae|mauris|tincidunt|sit|nec|amet|id|lorem|vel|vestibulum|nunc|donec|ipsum|nulla|orci|justo|pellentesque)+");
	auto words_begin =
		std::sregex_iterator(s.begin(), s.end(), words_regex);
	auto words_end = std::sregex_iterator();

	for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
		output.append( i->str() );
	}

	return output;
}





int main(int argc, char *argv[]) {

	char* files = argv[1];
	char* keyword_file = argv[2];
	char* vystup = argv[3];

	vector<string> keywords = loadKeyWords(keyword_file);
	vector<string>filenames = loadFileList(files);

	string tmpfile;
	for (auto it = filenames.begin(); it != filenames.end(); it++) {
		tmpfile = loadFile(*it);
		tmpfile = cleanFile(tmpfile,keywords);
		all.add_new_vertex(*it, tmpfile);
	}


	return 0;
}