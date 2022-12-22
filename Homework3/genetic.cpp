#include "genetic.h"

int geneticAlgorithm::r_count = 0;
mt19937 gen(std::random_device{}());

void geneticAlgorithm::runGeneticAlgorithm(int runs)
{
	unsigned int dec_size = current_map.points.size();
	cout << "\n";
	chrono::time_point<std::chrono::system_clock> start, end;
	cout << endl;
	double min, time;
	int i, j;
	for (i = 1; i <= runs; ++i)
	{
		start = chrono::system_clock::now();
		Genetic_Algorith();
		hillClimbingCandidate result(current_map, optimal_tour);
		min = result.hillClimbing(optimal_tour);
		end = chrono::system_clock::now();
		chrono::duration<double> seconds = end - start;
		time = seconds.count();
		cout << "Iteration " << i << ": found optimal tour"<< " with the minimum " << min<< " in " << time << " seconds." << endl << endl;
	}
}

void geneticAlgorithm::chromosome_generator(vector<int>& val)
{
	int i, map_size = current_map.points.size();
	uniform_int_distribution<> distance;
	for (i = 0; i < map_size - 1; ++i)
	{
		distance = uniform_int_distribution<>(0, map_size - i - 1);
		val.emplace_back(distance(gen));
	}
	val.emplace_back(0);
}

vector<int> geneticAlgorithm::decodeElem(const vector<int>& x)
{
	int i, map_size = current_map.points.size();
	vector<int> result;
	auto it = mold.begin();
	for (i = 0; i < map_size; ++i)
	{
		result.emplace_back(mold[x[i]]);
		mold.erase(it + x[i]);
		it = mold.begin();
	}
	set_mold();
	return result;
}

void geneticAlgorithm::set_mold()
{
	int i, map_size = current_map.points.size();
	for (i = 0; i <= map_size - 1; ++i)
		mold.emplace_back(i);
}

void geneticAlgorithm::init_pop()
{
	unsigned int population_size = standard_pop_size;
	int i;
	pop.resize(population_size);
	for (i = 0; i < population_size; ++i)
		chromosome_generator(pop[i]);
}

void geneticAlgorithm::hypermutation(int& count)
{
	uniform_real_distribution<> distance(0, 1);
	double mutation_prob;
	unsigned int population_size = pop.size(), chromosome_size = current_map.points.size();
	int i, j;
	if (count < limit)
		mutation_prob = mutation_chance;
	else
	{
		mutation_prob = 0.5;
		count = 1;
	}
	for (i = 0; i < population_size; ++i)
		for (j = 0; j < chromosome_size - 1; ++j)
			if (distance(gen) < mutation_prob)
				pop[i][j] = (pop[i][j] + 1) % (chromosome_size - j - 1);
}

void geneticAlgorithm::crossover()
{
	vector<double> crossover_chance;
	int i, j, size; 
	vector<int> aux_pop;
	double aux_chance;
	size = pop[0].size();
	uniform_real_distribution<> distance(0, 1);
	uniform_int_distribution<> distint(0, size - 2);
	bernoulli_distribution distber;
	int crossover_candidates_nr = 0;
	double crossover_p = cross_prob;
	vector<vector<int>> children;
	bool last_chromosome_chance;
	int pop_size = pop.size();
	for (i = 0; i < pop_size; ++i)
		crossover_chance.push_back(distance(gen));
	for (i = 1; i < pop_size; ++i)
	{
		aux_pop = pop[i];
		aux_chance = crossover_chance[i];
		j = i - 1;
		while (j >= 0 && crossover_chance[j] > aux_chance)
		{
			pop[j + 1] = pop[j];
			crossover_chance[j + 1] = crossover_chance[j];
			j--;
		}
		pop[j + 1] = aux_pop;
		crossover_chance[j + 1] = aux_chance;
	}
	i = 0;
	while (crossover_chance[i] < crossover_p && i < pop_size)
	{
		crossover_candidates_nr++;
		i++;
	}
	if (crossover_candidates_nr % 2 == 0)
	{
		children.resize(crossover_candidates_nr);
		for (i = 1; i < crossover_candidates_nr; i = i + 2)
		{
			int pos = distint(gen);
			children[i - 1] = pop[i - 1];
			children[i] = pop[i];
			for (j = pos; j < size; ++j)
			{
				children[i - 1][j] = pop[i][j];
				children[i][j] = pop[i - 1][j];
			}
		}
	}
	else
	{
		last_chromosome_chance = distber(gen);
		if (last_chromosome_chance == true)
		{
			crossover_candidates_nr++;
			children.resize(crossover_candidates_nr);
			for (i = 1; i < crossover_candidates_nr; i = i + 2)
			{
				int pos = distint(gen);
				children[i - 1] = pop[i - 1];
				children[i] = pop[i];
				for (j = pos; j < size; ++j)
				{
					children[i - 1][j] = pop[i][j];
					children[i][j] = pop[i - 1][j];
				}
			}
		}
		else
		{
			children.resize(crossover_candidates_nr - 1);
			for (i = 1; i < crossover_candidates_nr - 1; i = i + 2)
			{
				int pos = distint(gen);
				children[i - 1] = pop[i - 1];
				children[i] = pop[i];
				for (j = pos; j < size; ++j)
				{
					children[i - 1][j] = pop[i][j];
					children[i][j] = pop[i - 1][j];
				}
			}
		}
	}
	for (i = 0; i < children.size(); ++i)
		pop.push_back(children[i]);
}

void geneticAlgorithm::evalGen(vector<double>& fit)
{
	unsigned int population_size = pop.size();
	int i;
	for (i = 0; i < population_size; ++i)
		fit.push_back(1 / (0.00001 + helper::Euclidian_2D(current_map, decodeElem(pop[i]))));
}

unsigned int geneticAlgorithm::select1(const vector<double>& fs)
{
	uniform_real_distribution<> distance(0, 1);
	double probability = distance(gen) * fs.back();
	int i;
	for (i = 0; i < fs.size(); ++i)
		if (probability <= fs[i])
			return i;
}

void geneticAlgorithm::selection()
{
	vector<double> fit;
	unsigned int i;
	unsigned int poz;
	vector<vector<int>> newpop;
	unsigned int elitism_candidates_num = floor(elitism_rate * standard_pop_size);
	priority_queue<elitismCandidate, vector<elitismCandidate>, greater<elitismCandidate> >pq;
	evalGen(fit);
	for (i = 0; i < pop.size(); ++i)
	{
		pq.push(elitismCandidate(fit[i], i));
		if (pq.size() > elitism_candidates_num)
			pq.pop();
	}
	while (!pq.empty())
	{
		newpop.push_back(pop[pq.top().position]);
		pq.pop();
	}
	vector<double> fs;
	fs.emplace_back(fit[0]);
	for (i = 1; i < pop.size(); ++i)
		fs.emplace_back(fs[i - 1] + fit[i]);
	for (i = elitism_candidates_num; i < standard_pop_size; ++i)
		newpop.emplace_back(pop[select1(fs)]);
	pop = newpop;
}

void geneticAlgorithm::Genetic_Algorith()
{
	init_pop();
	set_mold();
	int i = 1;
	optimal_tour = decodeElem(pop[det_optimal_chromosome()]);
	unsigned opt_poz;
	double best_sol = helper::Euclidian_2D(current_map, optimal_tour);
	int counter = 1;
	double possible_sol;
	while (i <= number_of_gens)
	{
		hypermutation(counter);
		crossover();
		selection();
		opt_poz = det_optimal_chromosome();
		possible_sol = helper::Euclidian_2D(current_map, decodeElem(pop[opt_poz]));
		i++;
		if (possible_sol < best_sol)
		{
			best_sol = possible_sol;
			if (helper::Euclidian_2D(current_map, optimal_tour) > best_sol)
				optimal_tour = decodeElem(pop[opt_poz]);
			optimal_gen = i;
			counter = 1;
		}
		else
			counter++;
	}
}

unsigned int geneticAlgorithm::det_optimal_chromosome()
{
	unsigned int poz = 0;
	double minimum = helper::Euclidian_2D(current_map, decodeElem(pop[0]));
	double aux;
	int j;
	for (j = 1; j < pop.size(); j++)
	{
		aux = helper::Euclidian_2D(current_map, decodeElem(pop[j]));
		if (minimum > aux)
		{
			minimum = aux;
			poz = j;
		}
	}
	return poz;
}

double geneticAlgorithm::calc_f_value()
{
	unsigned int pop_size = pop.size();
	double sol = helper::Euclidian_2D(current_map, decodeElem(pop[0]));
	double aux;
	int j;
	for (j = 1; j < pop_size; j++)
	{
		aux = helper::Euclidian_2D(current_map, decodeElem(pop[j]));
		if (sol > aux)
			sol = aux;
	}
	return sol;
}

bool operator>(const elitismCandidate& c1, const elitismCandidate& c2)
{
	return c1.value > c2.value;
}

