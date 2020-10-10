#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

using namespace std;

class ObjLoader {
public:
	ObjLoader(string filename);
	vector<float> getVSets();
	vector<unsigned int> getFSets();
private:
	vector<float> vSets;
	vector<unsigned int> fSets;
	
};