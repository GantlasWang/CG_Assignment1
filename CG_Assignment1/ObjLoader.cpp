#include "ObjLoader.h"
#include <fstream>
#include <iostream>

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

		if (vline.size() != 4) {
			cout << "Size is not corret" << endl;
		}
		else {
			if (vline[0] == "v") {
				vector<GLfloat> vertex;
				for (int i = 1; i < vline.size(); ++i) {
					vertex.push_back(atof(vline[i].c_str()));
				}
				vSets.push_back(vertex);
			}
			else if (vline[0] == "f") {
				vector<GLint> face;
				for (int i = 1; i < vline.size(); ++i) {
					/*
					* string vertex_index = "";
					* for (int j = 0; j < vline[i].length(); ++j) {
					*	char ch = vline[i][j];
					*	if (ch != '/') {
					*		vertex_index += ch;
					*	}
					*	else {
					*		face.push_back(atoi(vertex_index.c_str())-1);
					*		break;
					*	}
					* }
					*/
					face.push_back(atoi(vline[i].c_str())-1);
				}
				fSets.push_back(face);
			}
		}
	}
	file.close();
}

void ObjLoader::Draw() {

}