#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "grovnick.hpp"

using namespace std;

class Map {
	public:
		Map();
		string *parseLine(const string line) const;
		bool loadFile(const string identifier, const int dimensions, fstream &file);
		void fillMissingGrovnicks(int &currentX, int &currentY, const int nextX, const int endY);
		void generateFile(fstream &file);
	private:
		string identifier;
		int dimensions;
		vector<vector<Grovnick>> grovnicks;
};