#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;
#include "file.h"
#include "genetic.h"
#include "sa.h"

class tsp
{
private:
	tspMap currentMap;
	void listOptions();

public:
	tsp(const string& mapPath);
	void run(int runs = 10);
};
