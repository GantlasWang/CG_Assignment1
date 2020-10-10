#include "ObjLoader.h"
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

ObjLoader::ObjLoader(string filename) {
	string sline;
	ifstream file;
	file.open(filename, ios::in);

	if (file.bad()) {
		cout << "Opening Objfiles Failed" << endl;
	}

	while (!file.eof() && file.good()) {
		getline(file, sline);
		vector<string> vline;

		/*
		* 在每一行末尾加入一个空格，便于取出元素
		*/
		string tail = " ";
		string element = "";

		sline = sline.append(tail);
		for (int i = 0; i < sline.length(); ++i) {
			char ch = sline[i];
			if (ch != ' ') {
				element += ch;
			}
			else {
				vline.push_back(element);
				element = "";//刷新element
			}
		}

			if (vline[0] == "v") {
				for (int i = 1; i < vline.size(); ++i) {
					vSets.push_back(atof(vline[i].c_str()));
				}
			}
			else if (vline[0] == "f") {
				for (int i = 1; i < vline.size(); ++i) {
					/*
					* string vertex_index = "";
					* for (int j = 0; j < vline[i].length(); ++j) {
					*	char ch = vline[i][j];
					*	if (ch != '/') {
					*		vertex_index += ch;
					*	}
					*	else {
					*		fSets.push_back(atoi(vertex_index.c_str())-1);
					*		break;
					*	}
					* }
					*/
					fSets.push_back(atoi(vline[i].c_str())-1);
				}
			}
			else if (vline[0] == "#") {

			}
		
	}
	file.close();
}

vector<float> ObjLoader::getVSets() {
	return this->vSets;
}

vector<unsigned int> ObjLoader::getFSets() {
	return this->fSets;
}
