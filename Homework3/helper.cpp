#include "fh.h"

double helper::Euclidian_2D(const tspMap& currentMap, const vector<int>& x)
{
	double tour_weight = 0;
	unsigned int chromo_size = x.size();
	unsigned int i;
	for (i = 1; i < chromo_size; ++i)
		tour_weight += sqrt((currentMap.points[x[i]].y - currentMap.points[x[i - 1]].y) * (currentMap.points[x[i]].y - currentMap.points[x[i - 1]].y) + (currentMap.points[x[i]].x - currentMap.points[x[i - 1]].x) * (currentMap.points[x[i]].x - currentMap.points[x[i - 1]].x));
	tour_weight += sqrt((currentMap.points[x[0]].y - currentMap.points[x[chromo_size - 1]].y) * (currentMap.points[x[0]].y - currentMap.points[x[chromo_size - 1]].y) + (currentMap.points[x[0]].x - currentMap.points[x[chromo_size - 1]].x) * (currentMap.points[x[0]].x - currentMap.points[x[chromo_size - 1]].x));
	return tour_weight;
}