#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

struct point
{
	double x, y;
	point(double px, double py)
	{
		x = px;
		y = py;
	}
	friend ostream& operator<<(ostream& out, const point& p);
};

class tspMap
{
public:
	vector<point> points;
	void loadMap(string path);
	friend ostream& operator<<(ostream& out, const tspMap& mp);
};
