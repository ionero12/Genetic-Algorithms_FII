#include "sa.h"

int simulatedAnnealing::runCount = 0;

random_device rd;
mt19937 e2(rd());
uniform_real_distribution<> dist(0, 1);

bool goodPermutation(const vector<int>& v)
{
	int n = v.size();
	int i, j;
	for (i = 0; i < n - 1; ++i)
		for (j = i + 1; j < n; ++j)
			if (v[i] == v[j])
				return false;
	return true;
}

void simulatedAnnealing::generateRandomCandidate()
{
	int i;
	for (i = 0; i < dim; ++i)
	{
		currentCandidate[i] = i;
	}
	random_shuffle(currentCandidate.begin(), currentCandidate.end());
	currentIterationBest = evaluateCurrentCandidate();
}

void simulatedAnnealing::initSimulatedAnnealing()
{
	temperature = 1000;
}

bool simulatedAnnealing::simulatedAnnealingExploration()
{
	int other_pos, fi_pos, sec_pos;
	bool foundBetter = false;
	double currentValue;
	int i, j;
	for (i = 0; i < dim && !foundBetter; ++i)
		for (j = i + 1; j < dim && !foundBetter; ++j)
		{
			other_pos = j;
			swap(currentCandidate[i], currentCandidate[other_pos]);
			currentValue = evaluateCurrentCandidate();
			if (currentValue < currentIterationBest || (dist(e2) < exp(-abs(currentValue - currentIterationBest) / temperature)))
			{
				currentIterationBest = currentValue;
				foundBetter = true;
				fi_pos = i;
				sec_pos = other_pos;
			}
			swap(currentCandidate[i], currentCandidate[other_pos]);
		}
	temperature *= 0.9;
	if (foundBetter)
		swap(currentCandidate[fi_pos], currentCandidate[sec_pos]);
	return foundBetter && temperature >= 1e-30;
}

double simulatedAnnealing::evaluateCurrentCandidate()
{
	return helper::Euclidian_2D(currentMap, currentCandidate);
}

void simulatedAnnealing::runSimulatedAnnealing(int runs)
{
	int iterations = 100;
	bool running = true;
	cout << "\n";
	chrono::time_point<chrono::system_clock> start, end;
	double min, time;
	int i;
	for (i = 0; i < runs; ++i) 
	{
		vector<int> bestCandidate;
		start = chrono::system_clock::now();
		min = simulatedAnnealingRun(iterations, bestCandidate);
		end = chrono::system_clock::now();
		chrono::duration<double> elapsed_seconds = end - start;
		time = elapsed_seconds.count();
		int dim = bestCandidate.size();
		cout << "Iteration " << i + 1 << ": found optimal tour " << "with the minimum " << min << " in " << time << " seconds\n\n";
	}
	cout <<"\n";
}

double simulatedAnnealing::simulatedAnnealingRun(int iterations, vector<int>& bestCandidate)
{
	double best;
	currentCandidate.resize(dim);
	int i;
	for (i = 0; i < iterations; ++i)
	{
		generateRandomCandidate();
		initSimulatedAnnealing();
		while (simulatedAnnealingExploration());
		if (i == 0)
		{
			best = currentIterationBest;
			bestCandidate = currentCandidate;
		}
		else 
		{
			if (currentIterationBest < best)
			{
				best = currentIterationBest;
				bestCandidate = currentCandidate;
			}
		}
	}
	return best;
}