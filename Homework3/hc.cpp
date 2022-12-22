#include "hc.h"

bool hillClimbingCandidate::hillClimbingExploration()
{
	bool foundBetter = false;
	int other_pos, fi_pos, sec_pos;
	double currentValue;
	int i, j;
	for (i = 0; i < dim - 1; ++i)
		for (j = i + 1; j < dim; ++j)
		{
			other_pos = j;
			swap(currentCandidate[i], currentCandidate[other_pos]);
			currentValue = evaluateCurrentCandidate();
			if (currentValue < currentIterationBest)
			{
				currentIterationBest = currentValue;
				foundBetter = true;
				fi_pos = i;
				sec_pos = other_pos;
			}
			swap(currentCandidate[i], currentCandidate[other_pos]);
		}
	if (foundBetter)
		swap(currentCandidate[fi_pos], currentCandidate[sec_pos]);
	return foundBetter;
}

double hillClimbingCandidate::hillClimbing(vector<int>& finalCandidate)
{
	cout << "Before HC: " << currentIterationBest;
	while (hillClimbingExploration());
	cout << ", after HC: " << currentIterationBest << '\n';
	finalCandidate = currentCandidate;
	return currentIterationBest;
}

double hillClimbingCandidate::evaluateCurrentCandidate()
{
	return helper::Euclidian_2D(currentMap, currentCandidate);
}
