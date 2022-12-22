#pragma once
#include <cmath>
#include <vector>
using namespace std;
#include "file.h"

#define swap(a, b) a^=b,b^=a,a^=b

namespace helper
{
	double Euclidian_2D(const tspMap& currentMap, const vector<int>& x);
}

