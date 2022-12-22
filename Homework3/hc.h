#pragma once
#include <vector>
using namespace std;
#include "file.h"
#include "fh.h"

class hillClimbingCandidate
{
private:
	vector<int> currentCandidate;
	const tspMap& currentMap;
	double currentIterationBest;
	int	dim;
	bool hillClimbingExploration();
	double evaluateCurrentCandidate();

public:
	hillClimbingCandidate(const tspMap& map, const vector<int> candidate) : currentMap(map)
	{
		currentCandidate = candidate;
		currentIterationBest = evaluateCurrentCandidate();
		dim = candidate.size();
	}
	double hillClimbing(vector<int>& finalCandidate);
};
