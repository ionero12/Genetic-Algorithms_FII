#include "file.h"

void tspMap::loadMap(string path)
{
	ifstream fileIn(path);
	string currentLine;
	double x, y;
	int id;
	while (getline(fileIn, currentLine))
	{
		if (currentLine == "NODE_COORD_SECTION")
			break;
	}
	points.clear();
	while (fileIn >> currentLine)
	{
		if (currentLine == "EOF")
			break;
		fileIn >> x >> y;
		points.emplace_back(x, y);
	}
	fileIn.close();
}

ostream& operator<<(ostream& out, const point& p)
{
	out << p.x << ' ' << p.y;
	return out;
}

ostream& operator<<(ostream& out, const tspMap& mp)
{
	out << mp.points.size() << '\n';
	for (auto& point : mp.points)
	{
		out << point << '\n';
	}
	return out;
}