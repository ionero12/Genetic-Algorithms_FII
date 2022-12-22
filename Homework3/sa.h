#pragma once
#include <fstream>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <random>
using namespace std;
#include "file.h"
#include "fh.h"

class simulatedAnnealing
{
private:
	static int runCount;
	const tspMap& currentMap;
	int	dim;
	double temperature;
	double currentIterationBest;
	vector<int> currentCandidate;
	void generateRandomCandidate();
	void initSimulatedAnnealing();
	bool simulatedAnnealingExploration();
	double evaluateCurrentCandidate();

public:
	simulatedAnnealing(const tspMap& map) : currentMap(map)
	{
		++runCount;
		dim = currentMap.points.size();
	}
	void runSimulatedAnnealing(int runs);
	double simulatedAnnealingRun(int iterations, vector<int>& bestCandidate);
}; 
