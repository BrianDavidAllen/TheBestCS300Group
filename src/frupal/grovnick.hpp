#ragma once
#include <string>
#include <iostream>
#include <fstream>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

class Grovnick {
	public:
		Grovnick(const int x, const int y, const bool visible, const int terrain, const string content);
        ~Grovnick();
        int getX();
        int getY();
        bool isVisible();
	bool isVisited();
    	int getTerrain();
        void setVisible();
	void setVisited(); 
		void saveState(ofstream &file) const;
        json toJson();
    private:
		int x, y;
		bool visible;
		bool visited;
		int terrain;
		string content;
};
