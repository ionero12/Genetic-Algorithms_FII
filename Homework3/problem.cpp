#include "problem.h"

void tsp::listOptions()
{
	cout << "   1. Genetic\n";
	cout << "   2. Simulated\n";
}

tsp::tsp(const string& mapPath)
{
	currentMap.loadMap(mapPath);
}

void tsp::run(int runs)
{
	bool running = true;
	listOptions();
	while (running)
	{
		string command;
		cout << "You choose: ";
		cin >> command;
		if (command == "Genetic")
		{
			geneticAlgorithm ga(currentMap);
			ga.runGeneticAlgorithm(runs);
			listOptions();
		}
		else
			if (command == "Simulated")
			{
				simulatedAnnealing sa(currentMap);
				sa.runSimulatedAnnealing(runs);
				listOptions();
			}
	}
}