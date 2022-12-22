#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <utility>
#include <vector>
#include <cmath>
using namespace std;
#include "problem.h"

string getPath(const string& map)
{
	ifstream fin("maps.txt");
	if (fin.is_open()) // not endoffile
	{
		string current_file, path;
		while (fin >> current_file)
		{
			fin >> path;
			if (map == current_file)
			{
				return path;
			}
		}
	}
	return "";
}

int main()
{
	srand(time(NULL));
	cout << "   run mapName\n";
	while (true)
	{
		string command;
		cin >> command;
		if (command == "run")
		{
			string map;
			cin >> map;
			string mapPath = getPath(map);
			if (mapPath.size() != 0)
			{
				tsp thisTsp(mapPath);
				thisTsp.run();
				cout << "run mapName\n";
			}
			else
				cout << "Invalid name!\n";
		}
	}
	return 0;
}
